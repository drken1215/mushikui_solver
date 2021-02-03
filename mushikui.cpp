#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <map>

/////////////////////////////////
// 虫食算を解く過程を管理するクラス
/////////////////////////////////

class Mushikuizan {
 private:
    // 掛けられる数と、掛ける数の桁数 (a, b とする)
    int a_, b_;

    // 虫食算の問題を表す情報
    std::vector<std::string> problem_;

    // 虫食算を解く過程を表す情報
    std::vector<std::vector<int>> field_;

    // 使用された数値
    std::set<int> used_;

    // 各文字がどの数値を表すか
    std::map<char, int> num_;

    // 各文字に数値が置かれた瞬間がいつだったか
    std::map<char, std::pair<int, int>> put_;

 public:
    // コンストラクタ
    Mushikuizan(int a, int b, 
                const std::vector<std::string> &input);

    // 掛けられる数と、掛ける数の桁数を取得する
    int get_a() { return a_; }
    int get_b() { return b_; }

    // r 段めの d 桁めに数値があるかどうか
    bool exist_val(int r, int d) {
        return (problem_[r][d] >= '0' 
                && problem_[r][d] <= '9');
    }

    // r 段めの d 桁めに文字があるかどうか
    bool exist_char(int r, int d) {
        return (!exist_val(r, d) 
                && problem_[r][d] != '*');
    }

    // 掛ける数の d 桁めにすでに数値がある場合その値を返す
    // そうでない場合は -1 を返す
    int already(int d) { return field_[1][d]; }

    // r 段めの d 桁めに値 v を置く (矛盾が生じるときは false)
    bool set_val(int r, int d, int v);

    // r 段めの d 桁めの値をリセットする
    void reset_val(int r, int d);

    // r 段めの値を計算する
    std::vector<int> calc(int r);

    // 最終段階において、最下段の値を計算する
    std::vector<int> calc_answer();

    // 解を出力する
    void print();
};


// コンストラクタ
Mushikuizan::Mushikuizan(int a, int b, 
const std::vector<std::string> &input): a_(a), b_(b) {
    // 問題の情報を取得する
    for (int i = 0; i < input.size(); ++i) {
        std::string str = input[i];
        std::reverse(str.begin(), str.end());

        // str の桁数が小さい場合
        // 便宜上 1 桁増やして最上位の値を 0 とする
        if (i == input.size() - 1) {
            // 最下段の場合
            if (str.size() == a + b - 1) str += "0";
        } else if (i >= 2) {
            // それ以外の段の場合
            if (str.size() == a) str += "0";
        }
        problem_.emplace_back(str);
        field_.emplace_back(
            std::vector<int>(str.size(), -1));
    }
}

// r 段めの d 桁めに値 v を置く
// 矛盾する場合は false を返す
bool Mushikuizan::set_val(int r, int d, int v) {
    // 掛けられる数の最上桁には 0 を置けない
    if (r == 0 && d == get_a() - 1 && v == 0) {
        return false;
    }

    // 最上位桁の条件があるときに、0 が入るのは false
    if (d == problem_[r].size() - 1
        && problem_[r][d] != '0'
        && v == 0) {
        return false;
    }

    // もともと数値があって一致しない場合は false
    if (exist_val(r, d) && problem_[r][d] - '0' != v) {
        return false;
    }

    // もともと文字がある場合
    if (exist_char(r, d)) {
        if (num_.count(problem_[r][d])) {
            // すでに置かれた数値と一致しない場合は false
            if (num_[problem_[r][d]] != v) return false;
        } else {
            // ほかの文字にすでに v が置かれている場合は false
            if (used_.count(v)) return false;

            // 文字に数値を置く
            used_.insert(v);
            num_[problem_[r][d]] = v;
            put_[problem_[r][d]] = {r, d};
        }
    }

    // 数値を置いて true を返す
    field_[r][d] = v;
    return true;
}

// r 段めの d 桁めの値をリセットする
void Mushikuizan::reset_val(int r, int d) {
    // 数値の置かれていない文字がある場合、文字情報を削除
    if (exist_char(r, d)
        && put_[problem_[r][d]].first == r
        && put_[problem_[r][d]].second == d) {
        used_.erase(num_[problem_[r][d]]);
        num_.erase(problem_[r][d]);
        put_.erase(problem_[r][d]);
    }
    field_[r][d] = -1;
}

// r 段めの値を計算する
std::vector<int> Mushikuizan::calc(int r) {
    std::vector<int> res(get_a() + 1, 0);
    for (int i = 0; i < get_a(); ++i) {
        // -1 に到達したら打ち切る
        if (field_[0][i] == -1) break;

        // i 桁めについて掛け算を実施する
        res[i] += field_[0][i] * field_[1][r - 2];

        // 繰り上がりを計算する
        res[i + 1] = res[i] / 10;
        
        // i 桁めを確定する
        res[i] %= 10;
    }
    return res;
}

// 最終段階において、最下段の値を計算する
std::vector<int> Mushikuizan::calc_answer() {
    // 各段の値を計算する
    std::vector<std::vector<int>> tmp(field_.size());
    for (int r = 2; r < tmp.size() - 1; ++r) 
        tmp[r] = calc(r);

    // 各段の総和をとる
    std::vector<int> res(get_a() + get_b(), 0);
    for (int i = 0; i < res.size(); ++i) {
        for (int r = 2; r < tmp.size() - 1; ++r) {
            if (i >= r - 2
                && i - r + 2 < tmp[r].size()) 
                res[i] += tmp[r][i - r + 2];
        }
        if (i + 1 < res.size()) 
            res[i + 1] = res[i] / 10;
        res[i] %= 10;
    }
    return res;
}

// 解を出力する
void Mushikuizan::print() {
    auto print_vec = [&](const std::vector<int> &v) -> void {
        for (int i = v.size() - 1; i >= 0; --i)
            std::cout << v[i];
        std::cout << std::endl;
    };
    std::cout << "-----------" << std::endl;
    print_vec(field_[0]), print_vec(field_[1]);
    for (int r = 2; r < field_.size() - 1; ++r) {
        std::vector<int> sub = calc(r);
        if (sub.back() == 0) sub.pop_back();
        print_vec(sub);
    }
    std::vector<int> total = calc_answer();
    if (total.back() == 0) total.pop_back();
    print_vec(total);
}


///////////////////////////
// ソルバー
///////////////////////////

void rec1(Mushikuizan &mu, int d,
          std::vector<Mushikuizan> &res);
void rec2(Mushikuizan &mu, int d1, int d,
          std::vector<Mushikuizan> &res);

// 最下段の値を val としてよいかどうかを再帰的に確認する
void check(Mushikuizan &mu, const std::vector<int> &val,
          int d, std::vector<Mushikuizan> &res) {
    // 終端条件
    if (d == val.size()) {
        res.emplace_back(mu);
        return;
    }

    // d 桁めの値を val[d] として問題なければ次の桁に進む
    if (mu.set_val(mu.get_b() + 2, d, val[d])) {
        check(mu, val, d + 1, res);
        mu.reset_val(mu.get_b() + 2, d);
    }
}

// 虫食算の掛けられる数の d 桁めを再帰的に処理していく
void rec1(Mushikuizan &mu, int d,
          std::vector<Mushikuizan> &res) {
    // 最後に、最下段を確認していく
    if (d == mu.get_a() + 1) {
        auto val = mu.calc_answer();
        check(mu, val, 0, res);
        return;
    }

    if (d == mu.get_a()) {
        // 掛けられる数がすべて埋まったならば
    	// 掛けられる数のまだ埋まっていないところを順に埋めていく
        rec2(mu, d, 0, res);
    } else {
        for (int v = 0; v <= 9; ++v) {
            // 掛けられる数の d 桁めに v を入れる
            if (mu.set_val(0, d, v)) {
                rec2(mu, d, 0, res);
                mu.reset_val(0, d);
            }
        }
    }
}

// 虫食算の掛ける数を d 桁めを再帰的に処理していく
// 掛けられる数は d1 桁めまで埋まっていることを仮定する
void rec2(Mushikuizan &mu, int d1, int d,
          std::vector<Mushikuizan> &res) {
    // 終端条件
    if (d == mu.get_b()) {
        rec1(mu, d1 + 1, res);
        return;
    }

    // 掛けられる数がすべて埋まっていない、かつ
    // d1 桁めにヒントがない場合はスキップして次に進む
    if (d1 != mu.get_a()
        && !mu.exist_val(d + 2, d1)
        && !mu.exist_char(d + 2, d1)) {
        rec2(mu, d1, d + 1, res);
        return;
    }
    
    if (mu.already(d) != -1) {
      	// すでに数値が入っている場合
        auto val = mu.calc(d + 2);
        if (mu.set_val(d + 2, d1, val[d1])) {
            rec2(mu, d1, d + 1, res);
            mu.reset_val(d + 2, d1);
        }
    } else {
        for (int v = 1; v <= 9; ++v) {
            // 掛ける数の d 桁めに v を入れる
            if (mu.set_val(1, d, v)) {
                auto val = mu.calc(d + 2);
                if (mu.set_val(d + 2, d1, val[d1])) {
                    rec2(mu, d1, d + 1, res);
                    mu.reset_val(d + 2, d1);
                }
                mu.reset_val(1, d);
            }
        } 
    }
}

// 虫食算を解く
std::vector<Mushikuizan> solve(Mushikuizan &mu) {
    std::vector<Mushikuizan> res;
    rec1(mu, 0, res);
    return res;
}

int main() {
    // 入力
    int a, b;
    std::cin >> a >> b;
    std::vector<std::string> input(b + 3);
    for (int i = 0; i < b + 3; ++i) 
        std::cin >> input[i];

    // 虫食算を解く
    Mushikuizan mu(a, b, input);
    std::vector<Mushikuizan> res = solve(mu);

    // 解を出力
    for (int i = 0; i < res.size(); ++i) {
        std::cout << i << " th result:" << std::endl;
        res[i].print();
    }
}
