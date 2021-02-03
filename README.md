# 概要

虫食算ソルバーを実装しています。虫食算の問題を標準入力として与えると、それを解いて解を出力します。下図は、虫食算の例と、その解答を表しています。

　

<img src=https://github.com/drken1215/mushikui_solver/blob/master/mushikui_example.png width=500mm>

(虫食算の問題例)

<img src=https://github.com/drken1215/mushikui_solver/blob/master/mushikui_snswer.png width=500mm>

(虫食算の問題例の解答)

　

# 使い方

## 使用言語

- Sudoku.cpp を C++11 上でコンパイルして実行します。
- Wandbox 上の gcc 10.1.0 で動作します。



## 入力

虫食算の入力はリスト 1 のような形式で、標準入力で受け取るものとします。リスト 1 は、下図の虫食算を表しています。1 行めには、掛けられる数の桁数 ($a$ とします) と、掛ける数の桁数 ($b$ とします) を記入します。続く 2 行は、掛けられる数の情報と、 掛ける数の情報を表します。さらに続く $b$ 行は、筆算の過程を表します。そして最後の行は、筆算の結果を表します。また、虫食算の ⬜︎ に対応するところは文字 `*` で表しています。

　

```
4 3
4*F*
*O*
F*U*
**O*R
**U*
***R***
```

(虫食算の入力例)

　

## 出力

- 一意解の場合は、たとえば先述の入力例に対しては、以下の結果を出力します。

```
0 th result:
-----------
4794
232
9588
14382
9588
1112208
```



- 解なしの場合は "no solutions." と出力します。
- 解が複数個ある場合は "more than one solutions." と出力します。

　

# 実行例

下図のような超巨大虫食算数に対しても **0.38 秒**の計算時間で解くことができます。

なお、計算実行環境は、以下の通りです。

- MacBook Air (13-inch, Early 2015)
- プ ロ セ ッ サ：1.6 GHz Intel Core i5
- メモリ：8GB



<img src=https://github.com/drken1215/mushikui_solver/blob/master/mushikui_super_large.png width=750mm>

(超巨大虫食算)

　

# アルゴリズム

ソルバーのベースは、**深さ優先探索 (バックトラッキング)** です。探索順序の工夫や枝刈りなどを行うことで高速化を図っています。詳細については下記の参考資料に書かせていただきました。

　

# 参考資料

以下の連載記事に詳細の解説を行っています。

- 技術評論社：**Software Design** 2021年2月号 
  - https://gihyo.jp/magazine/SD/archive/2021/202102
  - 【連載】パズルで鍛えるアルゴリズム力
    【7】虫食算と覆面算の融合！
    ……けんちょん（大槻 兼資）

　

# License

These codes are licensed under CC0.

[![CC0](http://i.creativecommons.org/p/zero/1.0/88x31.png "CC0")](http://creativecommons.org/publicdomain/zero/1.0/deed.ja)