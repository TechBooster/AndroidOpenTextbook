= ユーザーインターフェイス基礎

実際にアプリを作っていく上で必要な画面のレイアウトやデザイン。これらを、「ユーザーが操作していくもの」という意味も含めて、ユーザーインターフェイスと呼びます。
たとえば、テキストを入力するものや、押すと何かが起こるボタンなどです。また、操作するわけではないですが、アプリの重要な要素として、テキストや画像などもそのうちのひとつです。

Androidアプリのユーザーインターフェイスは、プロジェクトの「res」フォルダ内のXMLファイルに記述していきます。

慣れてきたら自分でXMLを作成していくのですが、Androidにどんなパーツがあるのかがまだわからないうちは、いきなりXMLを書いていくのは少しむずかしいでしょう。
最初は、ドラッグアンドドロップで簡単にインターフェイスを作ることができる「Graphical Layout」を使って、Androidにはどのようなパーツがあるのかを見ていきます。
その後で、XMLを使ったレイアウトの作成方法をみていきましょう。


== Graphical Layoutを使ってレイアウトを作ってみよう

この節では、ADTについている「Graphical Layout」を使ってレイアウトを完成させます。
細かいことは気にせず、Androidにはどのようなパーツがあり、どんなときに使うのかをざっと把握することが目的です。

=== 新規プロジェクトを作成し、「Graphical Layout」を表示する

ADTを立ち上げ、新たにAndroidプロジェクトを作成します。
そうすると、「MainActivity.java」「fragment_main.xml」のふたつのファイルが開かれた状態になります。

上のタブで「fragment_main.xml」を選び、下のタブが「Graphical Layout」になっていることを確認します。

//image[001.png][]{
//}

=== Graphical Layout の見方を確認しよう

Graphical Layout を開くと、Androidの画面が大きくあらわれます。初期状態では、「Hello world!」というテキストがひとつ書かれているのがわかります。
他にもいくつかのパネルがあります。

＜001にA〜Gとそのタイトルを入れる＞

(A)パレット：各パーツが入っている
(B)プレビュー：現在設定されているものがプレビューされます
(C)プレビューに関する端末の設定：プレビューを表示する端末の種類やバージョンを設定します
(D)パーツのレイアウトに関する設定：パーツのレイアウト設定をします
(E)プレビューに関するビューの設定：プレビューを表示する大きさを設定します
(F)アウトライン：配置したパーツがどういう階層構造になっているのかを設定します
(G)プロパティ：配置したパーツの詳細設定をしていきます

最初からすべてを把握するのは大変です。必要なものはそのときどきで説明していきますので、まずはパーツを配置してみることからはじめていきましょう。



=== パーツを配置してみよう

(A)のパレットから好きなパーツを選んで、(B)のプレビュー領域にドラッグアンドドロップをしてみましょう。

//image[002.png][ドラッグ中]{
//}
//image[003.png][ドラッグ完了]{
//}
(A)パレットの「Form Widgets」の中から「Button」を選んでドラッグアンドドロップし、Buttonを配置した

ボタンが配置されました。

=== プロパティパネルを使ってカスタマイズする

先ほど配置したボタンは、「Button」と書かれています。
このままではいったい何のボタンかわかりませんので、文字を変更します。

プレビュー画面のボタンを選択した状態で、プロパティパネルを見ると、ボタンのプロパティがたくさん出ています。


//image[004.png][ボタンのプロパティ]{
//}

この中にある「Text」という項目が、ボタンのラベルになっていますので、右の列の「Button」と書かれてあるところをダブルクリックし、「送信」に変えてみましょう。
（Textは2ヶ所ありますが、どちらも同じです）

//image[005.png][ボタンのラベルを「送信」に変えた]{
//}

そうすると、プレビューエリアのボタンのラベルも変わりました。


//image[006.png][プレビューエリアのボタンのラベルも「送信」に変わった]{
//}

このように、プロパティパネルにある値をいろいろと変えていくことで、パーツのカスタマイズができます。

=== Graphical Layoutのパーツ一覧

パーツは、パレットエリアに、カテゴリごとにフォルダ分けされています。他のフォルダの中も見てみましょう。

//image[007.png][]{
//}
//image[008.png][]{
//}
//image[009.png][]{
//}
//image[010.png][]{
//}
//image[011.png][]{
//}
//image[012.png][]{
//}
//image[013.png][]{
//}
//image[014.png][]{
//}
//image[015.png][]{
//}

パレットの中に入っているパーツは、配置するだけで、アプリ内でそのまま使えるものもあります。たとえば、テキストや画像などは何かを表示するだけのものですので、配置するだけでその役目を果たします。
しかし多くの場合はそうではなく、たとえば、テキストを入力してボタンを押したらどうなるか、というところをプログラミングしなければ、アプリとして成立しません。
それらは、配置するだけなら「レイアウトデザイン」として見せかけだけの画面はできあがりますが、アプリとして何か使えるようになるわけではないのです。

また、プログラミングによって関連付けをしなければ、たとえレイアウトデザインとしてであっても使えないパーツもたくさんあります。
「Composite」フォルダの中などはそういったものばかりです。

ここでは、ボタンを押しても何も起こらないような、見せかけだけのレイアウトを作っていくことを頭に置いておきましょう。

=== 実習1：Graphical Layoutを使って、次のインターフェイスを作成してみよう


//image[016.png][]{
//}


==== 解説

基本的には、あてはまるパーツをパレットからドラッグアンドドロップして配置することで、完成します。
ただし、いくつかの注意点があります。

===== 1. Text Fieldsの種類

このサンプルには、

 * お名前
 * Email
 * お電話番号
 * パスワード

の4つのテキストフィールドがあり、それぞれ「Text Fields」フォルダの中から選んで配置しますが、それぞれの「Input Type」が異なるものを選んでくるのが最適です。

//image[017.png][E-mailでは、Input Typeの値が「textEmailAddress」になっている]{
//}

//image[018.png][]{
//}

//image[019.png][]{
//}

//image[020.png][]{
//}

//image[021.png][それぞれの特徴にあったText Fieldsを選ぶ]{
//}

こうすることで、Android OSが最適なIME（日本語入力）キーボードを自動で表示することができます。
たとえば、「Plain Text」のときは通常の日本語入力キーボードですが、「E-mail」にしたときは英数字キーボードで、しかも「@」も表示されているのがわかります。
また、電話番号は「Phone」にすれば数字キーボードになります。パスワードも、「Password」にすると、入力済みの文字が「●」で表示され、見えなくなるようになっています。

すべて同じ「Plain Text」にしても間違いではないのですが、もし自分が入力する立場になったときのことを考えると、メールアドレスを入力したいときに、日本語キーボードが表示されていたら、英数字キーボードに切り替えるのは手間ですよね。
Input Typeで表示させるキーボードを切り分けることによって、@<b>[よりユーザーが使いやすいインターフェイスを表現できる]のです。

===== 2. ラジオグループ

ラジオボタンは、ひとまとまりのグループにおいて、ひとつの項目しか選択できない状態でなければなりません。

そうするためには、「Form Widgets」の中の「RadioGroup」を選ぶべきです。「RadioButton」を選んでしまうと、それぞれが単独で選べることになってしまいますので、注意しましょう。

//image[022.png][ラジオボタンをそれぞれ単独で選ぶことができる状態。こうならないようにしよう]{
//}

===== 3. 「登録する」というボタン

普通にボタンを置いた状態では、ボタンの横幅が文字数に応じたものになってしまいます。



//image[023.png][ボタンの横幅が、「登録する」の4文字分しかない]{
//}

これでは、小さくて押しづらくなってしまいます。
このボタンは、すべての項目を入力したあとの「入力完了」という意味も含めて押すものですので、横幅いっぱいに大きく配置しましょう。

まずはボタンを配置し、「登録する」というラベルに変更したあとに、マウスをうまく使って横幅いっぱいに伸ばします。


//image[024.png][ボタンの横幅を調整する]{
//}

ここまで完成したら、次の節ではXMLでインターフェイスを作っていきます。


== XMLを使ってパーツを作れるようになる

Graphical Layoutではドラッグアンドドロップでだれでもレイアウトを作ることができます。しかし、細かいレイアウトの設定は、XMLでしていかなければなりません。

この節では、XMLを使ってよく使うパーツを作れるようになりましょう。

=== XMLレイアウトがどんなものかを知る

まず、XMLレイアウトがどんなものかを知るために、あらかじめ配置されているXMLをカスタマイズしていきます。

新規Androidプロジェクトを作成します。
そして、立ち上がった「fragment_main.xml」を開きます。Graphical Layoutを見ると、「Hello world!」が表示されています。


//image[201][Graphical Layoutを見る]{
//}

下のタブを「fragment_main.xml」に切り替えてみましょう。

ここに書かれているのが、XMLレイアウトです。


//image[202][XMLレイアウトが書かれている]{
//}

テキストが表示されている部分のXMLは、次のようになっています。

//list[201][テキストが表示されている部分のXML]{
<TextView
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:text="@string/hello_world" />
//}

==== テキストの文字列を変更する

4行目の「android:text=””」の中で、表示する文字列を指定しています。ここでは、@stringといって他のファイルを参照するように指定されていますが、いったん無視して、好きなテキストに変えてみます。

//list[202][4行目を追加した]{
<TextView
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:text="XMLレイアウトの基礎" />
//}

確認方法は、AVDでも実機でもどちらでもかまいません。簡易的な確認であれば、Graphical Layoutでもできます。

//image[203][出力結果。Graphical Layoutで確認した場合]{
//}

先ほど「Hello world!」だったテキストが、入力したものに変わっていることが確認できます。

==== テキストの色や大きさを変更する

では次に、テキストの色や大きさを変更してみましょう。
先ほどの@<list>[202]に、次の2行を追加します。

//list[203][5〜6行目を追加した]{
<TextView
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:text="XMLレイアウトの基礎"
        android:textColor="#ff0000"
        android:textSize="22sp" />
//}

「android:textColor」がテキストの色を指定する属性で、「#ff0000（赤色）」を指定しました。
「android:textSize」はテキストの大きさを指定する属性で、「22sp」を指定しました。
これらを追加することによって、どのような表示になるかはだいたい想像がつきますよね。


//image[204][出力結果。テキストが赤色になり、大きくなった]{
//}

テキストが赤色になり、大きさが大きくなったのが確認できます。

このように、さまざまな属性を指定していくことで、Androidのレイアウトを作成していくのです。

=== XMLの書き方

XMLは、次のような要素で構成されています。


//image[205][テキストを表示させるXML]{
//}

 * 要素名：何を表示させるか
 * 属性：要素に対する詳細な設定。ほとんどの場合、先頭に「android:」の接頭辞が付く
 * 値：属性の設定値

==== 要素名

要素名は、よく使うものとして次のようなものがあります。

//table[よく使う要素名][]{
Button  ボタンを表示する
TextView        テキストを表示する
ImageView       画像を表示する
EditText        テキストフィールド（文字入力エリア）を表示する
CheckBox        チェックボックスを表示する
RadioButton     ラジオボタンを表示する
ProgressBar     プログレスバーを表示する
SeekBar シークバーを表示する
//}

==== 属性

属性は、各要素によって設定できるものが決まっています。
たとえば、テキストを表示するTextViewであれば、色（textColor）や大きさ（textSize）の設定ができますが、画像を表示するImageViewだと、それらの設定はありません。

各要素に設定できる属性は、それぞれにおいてかなりの数があります。
どのような属性が指定できるかは、コードを書いている中で「android:」と打っていくと、指定可能な候補一覧が表示されますので、それを見ておくとよいでしょう。


//image[206][TextViewで指定可能な属性の候補が表示される]{
//}

==== 値

値は、各属性によって設定できるものが決まっています。
これも、設定できるものが選べる場合は、属性と同じように候補一覧が表示されますので、見ておくとよいでしょう。


//image[207][TextViewのtextStyleという属性で指定可能な値の候補が表示される]{
//}

=== 主要なパーツを作ってみる

それでは、よく使うパーツを詳しく説明していきます。
これらのパーツは、何も見なくてもXMLで作成できるようにしておくことが望ましいです。

 * ボタン
 * テキスト
 * 画像
 * テキストフィールド

==== ボタン

ボタンを表示するときには、次のようなXMLを書きます。

//list[button1-1][]{
<Button
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:text="Button" />
//}

#@# //image[button1-1][ボタン表示]{
#@# //}

デザインのカスタマイズを何もしていない、デフォルトの状態のこのボタンは、タップすると色が変わり、「タップされた」という状態をユーザーにフィードバックしています。

#@# //image[button1-1-pressed-4][Android 4.3までは青くなる]{
#@# //}
#@# //image[button1-1-pressed-kitkat][Android 4.4では少し色が薄くなる]{
#@# //}

@<list>[button1-1]の4行目の「android:text」という属性が、ボタン内に表示するテキストです。

@<list>[button1-1]の2行目の「android:layout_width」と3行目の「android:layout_height」は、ボタンだけではなくどの要素にも共通して必要な属性です。
それぞれの要素が、横方向（layout_width）または縦方向（layout_height）に対してどれぐらいの領域を占めるかという設定です。
これらには「wrap_content」と「match_parent」というふたつの値が存在します。

//table[][]{
値       説明
wrap_content    要素を占める領域は、その要素がもっているサイズのみにとどまる
match_parent    要素を占める領域は、縦または横の画面サイズいっぱいまで広がる
//}

言葉に書くとむずかしいですね。
実際にどうなるかを設定して確認してみましょう。

===== wrap_content

wrap_contentを設定した場合、要素を占める領域は、その要素がもっているサイズのみにとどまります。つまり、「Button」という文字が設定されているボタンであれば、「Button」という文字のまわりに一定の余白がとられ、それらをひっくるめたものが要素の領域となります。

#@# //image[button1-2][要素を幅としたボタン]{
#@# //}

確認のため、ボタン内に表示するテキストを変更してみます。

//list[button1-2][]{
<Button
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:text="登録して送信する" />
//}

ボタン全体の横幅が、テキストの文字数にあわせて伸びました。
このように、要素がもっているサイズにあわせたいときは、「wrap_content」を設定します。

wrap_contentを設定した要素をどんどん並べていくと、横へ横へと配置されていきます。

//list[button1-3][]{
<Button
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:text="キャンセル" />
//}
<Button
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:text="登録して送信する" />
//}

#@# #@# //image[button1-3][no title]{
#@# //}

画面サイズにおさまらなくなった場合は、改行されます。

#@# //image[button1-4][no title]{
#@# //}

もちろんボタンだけではなく、テキスト（TextView）や、このあとに出てくる画像（ImageView）などもいっしょに横に並べて配置することもできます。

===== match_parent

@<list>[button1-1]では、android:layout_widthもandroid:layout_heightも、両方とも「wrap_content」が設定されていました。
2行目のandroid:layout_widthを「match_parent」に変更してみます。

//list[button2-1][]{
<Button
        android:layout_width="match_parent"
        android:layout_height="wrap_content"
        android:text="Button" />
//}

#@# //image[button2-1][no title]{
#@# //}

横方向に、画面サイズいっぱいまで広げる設定をしましたので、ボタンは
#@# @<image>[button2]
のように横幅いっぱいまで広がります。

では今度は、@<list>[button2-2]のように、android:layout_widthを「wrap_content」に戻し、android:layout_heightを「match_parent」に設定してみましょう。

//list[button2-2]{
<Button
        android:layout_width="wrap_content"
        android:layout_height="match_parent"
        android:text="Button" />
//}

#@# //image[button2-2][no title]{
#@# //}

今度は縦方向に、画面サイズいっぱいまで広がりました。

実際のアプリケーションでは、ボタンをこのように縦方向の画面サイズいっぱいまで広げるようなレイアウトはほとんどありませんが、他の要素では必要になることもありますので、覚えておきましょう。


==== 画像

画像を表示するときには、次のようなXMLを書きます。

//list[img1-1][]{
<ImageView
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:src="@drawable/ic_launcher" />
//}

#@# //image[img1-1][drawable-xhdpiフォルダにあるic_launcher.pngを表示した]{
#@# //}

#@# //image[img1-2][resフォルダ内のdrawableフォルダ]{
#@# //}

@<list>[img1-1]の4行目の「android:src」は、resフォルダの「drawable-xxxxx」フォルダ内に入っている画像リソースのファイル名を設定する属性です。

画像の拡張子は、pngです。

ic_launcher.pngは、resフォルダの「drawable-mdpi」「drawable-hdpi」「drawable-xhdpi」という3つのフォルダの中に1つずつ入っています（
#@# @<image>[img1-3]
）。内容は同じですが、それぞれの画面解像度に合ったサイズのものが入っています。

#@# //image[img1-3][各drawableフォルダにはそれぞれの画面解像度にあった大きさの画像リソースが入っている]{
#@# //}

「android:src」の指定では、「@drawable」をつけて、<b>drawableというフォルダ内を参照</b>します。しかしここでは「@drawable-xhdpi/ic_launcher」のように画面解像度は書きません。

#@# //image[img1-4][参照フォルダに画面解像度は入れない]{
#@# //}

端末ごとにどの画面解像度なのかは一意に決まっていますので、「@drawable/ic_launcher」と書くだけで、Android側で自動で判別し、該当する画像リソースを取得します。


==== テキストフィールド

画像を表示するときには、次のようなXMLを書きます。

//list[textfield1-1][]{
<EditText
        android:layout_width="match_parent"
        android:layout_height="wrap_content" />
//}

これだけで、最低限の入力エリアを作成することができます。

#@# //image[textfield1-1][no title]{
#@# //}

@<list>[textfield1-1]では横幅いっぱいに配置していますが、@<list>[textfield1-2]のように「android:ems」を追加し、文字数によって横幅を指定することもできます。

//list[textfield1-2][]{
<EditText
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:ems="10" />
//}

この場合は10文字分の横幅になります。
また、「android:inputType」を追加し、いろいろな入力制限を設定することもできます。

たとえば、「android:inputType=”textPassword”」を追加すると、前節にも出てきたパスワード入力用のテキストフィールドになります。

//list[textfield1-3][]{
<EditText
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:ems="10"
        android:inputType=”textPassword” />
//}

#@# //image[textfield1-3][no title]{
#@# //}

「android:inputType」については、場合に応じて必要な入力制限を設定するようにしましょう。

=== valuesフォルダを活用しよう

==== valuesフォルダとは？
==== string
==== dimen
==== color
==== style


== レイアウトを作成する

=== RelativeLayout


=== LinearLayout


=== FrameLayout


== 実際にデザインを作るときにどうする？

=== キャンバスサイズ
=== テキストの大きさの目安
=== どういうふうにエンジニアさんに渡せばいいのか？
=== 画像パーツの切り出しと名前の付け方
=== 画像を切り出さずに渡す場合


==ボタンのカスタマイズと装飾


=== ボタンの背景
==== 画像リソースを使う場合

==== コードで書く場合
layer-listとかここ


=== 状態の変化を設定する
statefulとかここ