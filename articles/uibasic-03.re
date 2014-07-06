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
strings.xml     アプリ内の文字列
colors.xml      アプリ内の色
dimens.xml      画像のサイズや文字のサイズ、余白のサイズなど、dpやspなどの単位で設定する数値
styles.xml      レイアウトの属性をまとめる
//}

何の設定をするかによって、ファイルを分けます。
それぞれについて、詳しくみていきましょう。

=== strings.xml

strings.xmlは、文字列を管理するファイルです。
新規Androidプロジェクトを作成すると、このファイルはあらかじめ存在し、すでに次のように定義されています。

//listnum[][]{
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

「android:text」の部分で、参照するファイルの名前（単数形）の前に「@」をつけ、そのあとに「/（スラッシュ）」と、設定したnameを入れます。

==== 他言語化対応をする

文字列をvaluesフォルダで管理することのメリットは、他言語化対応です。
他言語化対応のことを、@<b>{ローカライズ}といいます。

Androidアプリをひとたびリリースすれば、それは全世界で公開されます。
より多くの人に使ってもらおうとするなら、日本語だけではなく、最低でも英語対応は必須でしょう。

とはいえ、日本語版は日本語用のレイアウト、英語版は英語用のレイアウト、などのように何通りも同じようなレイアウトXMLを作るのは、骨の折れる作業です。
また、少しの変更があった場合に、すべてのレイアウトXMLを更新するのも現実的ではありません。

Androidアプリは、ローカライズしやすいように構成されています。
新規に立ち上げたサンプルアプリで、英語版にも対応するように作り変えてみましょう。

 1. 「values」フォルダと同じ階層に「values-ja」というフォルダを作成する
 2. 「values-ja」フォルダ内に、「values」フォルダ内の「strings.xml」をコピーする
 3. 新しくコピーしてきた「values-ja」フォルダ内の「strings.xml」の文字列を日本語にする

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
言語によっては、複数の地域で使われているものもあり、それらは「values-xx_XX」となり、ISO 3166に定められているリージョンコードとなります。@<fn>{values-iso}

//footnote[values-iso][http://www.localeplanet.com/icu/ を参照]

Androidアプリのローカライズでよく使うものを挙げておきます。

//table[][ローカライズでよく使うもの]{
言語      言語コード
---------------------------------------------
日本語     values-ja
英語      values-en
フランス語   values-fr
スペイン語   values-es
中国語     values-zh_CN
台湾      values-zh_TW
韓国語     values-ko
//}

@<img>{strings-localize-ja}のように日本語が表示されたら、今度はAndroid端末の言語設定を英語（English(United States)）にして、再度アプリを確認してみましょう。
アプリ内の文字列が英語で表示されていたら、うまくローカライズができていることになります。

このように、strings.xmlを使えば、レイアウトXMLやJavaを編集することなく、効率よくローカライズができることがわかります。

=== colors.xml



=== dimens.xml


=== styles.xml


