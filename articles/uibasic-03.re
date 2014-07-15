= XMLを使ってパーツを作る

Graphical Layoutではドラッグアンドドロップでだれでもレイアウトを作ることができます。しかし、細かいレイアウトの設定は、XMLを使って作れるようにならなければなりません。

この節では、XMLを使ってよく使うパーツを作れるようになりましょう。

== XMLレイアウトがどんなものかを知る

まず、XMLレイアウトがどんなものかを知るために、あらかじめ配置されているXMLをカスタマイズしていきます。

新規Androidプロジェクトを作成します。
そして、立ち上がった「fragment_main.xml」を開きます。Graphical Layoutを見ると、「Hello world!」が表示されています。


//image[201][Graphical Layoutを見る]{
//}

下のタブを「fragment_main.xml」に切り替えてみましょう。

ここに書かれているのが、レイアウトを構成するXMLです。


//image[202][レイアウトを構成するXMLが書かれている]{
//}

テキストが表示されている部分のXMLは、次のようになっています。

//listnum[201][テキストが表示されている部分のXML]{
<TextView
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:text="@string/hello_world" />
//}

=== テキストの文字列を変更する

4行目の「android:text=””」の中で、表示する文字列を指定しています。ここでは、@stringといって他のファイルを参照するように指定されていますが、いったん無視して、好きなテキストに変えてみます。

//listnum[202][4行目を追加した]{
<TextView
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:text="XMLレイアウトの基礎" />
//}

確認方法は、AVDでも実機でもどちらでもかまいません。簡易的な確認であれば、Graphical Layoutでもできます@<fn>[easy]。
//footnote[easy][レイアウトが複雑になってきたり、プログラミングによってレイアウトを表示させたりするものは、Graphical Layoutでは確認できません。そのときはAVDや実機を使って確認するようにしましょう。]


//image[203][出力結果。Graphical Layoutで確認した場合]{
//}

先ほど「Hello world!」だったテキストが、入力したものに変わっていることが確認できます。

=== テキストの色や大きさを変更する

では次に、テキストの色や大きさを変更してみましょう。
先ほどの@<list>{202}に、次の2行を追加します。

//listnum[203][5〜6行目を追加した]{
<TextView
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:text="XMLレイアウトの基礎"
        android:textColor="#ff0000"
        android:textSize="22sp" />
//}

「android:textColor」がテキストの色を指定する属性で、「#ff0000（赤色）」を指定しました。
「android:textSize」はテキストの大きさを指定する属性で、「22sp」を指定しました。
これらを追加することによって、どのような表示になるかはだいたい想像がつきますよね@<fn>{imagine}。
//footnote[imagine][XMLでレイアウトを作るときには、必ず「このコードを書くとどうなるかな」というのを想像するようにしましょう。コードから、実際の表示がどうなるかというのを、自分の中でイメージできることが大切なのです。]


//image[204][出力結果。テキストが赤色になり、大きくなった]{
//}

テキストが赤色になり、大きさが大きくなったのが確認できます。

このように、さまざまな属性を指定していくことで、Androidのレイアウトを作成していくのです。

== XMLの書き方

XMLは、次のような要素で構成されています。


//image[205][テキストを表示させるXML]{
//}

 * ビュー名：何を表示させるか
 * 属性：ビューに対する詳細な設定。ほとんどの場合、先頭に「android:」の接頭辞が付く
 * 値：属性の設定値

=== ビュー名

Androidでは、各パーツのことを「ビュー（View）」と呼びます。
ビュー名は、よく使うものとして次のようなものがあります。

//table[よく使うビュー名][]{
Button  ボタンを表示する
TextView        テキストを表示する
ImageView       画像を表示する
EditText        テキストフィールド（文字入力エリア）を表示する
CheckBox        チェックボックスを表示する
RadioButton     ラジオボタンを表示する
ProgressBar     プログレスバーを表示する
SeekBar シークバーを表示する
//}

=== 属性

属性は、各ビューによって設定できるものが決まっています。
たとえば、テキストを表示するTextViewであれば、色（textColor）や大きさ（textSize）の設定ができますが、画像を表示するImageViewだと、それらの設定はありません。

各ビューに設定できる属性は、それぞれにおいてかなりの数があります。
どのような属性が指定できるかは、コードを書いている中で「android:」と打っていくと、指定可能な候補一覧が表示されますので、それを見ておくとよいでしょう。


//image[206][TextViewで指定可能な属性の候補が表示される]{
//}

=== 値

値は、各属性によって設定できるものが決まっています。
これも、設定できるものが選べる場合は、属性と同じように候補一覧が表示されます@<fn>{candidate}ので、見ておくとよいでしょう。

//footnote[candidate][数値を指定するものは、候補には出てきません。自分で値を入力しないといけないので注意してください。]


//image[207][TextViewのtextStyleという属性で指定可能な値の候補が表示される]{
//}

== 主要なパーツを作ってみる

それでは、よく使うパーツを詳しく説明していきます。
これらのパーツは、何も見なくても@<fn>{notsee}XMLで作成できるようにしておくことが望ましいです。

//footnote[notsee][とはいえ、すべてを覚える必要はありません。ADTでは候補一覧が表示されますので、この機能はヒントとして積極的に使っていきましょう。]

 * ボタン
 * テキスト
 * 画像
 * テキストフィールド

=== ボタン

ボタンを表示するときには、次のようなXMLを書きます。

//listnum[button-normal][]{
<Button
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:text="Button" />
//}


//image[button-normal][ボタン表示]{
//}

デザインのカスタマイズを何もしていない、デフォルトの状態のこのボタンは、タップすると色が変わり、「タップされた」という状態をユーザーにフィードバックしています。


//image[button1-1-pressed-4][Android 4.3までは青くなる]{
//}


//image[button1-1-pressed-kitkat][Android 4.4では少し色が濃くなる]{
//}

@<list>{button-normal}の4行目の「android:text」という属性が、ボタン内に表示するテキストです。

@<list>{button-normal}の2行目の「android:layout_width」と3行目の「android:layout_height」は、ボタンだけではなくどのビューにも共通して必要な属性です。
それぞれのビューが、横方向（layout_width）または縦方向（layout_height）に対してどれぐらいの領域を占めるかという設定です。
これらには「wrap_content」と「match_parent@<fn>{matchparent}」というふたつの値が存在します。

//footnote[matchparent][APIレベルxx以前は、「fill_parent」が使われていました。古いアプリではそれが使われていることもあります。]

//table[][]{
値       説明
wrap_content    ビューを占める領域は、そのビューがもっているサイズのみにとどまる
match_parent    ビューを占める領域は、縦または横の画面サイズいっぱいまで広がる
//}

言葉に書くとむずかしいですね。
実際にどうなるかを設定して確認してみましょう。

==== wrap_content

wrap_contentを設定した場合、ビューを占める領域は、そのビューがもっているサイズのみにとどまります。つまり、「Button」という文字が設定されているボタンであれば、「Button」という文字のまわりに一定の余白がとられ、それらをひっくるめたものがビューの領域となります。


//image[button-normal][ビューを幅としたボタン]{
//}

確認のため、ボタン内に表示するテキストを変更してみます。

//listnum[button-normal-textedit][]{
<Button
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:text="登録して送信する" />
//}


//image[button-wrapcontent][ボタンの文言を変更すると、それにあわせてボタンの横幅が伸びた]{
//}

ボタン全体の横幅が、テキストの文字数にあわせて伸びました。
このように、ビューがもっているサイズにあわせたいときは、「wrap_content」を設定します。


==== match_parent

@<list>{button-normal}では、android:layout_widthもandroid:layout_heightも、両方とも「wrap_content」が設定されていました。
2行目のandroid:layout_widthを「match_parent」に変更してみます。

//listnum[button-matchparent][]{
<Button
        android:layout_width="match_parent"
        android:layout_height="wrap_content"
        android:text="Button" />
//}


//image[button-matchparent][]{
//}

横方向に、画面サイズいっぱいまで広げる設定をしましたので、ボタンは@<image>{button-matchparent}のように横幅いっぱいまで広がります。

では今度は、@<list>{button-matchparent-height}のように、android:layout_widthを「wrap_content」に戻し、android:layout_heightを「match_parent」に設定してみましょう。

//listnum[button-matchparent-height][]{
<Button
        android:layout_width="wrap_content"
        android:layout_height="match_parent"
        android:text="Button" />
//}


//image[button-matchparent-height][]{
//}

今度は縦方向に、画面サイズいっぱいまで広がりました。

実際のアプリケーションでは、ボタンをこのように縦方向の画面サイズいっぱいまで広げるようなレイアウトはほとんどありませんが、他のビューでは必要になることもありますので、覚えておきましょう。


=== 画像

画像を表示するときには、次のようなXMLを書きます。

//listnum[imageview-normal][]{
<ImageView
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:src="@drawable/ic_launcher" />
//}


//image[imageview-normal][drawable-xhdpiフォルダにあるic_launcher.pngを表示した]{
//}


//image[imageview-res][resフォルダ内のdrawableフォルダ]{
//}

@<list>{imageview-normal}の4行目の「android:src」は、resフォルダの「drawable-xxxxx」フォルダ内に入っている画像リソースのファイル名を設定する属性です。

画像の拡張子は、pngです。

ic_launcher.png@<fn>{launchericon}は、resフォルダの「drawable-mdpi」「drawable-hdpi」「drawable-xhdpi」という3つのフォルダの中に1つずつ入っています（@<image>{imageview-res}）。内容は同じですが、それぞれの画面解像度に合ったサイズのものが入っています。

//footnote[launchericon][プロジェクトにあらかじめ準備されている、ランチャーアイコンの画像。同じ名前、同じ大きさで上書きすれば、ランチャーアイコンを変更できる。]


//image[imageview-imagesize][各drawableフォルダにはそれぞれの画面密度にあった大きさの画像リソースが入っている]{
//}

「android:src」の指定では、「@drawable」をつけて、@<b>{drawableというフォルダ内を参照}します。しかしここでは「@drawable-xhdpi/ic_launcher」のように画面密度は書きません。

//list[][参照フォルダに画面密度は入れない]{
android:src="@drawable/ic_launcher"
//}

なぜなら端末ごとに、どの画面密度なのかは一意に決まっていて、@<b>{Android側で自動で判別することができる}からです。
例えば、Galaxy S3 の端末はxhdpiに属します。このときに画像を参照するフォルダは「drawable-xhdpi」ですが、コードで書くのは「@drawable/ic_launcher」でよいのです。

逆に、画面密度を指定してしまうと、他の画面密度の違ういろいろな端末から見たときに、表示がおかしくなってしまう恐れがあるので注意しましょう。

画像の大きさについては、たとえばランチャーアイコンやアクションバーのアイコンの大きさなど、Android全体でルールが決められているものはそれに従いましょう。
それ以外の自分で作るレイアウトについては、自分たち自身で決めていかなければなりません。

Android全体のルールは、ガイドラインのIconographyの項@<fn>{iconography}に書かれているので、ぜひ見ておきましょう。

//footnote[iconography][http://developer.android.com/design/style/iconography.html]


//image[iconography][各画面密度に最適化した大きさの画像が必要になる]{
//}

=== テキストフィールド

入力エリアを表示するときには、次のようなXMLを書きます。

//listnum[textfield-normal][]{
<EditText
        android:layout_width="match_parent"
        android:layout_height="wrap_content" />
//}

これだけで、最低限の入力エリアを作成することができます。


//image[textfield-normal][no title]{
//}

@<img>{textfield-normal}では、「android:layout_width="match_parent"」となっているので、入力エリアが横幅いっぱいに配置されます。



文字数によって横幅を指定したいときは、@<list>{textfield-normal-ems}のようにandroid:layout_widthをmatch_parentにして、「android:ems」を追加します。
「android:ems」には、文字にして何文字分か？という数値を設定します。

//listnum[textfield-normal-ems][]{
<EditText
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:ems="10" />
//}


//image[textfield-normal-ems][]{
//}

この場合は10文字分の横幅になります。

しかし、実はこれだけでは実用には不十分で、10文字以上文字を入力すると、このままだと改行されてしまいます。


//image[textfield-normal-ems-br][]{
//}

複数行入力させるような内容の部分ではこれでもよいのですが、通常の入力フォームでは、名前やメールアドレス、パスワードなどを入力することが多いので、これではあまり使い勝手がよくありません。

こういったときには入力制限を設定することが必要で、改行をできないようにする場合は、「android:inputType="text"」を追加します。

//listnum[textfield-normal-ems-nobr][]{
<EditText
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:ems="10"
        android:inputType="text" />
//}


//image[textfield-normal-ems-nobr][10文字以上入力しても、改行されずに左に押し込まれるようになった]{
//}

「android:inputType」という属性は、その他にもいろいろな入力制限を設定することができます。
たとえば、「android:inputType="textPassword"」を追加すると、前章にも出てきたパスワード入力用のテキストフィールドになります。

//listnum[textfield-normal-ems-pass][]{
<EditText
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:ems="10"
        android:inputType="textPassword" />
//}


//image[textfield-normal-ems-pass][パスワード用の入力エリアになった]{
//}

「android:inputType」については、場合に応じて必要な入力制限を設定するようにしましょう。
@<table>{inputtype}に、よく使われるものを挙げておきます。
すべての値は、ガイドラインのTextView > android:inputType @<fn>{inputtype} の項を参照してください。

//footnote[inputtype][https://developer.android.com/reference/android/widget/TextView.html#attr_android:inputType]

//table[inputtype][android:inputTypeでよく使う値]{
none    入力不可
text    文字を入力
textAutoCorrect 文字のスペルミスを自動で修正する
textAutoComplete        文字の補完入力をする
textMultiLine   文字を複数行入力する
textUri URLを入力する
textEmailAddress        メールアドレスを入力する
textPassword    パスワード入力する
textVisiblePassword     パスワードを隠さずに入力する
number  数値を入力する
phone   電話番号を入力する
//}

==== ユーザーにわかりやすい入力フォーム

この入力フォームをいちばんよく使うところといえば、アプリを開いて、サービスにログインするところでしょう。
メールアドレスとパスワードを入力することが圧倒的に多いです。

以前は、@<img>{textfield-form-before}のように、「Eメール」というTextViewを置き、その横にEditTextを並べていることがほとんどでした。


//image[textfield-form-before][]{
//}

しかし、最近では@<img>{textfield-dropbox-login}のように、入力エリアの中に、うすい文字で説明を書いておくことが多くなりました。


//image[textfield-dropbox-login][Dropboxアプリのログイン画面]{
//}

@<img>{textfield-form-before}との違いは、説明のテキストラベル（ここでは「Eメール」）を入力エリア外に出しておくか、入力エリア内に入れるか、ということです。

テキストラベルを入力エリア内に入れる場合は、次のように「android:hint」を追加します@<fn>{placeholder}。

//footnote[placeholder][HTMLでもテキストフィールドに説明テキストを入れることができます。HTMLの場合は、placeholderという属性になります。<input type=”text” placeholder=”Eメールアドレスを入力”>]

//list[][]{
<EditText
        android:layout_width="match_parent"
        android:layout_height="wrap_content"
        android:hint="Eメールアドレスを入力"
        android:inputType="textEmailAddress" />
//}


//image[textfield-hint][]{
//}

このように、入力エリア内に説明のテキストラベルを入れてもいいのですが、ユーザーの操作性からすると、少し問題もあります。
入力前の状態（@<img>{textfield-form-before}と@<img>{textfield-dropbox-login}）を比べてもあまり差がわからないかもしれませんので、入力後の状態を見てみましょう。


//image[textfield-form-after][テキストラベルが入力エリアの外にある場合]{
//}


//image[textfield-dropbox-after][テキストラベルが入力エリアの中にある場合、入力するとラベルが消えてしまった]{
//}

@<img>{textfield-dropbox-after}のほうは、説明のテキストラベルが消えてしまいました。
これは、今回のようなEメールとパスワードしかないような場合は問題ないですが、入力項目が多くなった場合、あとで見返したときになんの項目を入力していたかがわからなくなるので、ユーザーにとってはあまりよくないものになってしまいます。

単なるテキストフィールドとはいえ、ユーザーが実際に入力をしていくパーツです。
やみくもにEditTextを置いていくだけではなく、inputTypeをそれぞれについて最適なものを設定したりして、操作性と、認識性には常に心がけるようにしましょう。





