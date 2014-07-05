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
 * Javaの予約語（例えばswitchなど）

は使用することができないので注意しましょう。@<list>{stringxml}に、アプリ名とメッセージの対応を記述した例を示します。

//list[stringxml][strings.xmlの例] {
<resources>
    <string name="app_name">メモ帳アプリ</string>

    <string name="error_failed_to_connect">サーバーに接続できませんでした。</string>
    <string name="settings">設定</string>
</resources>
//}

==== レイアウトXML内の文字列を@string/xxxxにする

次に、レイアウトXML内の文字列（TextViewやButtonなど）を置き換えていきます。android:textやandroid:hintなど、文字列を指定する箇所を@string/xxxx形式で指定します。xxxxの部分は、先ほど作成したstrings.xmlのname属性に設定した名前を指定します。例えば、@<list>{layout_string}では、TextViewに「メモ帳アプリ」が表示されます。

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
 
=== 画像をローカライズする

=== デフォルトの言語を英語にする

== アクセシビリティ

=== android:contentDescriptionを付ける

=== 方向キーに対応する

=== テストする

== まとめ
