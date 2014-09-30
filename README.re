= Android Open Textbook

[![Circle CI](https://circleci.com/gh/TechBooster/AndroidOpenTextbook.png?style=badge)](https://circleci.com/gh/TechBooster/AndroidOpenTextbook)

Android Open Textbook は「Androidの教科書」をつくるオープンソースプロジェクトです。
GitHubと書籍制作ツールRe:VIEWを利用して、開発者が欲しい技術情報を集約する試みです。

== マイルストーン

成果はオープンソースプロジェクトとしてGitHub上で公開していくほか、2014年夏のコミックマーケットで書籍として頒布予定です（７月前後が初期版のマイルストーンです）。また本プロジェクトの成果の一部はアプリ開発人材育成プログラム「Tech Institute」で利用され、フィードバックを得ます。

== 関連する外部のプレスリリース

サムスン電子、早稲田大学EXセンタ、角川アスキー総研がアプリ開発人材育成プログラム「Tech Institute」の募集を開始！
@<href>{http://mobileascii.jp/elem/000/000/102/102939/}

== 連絡窓口

@<href>{https://twitter.com/mhidaka,@mhidaka}　（コラボレーターに個別に連絡すると混乱するので代表窓口として）

== コラボレーター

（編集中かつ募集中）

== Re:VIEWとは

Re:VIEW は、EWB や RD あるいは Wiki に似た簡易フォーマットで記述したテキストファイルを、目的に応じて各種の形式に変換するツールセットです。Android Open TextbookはRe:VIEWを使って書かれています。詳細については以下の公式Wiki等を参照して下さい。

 * @<href>{https://github.com/kmuto/review/blob/master/doc/quickstart.rdoc,Re:VIEWクイックスタートガイド}
 * @<href>{https://github.com/kmuto/review/blob/master/doc/format.rdoc,Re:VIEW フォーマット}


== ビルド方法

本節では、Android Open Textbookを各種環境でビルドする方法を解説します。

=== Windows向け

TODO

=== Mac向け

==== npmをインストールする

Android Open Textbookのビルド等の操作は@<href>{http://gruntjs.com/, grunt}を用いて行います。@<href>{http://gruntjs.com/, grunt}はnpmが必要です。

//emlist{
brew install npm
npm install -g grunt-cli
//}

==== Re:VIEWをインストールする

Re:VIEWはGithub上のソースをcloneしてパスを通せば利用できます。

//emlist{
git clone https://github.com/kmuto/review.git
export PATH=$PATH:/path/to/review/bin
//}

==== セットアップ

Android Open Textbookをクローンし、必要なライブラリ等をインストールします。

//emlist{
git clone https://github.com/TechBooster/AndroidOpenTextbook.git
cd AndroidOpenTextbook
npm install
//}

==== Readmeをビルドする

まずはREADME.mdをビルドしてみましょう。README.mdは実はREADME.reから生成しています。以下のコマンドでREADME.mdの生成が行えます。

//emlist{
grunt readme
//}

本ページ(README.md)を更新する場合は、README.reを編集し、上記コマンドでREADME.mdを生成して下さい。

==== 原稿をビルドし、HTMLを出力する

以下のコマンドを実行すると、articlesディレクトリ内の原稿をコンパイルしてHTMLファイルを出力します。

//emlist{
grunt html
//}

==== 原稿をビルドし、PDFを出力する

PDFを出力するには、[MacTeX](http://tug.org/mactex/)が必要です。まずは[MacTeX](http://tug.org/mactex/)をインストールしてください。

インストール後、以下のコマンドでPDFを出力できるようになります。

//emlist{
grunt pdf
//}


==== タスクの種類を確認する

gruntのタスクのバリエーションはGruntfile.jsに定義されています。以下のコマンドで、gruntでどのような事ができるか確認できます。

//emlist{
grunt --help
//}
