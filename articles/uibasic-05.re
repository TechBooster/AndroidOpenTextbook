= valuesフォルダを活用しよう

Androidプロジェクトのレイアウトを構成していく「res」フォルダの中に、「values」
というフォルダがあります。


//image[valuesfolder][valuesフォルダ]{
//}

valuesフォルダに入っているXMLは、@<b>{さまざまな値を一括して管理するデータシートのような役割}をします。
たとえば、アプリ内で使う色を例に挙げてみましょう。やみくもにいろいろなものを使うわけではなく、最低でも

* メインカラー
* サブカラー
* アクセントカラー
* ベースとなるバックグラウンドカラー

の4つはあらかじめ決めて設計をしていくことが望ましいです。
しかし、アプリを作っていくと、いろんなところでこれらの色を使っていくものです。
TextViewでメインカラーを使うところ、背景でバックグラウンドカラーを使うところ、また他の画面でもメインカラーを使ったり…。レイアウトファイルで、これらの色の指定をする部分がかなりの数で出てきます@<fn>{layout-color}。

//footnote[layout-color][レイアウトファイルだけでなく、Javaファイルでも色の指定をするときは、さらに指定箇所は増えます。]


//image[about-color-normal][]{
//}

@<img>{about-color-normal}の例では、TextViewで同じ色（#c00：濃い赤色）を指定しているところが3ヶ所あります。
#c00をメインカラーとしたとき、この3つのテキストは常に同じ色であることを求められます。しかし、いろいろな事情で、「やっぱりメインカラーをちょっと変えよう」ということも、アプリを開発している途中でよく起こることです。

「ちょっと変えよう」というときに、@<img>{about-color-normal}の例では、3ヶ所すべてのカラーコードを変更しなければなりません。
3ヶ所だけならまだしも、実際には他の画面にもたくさんメインカラーは使われています。また、プログラムのJavaでもこのカラーコードが書かれているとしたら、すべての箇所を変更しなければなりません。
これは、大変膨大で、またミスを引き起こしやすい状態となってしまいます。


//image[about-color-ng][]{
//}

こういったときに、valuesフォルダのXMLを使って、「メインカラーは#c00である」ということを定義しておくと、「ちょっと変えよう」ということが起こっても、定義している部分の1ヶ所だけを修正することで、すべてが反映されます。


//image[about-color-ok][]{
//}

言い換えると、valuesフォルダのXMLを使ってさまざまな値を一括して管理することは、メンテナンス性の向上にもつながるのです。

== valuesフォルダ内で管理できるもの

valuesフォルダ内で管理できるものは、ガイドラインの「More Resource Types」を参照してください@<fn>{values-resoueces}。

//footnote[values-resoueces][http://developer.android.com/guide/topics/resources/more-resources.html]

その中でも、よく使うものは次のとおりです。

//table[][XMLファイルとその役割]{
strings.xml	アプリ内の文字列
colors.xml	アプリ内の色
styles.xml	レイアウトの属性をまとめる
dimens.xml	画像のサイズや文字のサイズ、余白のサイズなど、dpやspなどの単位で設定する数値
//}

何の設定をするかによって、ファイルを分けます。
それぞれについて、詳しくみていきましょう。

=== strings.xml

strings.xmlは、文字列を管理するファイルです。
新規Androidプロジェクトを作成すると、このファイルはあらかじめ存在し、すでに次のように定義されています。

//listnum[][values/string.xml]{
<?xml version="1.0" encoding="utf-8"?>
<resources>
    <string name="app_name">AndroidXMLBasic</string>
    <string name="hello_world">Hello world!</string>
    <string name="action_settings">Settings</string>
</resources>
//}

この文字列は、実際のレイアウトと照らし合わせると、次の部分の文字列になります。


//image[strings-default][]{
//}

この中で、レイアウトXMLから参照されているものは「Hello world!」のみで、他のものは、AndroidManifest.xmlやJavaから参照されています。

レイアウトXMLから参照する場合、次のように書きます。
//listnum[][fragment_main.xml]{
<TextView
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:text="@string/hello_world" />
//}

「android:text」の部分で、参照するファイルの名前（ここでは「string」、単数形にする）の前に「@」をつけ、そのあとに「/（スラッシュ）」と、設定したnameを入れます。

==== 他言語化対応をする

文字列をvaluesフォルダで管理することのメリットは、他言語化対応です。
他言語化対応のことを、@<b>{ローカライズ}といいます。

Androidアプリをひとたびリリースすれば、それは全世界で公開されます。
より多くの人に使ってもらおうとするなら、日本語だけではなく、最低でも英語対応は必須でしょう。

とはいえ、日本語版は日本語用のレイアウト、英語版は英語用のレイアウト、などのように何通りも同じようなレイアウトXMLを作るのは、骨の折れる作業です。
また、少しの変更があった場合に、すべてのレイアウトXMLを更新するのも現実的ではありません。

Androidアプリは、ローカライズしやすいように構成されています。
新規に立ち上げたサンプルアプリで、英語版にも対応するように作り変えてみましょう。

 1. 「values」フォルダと同じ階層に「values-ja」というフォルダを作成する（@<image>{strings-folderを参照}）
 2. 「values-ja」フォルダ内に、「values」フォルダ内の「strings.xml」をコピーする（@<image>{strings-folderを参照}）
 3. 新しくコピーしてきた「values-ja」フォルダ内の「strings.xml」の文字列を@<list>{values-ja}のように日本語にする

//listnum[values-ja][values-ja/string.xml]{
<?xml version="1.0" encoding="utf-8"?>
<resources>
    <string name="app_name">アンドロイドXML基礎</string>
    <string name="hello_world">こんにちは世界！</string>
    <string name="action_settings">設定</string>
</resources>
//}


//image[strings-folder][]{
//}

完成したら、エミュレータまたは実機で確認をしてみましょう。

アプリを立ち上げてみて、「values-ja」で定義した日本語の文字列になっていれば、成功です。


//image[strings-localize-ja][文字列が日本語に変わった]{
//}

どの言語を参照しているかは、使っているAndroid端末の言語設定@<fn>{settings-language}に依存します。

//footnote[settings-language][設定 -> 言語と入力 から設定できます]

日本語の設定にしている場合は、「values-ja」フォルダ内のstrings.xmlが参照されます。
他にたとえば、フランス語の設定にしている場合は、「values-fr」フォルダ内のstrings.xmlが参照されます。
しかし、今回のように「values-fr」というフォルダがない場合は、デフォルト（「values」フォルダ）のstrings.xmlを参照します。

「values-xx」の「xx」にあたるものは、ISO 639に定められている言語コードです。
言語によっては、複数の地域で使われているものもありますが、それらは「values-xx_XX」となり、ISO 3166に定められているリージョンコードとなります。@<fn>{values-iso}

//footnote[values-iso][http://www.localeplanet.com/icu/ を参照]

Androidアプリのローカライズでよく使うものを挙げておきます。

//table[][ローカライズでよく使うもの]{
言語	言語コード
---------------------------------------------
日本語	values-ja
英語	values-en
フランス語	values-fr
スペイン語	values-es
中国語	values-zh_CN
台湾語	values-zh_TW
韓国語	values-ko
//}

@<img>{strings-localize-ja}のように日本語が表示されたら、今度はAndroid端末の言語設定を英語（English(United States)）にして、再度アプリを確認してみましょう。
アプリ内の文字列が英語で表示されていたら、うまくローカライズができていることになります。

このように、strings.xmlを使えば、レイアウトXMLやJavaを編集することなく、効率よくローカライズができることがわかります。

また、ローカライズをしない場合でも、文字列はstrings.xmlファイルで管理することが推奨されていますので、できるだけそうするようにしましょう。

=== colors.xml

colors.xmlは、色を管理するファイルです。
このファイルは、新規Androidプロジェクトを作成したときにはありませんので、必要なときには自分で作ります。

先の例でも挙げましたが、配色を設計するときに指定した色を書いておくとよいでしょう。

まず、色を管理するサンプルXMLを作成します。ここで作成するサンプルは、@<image>{colors-sample}のような簡単なリストです。

//image[colors-sample][]{
//}

このサンプルのキーカラーは赤色で、見出しとなるビューに適用されています。複数箇所に使われていますが、colors.xmlで一括管理されていますので、レイアウトXMLを修正することなく、簡単に色の変更ができるサンプルになっています。

また、少しデザイン的な要素も含んでいますので、デザインの勉強もあわせてしていきましょう。

==== 1. ビューを作成し、文字列を入れる

まず、LinearLayoutを作成し、中の要素が縦向きに並ぶように「android:orientation="vertical"」を指定します。その中にテキスト（TextView）を合計8つ（それぞれがタイトルと本文を持ち、4x2となるように）作成します。

//listnum[color-xml-normal][fragment_main.xml]{
<LinearLayout xmlns:android="http://schemas.android.com/apk/res/android"
    xmlns:tools="http://schemas.android.com/tools"
    android:layout_width="match_parent"
    android:layout_height="match_parent"
    android:orientation="vertical">
    <TextView
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:text="@string/animal" />
    <TextView
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:text="@string/animal_text" />
    <TextView
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:text="@string/pet" />
    <TextView
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:text="@string/pet_text" />
    <TextView
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:text="@string/plant" />
    <TextView
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:text="@string/plant_text" />
    <TextView
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:text="@string/meat" />
    <TextView
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:text="@string/meat_text" />
</LinearLayout>
//}

//listnum[color-string][values/strings.xml]{
<?xml version="1.0" encoding="utf-8"?>
<resources>
    <string name="app_name">XML基礎 - 色の管理</string>
    <string name="animal">動物園にいる動物の種類</string>
    <string name="animal_text">ライオン\nぞう\nシマウマ\n馬\nキリンなど</string>
    <string name="pet">ペットとして飼える動物の種類</string>
    <string name="pet_text">犬\n猫\nフェレット\n鳥\nトカゲ\nなど</string>
    <string name="plant">草食動物の種類</string>
    <string name="plant_text">馬\nシカ\nぞう\nうさぎ\nイグアナ\nなど</string>
    <string name="meat">肉食動物の種類</string>
    <string name="meat_text">ライオン\nアライグマ\nクマ\nピューマ\nミーアキャット\nなど</string>
</resources>	
//}

@<list>{color-string}内の「\n」という記号は、改行を意味するもので、「¥n」とも書きます（HTMLだと「<br>」に該当するものです）。

@<list>{color-xml-normal}と@<list>{color-string}により、@<img>{color-normal}のような状態を作成します。
しかし、これだけでは見出しと本文の違いがまったくわからず、見やすい状態ではありませんので少しデザイン要素を追加していきます@<fn>{design}。

//footnote[design][「デザイン」と聞くと、絵を描くことを思い浮かべる人がいるかもしれませんが、それは少し偏った考え方です。「デザイン」には本来「設計」という意味が含まれており、絵やイラストなどを入れなくとも文字の大きさや余白を調整することで、デザインとなりうるのです。]


//image[color-normal][とりあえず必要な要素を出した]{
//}

==== 2. 見出しの文字を少し大きくする

見出しと本文の違いをデザインによって表現するため、見出しの文字を大きくしてみましょう。
見出しとなるTextView（@<list>{color-xml-normal}の6〜9行目、14〜17行目、22〜25行目、30〜33行目の4ヶ所）に、次の2行を追加します。

//list[][見出しとなるTextViewに追加する]{
android:textSize="16sp"
android:textStyle="bold"
//}

テキストの大きさが16spになり、太字になりました。
このように、見出しの文字サイズを大きくしたり、太字にしたりすることはよくありますので覚えておきましょう。


//image[color-bold][]{
//}

==== 3. 見出しの文字を装飾する

次に、見出しの文字をもう少し目立たせるように装飾していきましょう。
見出しの背景をキーカラー（少し濃い色がいいでしょう）にし、文字を白色にしてみましょう。

先ほどと同じく、見出しとなるTextView（4ヶ所）に次の2行を追加します。

//listnum[][]{
android:textColor="#fff"
android:background="#b81c22"
//}

この時点で、見出しのTextViewは@<list>{color-keycolor}のようになっています。

//listnum[][]{
<TextView
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:textSize="16sp"
        android:textStyle="bold"
        android:textColor="#fff"
        android:background="#b81c22"
        android:text="@string/animal" />
//}

「android:textColor」は文字の色を決める属性、「android:background」はその要素を占める領域の背景色を決める属性です。それぞれ#fff（白）と#b81c22（濃い赤：キーカラー）に設定しました。

キーカラーは自分の好きな色に変えてもかまいません。


//image[color-keycolor][]{
//}

==== 4. 余白を調整する

さて、ここまで装飾をしてきて、すっかりデザインをした気になっていませんか？
よくある話なのですが、プログラマは、ここまでの見栄えができたら、アプリを公開してしまいます。しかし、この状態では「デザインされている」とはまったくもって言えません。
情報としてはまちがっていないですし、問題ないのですが、読みやすさに関してまったく配慮がなされていないのです。

読みやすさを左右する重要な要素に、@<b>{余白}があります。
AndroidアプリのレイアウトXMLでは、余白を自動的にとってくれるようなシステムは残念ながらありませんので、開発者ひとりひとりが気をつけながら作っていかなければなりません。
開発に関するプログラムを学ぶことは必要最低限なことであり、その後良いアプリが作れるかどうかは、デザイン力にもかかってきます。
プログラムといっしょに、あわせてデザインに関する知識もつけていくようにしましょう。

@<img>{color-keycolor}において何がよくないのかというと、基本的に要素と要素がくっついているのが不自然なのです。


//image[color-margin][]{
//}

よっぽどのことがない限り（特にテキストの場合）、要素と要素がぴったりとくっつくことはありません。

@<img>{color-margin}の指摘箇所に余白を追加していきます。

(A)については、全体的なViewに余白がないため、親となるView（ここではLinearLayout）に余白を追加します。

//listnum[][5行目の「android:layout_margin="10dp"」を追加した]{
<LinearLayout xmlns:android="http://schemas.android.com/apk/res/android"
    xmlns:tools="http://schemas.android.com/tools"
    android:layout_width="match_parent"
    android:layout_height="match_parent"
    android:layout_margin="10dp"
    android:orientation="vertical">

　〜中略〜

</LinearLayout>
//}

(B)については、見出しとなるTextViewの内側に余白を追加します。

//listnum[][8行目に「android:padding="10dp"」を追加した]{
<TextView
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:textSize="16sp"
        android:textStyle="bold"
        android:textColor="#fff"
        android:background="#b81c22"
        android:padding="10dp"
        android:text="@string/animal" />
//}

最後に(C)については、見出しと本文の両方のTextViewの下側に余白を追加します。

//listnum[color-marginpaddingok][9行目と14行目にそれぞれ「android:layout_marginBottom」を追加した]{
<TextView
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:textSize="16sp"
        android:textStyle="bold"
        android:textColor="#fff"
        android:background="#b81c22"
        android:padding="10dp"
        android:layout_marginBottom="5dp"
        android:text="@string/animal" />
    <TextView
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:layout_marginBottom="10dp"
        android:text="@string/animal_text" />
//}

最終的には、@<img>{color-marginok}のようなデザインになります@<fn>{color-marginok-noscroll}。

//footnote[color-marginok-noscroll][この状態では、コンテンツが長くなってもスクロールがまだできない状態です]


//image[color-marginok][]{
//}

ここで、余白を追加するのにそれぞれ違う種類の属性を使いました。
(A) android:layout_margin
(B) android:padding
(C) android:layout_marginBottom

(C)については、android:layout_marginの下側に限った属性です。すでに想像できると思いますが、同じように

 * android:layout_marginTop
 * android:layout_marginBottom
 * android:layout_marginLeft
 * android:layout_marginRight

があり、基本的な考え方は「android:layout_margin」と同じものです。

それでは、marginとpaddingの違いは何でしょうか。
同じ余白ではありますが、marginは外側の余白、paddingは内側の余白と解釈されます。


//image[color-marginpadding][]{
//}

背景色がついていないものについては、marginにしてもpaddingにしても結果は変わらない場合があります。
しかし、背景色があるものについては、背景色の部分が領域の境界となりますので、marginとpaddingは正しく使い分けるようにしましょう。

==== 5. 応用

今回作成したサンプルはコンテンツの量が多く、しかもスクロールができないために下のほうが見切れてしまいました。
全体をスクロールをさせるために、いちばん大きな親として、ScrollViewを追加し、すでにあるLinearLayoutを少し修正します。

//listnum[][ScrollViewを追加し、LinearLayout「android:layout_height」を「wrap_content」に変更した]{
<ScrollView xmlns:android="http://schemas.android.com/apk/res/android"
    xmlns:tools="http://schemas.android.com/tools"
    android:layout_width="match_parent"
    android:layout_height="match_parent">
<LinearLayout
    android:layout_width="match_parent"
    android:layout_height="wrap_content"
    android:layout_margin="10dp"
    android:orientation="vertical">

　〜中略〜

</LinearLayout>
</ScrollView>
//}

この結果、全体が縦方向にスクロールするようになりました。
@<img>{color-scroll}の右端の細い縦棒のようなものは、スクロールバーです。スクロールバーは、スクロールしているときのみ出現します。


//image[color-scroll][]{
//}

==== 6. colors.xmlに色を定義する

ここまでがサンプルファイルの作成でした。ここからcolors.xmlを作成し、使っていきます。

「values」フォルダ内に「colors.xml」を作成し、@<list>{color-colorsxml}のように書きましょう。


//image[color-addxml][valuesフォルダにcolors.xmlを作成した]{
//}

//listnum[color-colorsxml][values/colors.xml]{
<?xml version="1.0" encoding="utf-8"?>
<resources>
    <color name="keycolor">#b81c22</color>
    <color name="keycolor_text">#fff</color>
</resources>
//}

keycolorという名前で「#b81c22」という色を定義しました。また、keycolor_textという名前で「#fff（白）」を定義しました。
レイアウトXMLもこれにあわせて変更します。

@<list>{color-marginpaddingok}の見出し部分、「android:background」と「android:textColor」の指定を@<list>{color-colorxmluse}のように変更します（4ヶ所）。

//listnum[color-colorxmluse][colors.xmlのkeycodeという名前の色を指定]{
android:background="@color/keycolor"
android:textColor="@color/keycolor_text"
//}

これにより、4ヶ所あった見出し部分の背景色（キーカラー）と文字色を、colors.xmlに指定して一元管理できるようになりました。
colors.xmlを使うメリットも、strings.xmlを使うときと同じで、@<b>{レイアウトXMLやJavaを編集することなく、効率よく修正ができる}ことにあります。
修正が発生した際に、数十ヶ所の値を直さなければならないのと、1ヶ所のみの修正でよいことを比べると、圧倒的に後者のほうが効率がよいでしょう。
今のうちからそういったことに気を付けておくと、複雑になったときでもミスなく対応することができるようになります。

=== styles.xml

styles.xmlは、ビューの属性を一括管理できるファイルです。
colors.xmlで作成したサンプルファイルをそのまま使ってみていきましょう。

今、レイアウトXMLの見出しと本文は、見出しの装飾も含めて@<list>{style-normal}のようなソースコードになっています。

//list[style-normal][fragment_main.xmlの一部]{
<TextView
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:textSize="16sp"
        android:textStyle="bold"
        android:textColor="@color/keycolor_text"
        android:background="@color/keycolor"
        android:padding="10dp"
        android:layout_marginBottom="5dp"
        android:text="@string/animal" />
    <TextView
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:layout_marginBottom="10dp"
        android:text="@string/animal_text" />
//}

しかもこれはほんの一部であり、同じようなものが4回繰り返されるのです。
色についてはcolors.xmlを使って一元管理しましたが、その他の部分に少しでも修正が入ったら…4ヶ所について同じように修正をしていかなければならないのです。

こういった同じ装飾を何度も繰り返す必要がある場合には、styles.xmlが便利です。
styles.xmlには、すでにいくつかの指定が書いてありますが、それはそのままにしておいて、追記してきます。
まずはstyles.xmlに<style name="head">〜</style>を追記します（@<list>{style-head}）。名前は自分で決めることができます。

//listnum[style-head][values/styles.xml]{
<resoueces>
　〜中略〜
    <style name="head">
    </style>
</resoueces>
//}

それができたら、fragment_main.xmlに@<list>{style-headxml}を追記します。

//list[style-headxml][fragment_main.xml]{
<TextView
    〜中略〜
    style="@style/head" />
//}

styles.xmlで作成した名前を、レイアウトXMLで参照します。書き方はstrings.xmlやcolors.xmlのときと同じです。
ここまでで準備完了です。次は、指定してある属性を、ひとつずつそのままstyles.xmlにうつしていきますが、書き方が少し異なりますのでまちがえないように注意してください。

//list[style-basexml][fragment_main.xmlに書かれているコード]{
android:layout_width="wrap_content"
//}

//list[style-basestyle][styles.xmlに書くコード]{
<item name="android:layout_width">wrap_content</item>
//}

レイアウトXMLでは@<list>{style-basexml}のように書かれているコードは、styles.xmlに書くときは@<list>{style-basestyle}のようになります。
他の属性についても、共通するものはすべてまとめてしまいましょう。
この結果、@<list>{style-endxml}と@<list>{style-endstyle}のようになります。

//list[style-endxml][fragment_main.xml]{
<TextView
        style="@style/head"
        android:text="@string/animal" />
//}

//list[style-endstyle][]{
<resoueces>
　〜中略〜
    <style name="head">
        <item name="android:layout_width">wrap_content</item>
        <item name="android:layout_height">wrap_content</item>
        <item name="android:textSize">16sp</item>
        <item name="android:textStyle">bold</item>
        <item name="android:textColor">@color/keycolor_text</item>
        <item name="android:background">@color/keycolor</item>
        <item name="android:padding">10dp</item>
        <item name="android:layout_marginBottom">5dp</item>
    </style>
</resoueces>
//}

同じく、本文についてもまとめられる属性はまとめておくと、ゆくゆく修正が発生したときに対応しやすくなります。

=== dimens.xml

dimens.xmlは、主に数値を管理するファイルです。
新規Androidプロジェクトを作ったときに、RelativeLayoutのpaddingの値がこれで定義されています。

//listnum[values-relative-dimen][5〜8行目にdimens.xmlを参照する記述がある]{
<RelativeLayout xmlns:android="http://schemas.android.com/apk/res/android"
    xmlns:tools="http://schemas.android.com/tools"
    android:layout_width="match_parent"
    android:layout_height="match_parent"
    android:paddingBottom="@dimen/activity_vertical_margin"
    android:paddingLeft="@dimen/activity_horizontal_margin"
    android:paddingRight="@dimen/activity_horizontal_margin"
    android:paddingTop="@dimen/activity_vertical_margin"
    tools:context="com.example.androidxmlbasic.MainActivity$PlaceholderFragment" >
　〜中略〜
　</RelativeLayout>
//}

dimens.xmlには@<list>{values-dimens}のコードが書かれています。

//listnum[values-dimens][values/dimens.xml]{
<resources>
    <!-- Default screen margins, per the Android Design guidelines. -->
    <dimen name="activity_horizontal_margin">16dp</dimen>
    <dimen name="activity_vertical_margin">16dp</dimen>
</resources>
//}

@<list>{values-relative-dimen}と照らしあわせてみると、上下のpaddingには「@dimen/activity_vertical_margin」が、左右のpaddingには「@dimen/activity_horizontal_margin」が指定されていることがわかります。paddingの値を変えたいときは、dimens.xmlフォルダ内の数値を変えるだけでよいので、ここでも一元管理によって効率の良さが考えられています。