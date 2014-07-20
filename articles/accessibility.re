= ローカライズとアクセシビリティ

本章では、作成したAndroidアプリを日本語以外のユーザーに提供する方法と、目や耳が不自由なユーザー向けに行うべきことを紹介します。

== ローカライズ

「このアプリ、すごくいいんだけど英語なんだよな。。。」といった経験をしたことはありませんか？すばらしいユーザー体験を提供するアプリであっても、言語が母国語でないというだけでインストールされないというケースはとても多いです。本節では、作成したAndroidを多言語対応（ローカライズ）する方法を紹介します。

=== 文字列をローカライズする

アプリ内で使用している文字列を端末の言語設定に応じて変更するには、以下の順で文字列を置き換えていきます。

 * res/values/strings.xmlを作成する
 * レイアウトXML内の文字列を@string/xxxxにする
 * Javaプログラム内の文字列をgetString(R.string.xxxx)で置き換える
 * res/values-en/strings.xmlなどを作成する

では、順に見ていきましょう。

==== res/values/strings.xmlを作成する

まず、strings.xmlに文字列の対応表を作成します。name属性を用いて文字列に名前を付けていきます。この時、

 * 数字で始まる名前
 * Javaの予約語（たとえばswitchなど）

は使用することができないので注意しましょう。@<list>{stringxml}に、アプリ名とメッセージの対応を記述した例を示します。

//list[stringxml][strings.xmlの例] {
<resources>
    <string name="app_name">メモ帳アプリ</string>

    <string name="error_failed_to_connect">サーバーに接続できませんでした。</string>
    <string name="settings">設定</string>
</resources>
//}

==== レイアウトXML内の文字列を@string/xxxxにする

次に、レイアウトXML内の文字列（TextViewやButtonなど）を置き換えていきます。android:textやandroid:hintなど、文字列を指定する箇所を@string/xxxx形式で指定します。xxxxの部分は、先ほど作成したstrings.xmlのname属性に設定した名前を指定します。たとえば、@<list>{layout_string}では、TextViewに「メモ帳アプリ」が表示されます。

//list[layout_string][TextViewの文字列を置き換える]{
<TextView
    android:layout_width="wrap_content"
    android:layout_height="wrap_content"
    android:text="@string/app_name"
    />
//}

==== Javaプログラム内の文字列をgetString(R.string.xxxx)で置き換える

Toastの引数やダイアログのメッセージなど、Javaプログラム内でユーザーに表示するための文字列を指定するシーンはいくつかあります。端末の言語設定を反映させるため、Javaプログラム中では文字列を""で指定するのではなく、次のような方法で指定します。

 * int resIdを引数にとるメソッドの場合、R.string.xxxxを渡す
 * StringやCharSequenceを引数にとるメソッドの場合、getString(R.string.xxxx)の戻り値を渡す

TextViewのsetText()メソッドやAlertDialog.BuilderのsetMessage()メソッドなど、一部のメソッドはセットする文字列としてCharSequence(String)だけでなく、int型のresIdを指定することができます。この場合、引数としてR.string.xxxxを渡すことで多言語に対応させます。xxxxの部分はstrings.xmlのname属性に設定した名前にします。

セットする文字列としてStringやCharSequenceしか受け取れないメソッドや、文字列の一部をユーザー名などに置き換えて使用したい場合、ActivityやFragmentのgetString(int resId)メソッドを使用して、端末の言語設定に従った文字列を取得し、それを使用します。

==== res/values-en/strings.xmlなどを作成する

レイアウトXMLとJavaプログラムを修正した後、以下の手順で各言語用のstrings.xmlを用意します。

 * res/values-<言語コード>フォルダを作ります。たとえば英語であればres/values-enフォルダを作成します。
 * res/values/strings.xmlをres/values-<言語コード>フォルダにコピーします。
 * コピーしたstrings.xmlファイルの文字列をその言語に翻訳します。

これにより、端末の言語設定に応じて使用されるstrings.xmlファイルが変更されます。尚、res/valuesフォルダ内のstrings.xmlはデフォルトリソースと呼ばれ、現在の言語用のstrings.xmlファイルが見つからない時に使用されます。
 
=== ローカライズした画像を用意する

アプリのタイトル画面用ロゴなど、文字列を画像で用意することもあるでしょう。この場合、ローカライズした画像ファイルを用意し、res/drawable-<言語コード>-<ピクセル密度>フォルダに同じファイル名で入れます。

=== デフォルトの言語を英語にする

res/valuesフォルダや、res/drawable-hdpiフォルダなど、言語コードを含まないフォルダ内のリソースは「デフォルトリソース」と呼ばれます。これらは、端末の言語設定にマッチするフォルダが無い時に使用されます。つまり、res/values/strings.xmlに日本語のリソースをいれた場合、サポートしていない言語のユーザーには日本語が表示されてしまい、すぐアンインストールされてしまうことにつながります。世界的に見て、英語であれば多少は読めるというユーザーがいると思われるので、res/valuesには英語リソースを入れ、res/values-jaに日本語リソースを入れておきましょう。これにより、サポートしていない言語の場合でも英語で表示されるため、作成したアプリを使ってもらえる可能性が高まります。

== アクセシビリティ

本節では、作成したAndroidアプリを、目や耳が不自由な方に使ってもらうためにやるべきことを紹介します。

=== アクセシビリティ機能を試す

まずは多くの端末で最初からインストールされているTalkBackを試してみましょう。「設定」の「ユーザー補助」から、TalkBackをONにします。これにより、読み上げ機能が有効になります。

=== 読み上げ機能に対応する

早速、作成したAndroidアプリを読み上げ機能に対応させてみましょう。

==== 読み上げ用文字列を設定する

ViewやViewGroupにフォーカスがあたった時、TalkBackはandroid:textまたはandroid:contentDescriptionの内容を読み上げます。TextViewやButtonであれば、android:textが設定されているので読み上げが行われますが、ImageViewなど、android:textを持たないViewやViewGroupは、android:contentDescriptionが設定されていないと、読み上げが行われません。これでは目の不自由な方が画像をタップした時、どのような画像をタップしたか分からないので、android:contentDescriptionに読み上げ用の文字列を設定しましょう。読み上げ用の文字列もローカライズのことを考え、@string/xxxx形式で指定するようにします。

==== 左右フリックに対応する

TalkBackがONの時は、左右フリックでフォーカスが移動します。この時の順序はViewのツリー構造に依存します。@<list>{treeorder}で、@+id/button1にフォーカスがあたっている状態で右フリック（順送り）を行うと、@+id/button2にフォーカスが移動します。

//list[treeorder][レイアウトXML]{
//}

==== フォーカスの制御を行う

レイアウトを重ねて表示している時、後ろのViewにフォーカスがあたってしまうとユーザーは混乱してしまいます。これを防止するため、Viewにフォーカスを当てない設定を行うことができます。@<list>{focus_xml}はレイアウトXMLで@+id/xxxxにフォーカスを当てないようにする例です。

//list[focus_xml][レイアウトXMLでフォーカスを当てないよう設定]{
//}

Javaプログラム中で動的に指定する時は、@<list>{focus_java}のようにsetImportantForAccessibility()メソッドを使用します。

//list[focus_java][Javaプログラムでフォーカスを当てないよう設定]{
//}

==== 方向キーに対応する

日本ではあまり見かけませんが、方向キーのある端末もあります。ユーザーは画面を見ることができなくても、Viewの並びを想像することはできるため、方向キーによる操作と想像しているViewの並びを合わせておく必要があります。これに対応するため、フォーカスが当たっている状態で上下左右が押された時、どのViewにフォーカスが移動すべきか指定することができます。@<table>{focus_order}は制御用の属性一覧です。たとえば、上キーが押された時@+id/button1にフォーカスを移動させた場合、android:nextFocusUpに@+id/button1を指定します。

//table[focus_order][フォーカスの制御用属性]{
属性名	意味
-----------
android:nextFocusDown	下キーが押された時にフォーカスの当たるViewを指定
android:nextFocusLeft	左キーが押された時にフォーカスの当たるViewを指定
android:nextFocusRight	右キーが押された時にフォーカスの当たるViewを指定
android:nextFocusUp	上キーが押された時にフォーカスの当たるViewを指定
//}

@<list>{focus_cursor}は上下左右で適切なViewにフォーカスが移動するよう設定した例です。

//list[focus_cursor][方向キーのある端末向けに設定をいれた例]{
//}

=== 動画に字幕を付ける

作成したAndroidアプリ内で、説明のためにVideoViewを用いて動画再生を行うこともあるでしょう。しかし、耳の不自由な方は台詞を聞くことができないため、動画に字幕をつける必要があります。Androidでは、VideoViewに再生中の動画に字幕を付けるAPIがAndroid 4.4（API Level 19）で追加されました。以下の手順で、VideoViewで再生する動画に字幕を付けることができます。

 * WebVTT形式の字幕ファイルを作成する。
 * VideoViewに字幕ファイルをセットする。

==== WebVTT形式の字幕ファイルを作成する

WebVTTとは、The Web Video Text Tracks Formatの略で、HTML5のvideoタグで表示される動画に字幕を付ける時などに使用されます。詳細なドラフトはhttp://dev.w3.org/html5/webvtt/で読むことができます。

WebVTTファイルの形式を@<list>[webvtt]に示します。

//list[webvtt][WebVTTフォーマット]{
WEBVTT
Kind:captions
Language:ja

1
00:00:06.500 -> 00:00:08.200
ようこそ

2
00:00:09.000 -> 00:00:11.300
OpenTextbookへ

//}

この形式のファイルを、res/rawフォルダにいれます。ここではcaption.vttとして保存したとして話を進めます。

==== VideoViewに字幕ファイルをセットする

次に、作成したvttファイルをVideoViewにセットします。VideoViewのインスタンスを取得し、addSubtitleSource()メソッドで字幕データをセットします。@<list>[videoview]に例を示します。

//list[videoview][VideoViewに字幕データをセットする]{
mVideoView.addSubtitleSource(
    getResources().openRawResource(R.raw.caption),
    MediaFormat.createSubtitleFormat(
        "text/vtt",locale.JAPANESE.getLanguage()));
//}

=== テストする

作成したアプリがアクセシビリティの機能を正しくサポートしているかテストしましょう。アクセシビリティテストのゴールは以下の3点です。

 * 作成したアプリを、視覚情報なしで利用できること
 * アプリ内の作業を方向操作のみで容易に行うことができること
 * その際のフィードバック（読み上げ）は正しいこと

次の6点を特に検証しましょう。

 * 画面のタップを用いず、方向キーのみで操作できるか？
 * TalkbackがONの時、フォーカスが当たったものが正しく読み上げられるか？
 * タッチガイドがONの時、タップしたものが正しく読み上げられるか？
 * ユーザーが操作可能なViewは、Android Designで推奨されている1辺48dp以上であるか？
 * TalkbackがONの時、画像の拡大縮小やスクロールなどのジェスチャーが正しく動作するか？
 * 音のみのフィードバックを行っていないか？メール着信を音のみで通知していた場合、耳の不自由な方は通知に気づくことができません。

== まとめ

本章では、作成したアプリをより多くのユーザーに使用してもらうため、ローカライズの手法とアクセシビリティ機能に対応する手法を紹介しました。日本語を読むことができないユーザーや、画面を見ることができないユーザー向けの改善を行うことで、より多くのユーザーによりよいユーザー体験を提供しましょう。
