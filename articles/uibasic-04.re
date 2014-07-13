= レイアウトを作成する

だいたいのパーツがつくれるようになったら、次は複数のパーツを組み合わせてレイアウトを作成していきます。
XMLにおいてレイアウトを作成し、パーツを配置していくためには、主に次の2つの方法があります。

//table[パーツを配置していくための方法][]{
RelativeLayout	相対配置。それぞれのパーツにおいて基準となるビューを決めながら配置していく。
LinearLayout	絶対配置。画面の左上を中心にして、ビューを右方向または下方向へ順番に配置していく。
//}

これらの方法を、適宜組み合わせながら配置していきます。

== RelativeLayout

RelativeLayoutは、相対配置ともいわれます。
それぞれのパーツを配置していくときに、基準となるビューを決めながら配置をしていきます。それでは実際に作成しながら説明をしていきます。

新規に作成したAndroidプロジェクトのXMLレイアウトは、「fragment_main.xml」ですが、ここにはあらかじめRelativeLayoutが記述されていますので、このまま使っていきましょう。


//image[rltv-default][]{
//}


=== 基準を親ビューにする

RelativeLayoutは相対配置ですから、基準となるビューが必要です。
まずは、基準を親ビューにすることを考えます。

@<img>{rltv-default}にある、すでに書かれているRelativeLayoutを使い、その中にImageViewを入れます。
この場合の確認は、シミュレータや実機でなくても、Graphycal Layoutでもかまいません。

//listnum[rltv-imagecenter][fragment_main.xml]{
<RelativeLayout xmlns:android="http://schemas.android.com/apk/res/android"
    xmlns:tools="http://schemas.android.com/tools"
    android:layout_width="match_parent"
    android:layout_height="match_parent"
    android:paddingBottom="@dimen/activity_vertical_margin"
    android:paddingLeft="@dimen/activity_horizontal_margin"
    android:paddingRight="@dimen/activity_horizontal_margin"
    android:paddingTop="@dimen/activity_vertical_margin"
    tools:context="com.example.androidxmlbasic.MainActivity$PlaceholderFragment" >
    <ImageView
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:src="@drawable/ic_launcher"  />
</RelativeLayout>
//}

1〜9行目と、14行目は、あらかじめ書かれているコードです。
中の、ImageViewを追加しましょう。



//image[rltv-imageonly][]{
//}

==== 親ビューの中央に配置する

次に、@<img>{rltv-imagecenter}のように、画像を中央に配置するために、@<list>{rltv-imagecenter}のImageViewを次のようにします。

//list[rltv-centerinparent][5行目に配置の指示を追加した]{
<ImageView
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:src="@drawable/ic_launcher"
        android:layout_centerInParent="true" />
//}

「android:layout_centerInParent」という属性を追加しました。値は「true」です@<fn>{boolean-default}。
これは、「親ビューに対して中央に配置する」という指示になります。

//footnote[boolean-default][値がtrueかfalseの場合、デフォルト値はfalseですので、通常はtrueを指定する場合のみ書きます]


//image[rltv-imagecenter][]{
//}


//image[rltv-imagecenter-o][親ビューに対して中央に配置されている]{
//}

@<img>{rltv-imagecenter}のように、ドロイドくんが中央に配置されたら成功です。

==== 親ビューの端にそろえて配置する

次に、中央だけではなく、親ビューの端にそろえてドロイドくんを配置していきます。
具体的には、@<img>{rltv-allposition}のように、上端、下端、左端、右端、またそれらの中央も含めて配置する方法です。


//image[rltv-allposition][親ビューの端にそろえてさまざまな場所に配置する]{
//}

@<list>{rltv-centerinparent}の5行目に追加した「android:layout_centerInParent="true" 」を、@<table>{rltv-parentposition}の属性を参考にしながら書き換えていきます。

//table[rltv-parentposition][親ビューに対する配置を決める属性一覧]{
android:layout_centerInParent	親ビューの中央に配置する
android:layout_centerHorizontal	親ビューの水平方向中央に配置する
android:layout_centerVertical	親ビューの垂直方向中央に配置する
android:layout_alignParentLeft	親ビューの左端にそろえて配置する
android:layout_alignParentRight	親ビューの右端にそろえて配置する
android:layout_alignParentTop	親ビューの上側にそろえて配置する
android:layout_alignParentBottom	親ビューの下側にそろえて配置する
//}

@<img>{rltv-allposition}のA〜Hの場合を実際に作ってみて確認してください。

=== あるビューを基準にして配置する

親を基準にしていろいろな場所に配置をしました。今度は親ビューではなく任意のビューを基準にして配置します。

@<img>{rltv-sample}のような配置を考えてみましょう。
画像（ImageView）の下にテキスト（TextView）があるような配置です。


//image[rltv-sample][]{
//}

先ほどと同じように、すでに書かれているRelativeLayoutを使い、その中にImageViewとTextViewを入れ、確認します。

//listnum[rltv-normal][fragment_main.xml]{
<RelativeLayout xmlns:android="http://schemas.android.com/apk/res/android"
    xmlns:tools="http://schemas.android.com/tools"
    android:layout_width="match_parent"
    android:layout_height="match_parent"
    android:paddingBottom="@dimen/activity_vertical_margin"
    android:paddingLeft="@dimen/activity_horizontal_margin"
    android:paddingRight="@dimen/activity_horizontal_margin"
    android:paddingTop="@dimen/activity_vertical_margin"
    tools:context="com.example.androidxmlbasic.MainActivity$PlaceholderFragment" >
    <ImageView
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:src="@drawable/ic_launcher"  />
    <TextView
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:text="このイラストはドロイドくんです"  />
</RelativeLayout>
//}


//image[rltv-double][]{
//}

そうすると、@<img>{rltv-double}のように、画像とテキストが重なってしまいます。
RelativeLayoutで囲んだものは、@<b>{配置に関する設定をしなければ、中のビューは常に同じ場所に配置され、重なって表示されてしまう}という特性があるということを覚えておきましょう。

RelativeLayoutにおいて今回のような配置にする場合は、「画像を基準として、その下にテキストを配置する」という考え方をします。

==== 基準とするものにid名をつける

基準とするものについては、目印として名前をつけます。
@<list>{rltv-normal}のImageViewを次のようにします。

//listnum[][5行目にidを追加した]{
<ImageView
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:src="@drawable/ic_launcher"
        android:id="@+id/droid" />
//}

「android:id」という属性を追加しました。値は、「@+id/droid」と書き、id名は「droid」になります。

これで、@<b>{このImageViewに「droid」という名前をつけた}ということになります。

==== 基準となるものに対して、どこに配置するかを指定する

次に、画像の下に配置するテキストについてです。
これは、テキストに対して、@<b>{「droid」という名前のついたビューの下にテキストを配置する}という指示をします。

@<list>{rltv-normal}のTextViewを次のようにします。

//listnum[][5行目に配置の指示を追加した]{
<TextView
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:text="このイラストはドロイドくんです"
        android:layout_below="@id/droid" />
//}

「android:layout_below」という属性を追加しました。値は、「@id/droid」と書き@<fn>{idname}、「droid」というid名を参照するものになります@<fn>{idjava}。

//footnote[idname][「@」と「id」の間に「+」があるかどうかに注意してください。「+」がある場合は「id名をつける」という意味になりますし、ない場合は「id名を参照する」という意味になります]

//footnote[idjava][つけたidは、プログラムから参照するときにも使います]

「layout_below」は、基準となるパーツのどこに配置するかを指示するもので、この場合は「下に配置する」という意味になります。

このような指定をすることで、@<img>{rltv-sample}のレイアウトを作成することができます。

==== ドロイドくんを中央に置き、そのまわりにテキストを配置する

次は、ドロイドくんの画像を画面の中央に置き、そのまわりにテキストを配置します。
前項で指定したImageViewについて、画面の中央に配置するように6行目に「android:layout_centerInParent="true"」を追加します。

//listnum[][6行目に配置を指示する属性を追加した]{
<ImageView
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:src="@drawable/ic_launcher"
        android:id="@+id/droid"
        android:layout_centerInParent="true" />
//}

また、テキストも追加しましょう。

//listnum[rltv-textalign][先ほど作ったテキスト以外に、さらにテキストを追加した]{
    <TextView
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:text="このイラストはドロイドくんです"
        android:layout_below="@id/droid" />
    <TextView
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:text="上側に配置します" />
//}

@<list>{rltv-textalign}の5行目の「android:layout_below="@id/droid"」は、@<b>{「droid」というid名がついたビューの下に配置する}という意味でした。
それ以外にも、基準となるビューに対してどこに配置するかを指定する属性があります。

//table[rltv-tagposition][基準となるビューに対してどこに配置するかを決める属性一覧]{
android:layout_above	上側に配置する
android:layout_below	下側に配置する
android:layout_toLeftOf	左側に配置する
android:layout_toRightOf	右側に配置する
android:layout_alignLeft	左端をそろえて配置する
android:layout_alignRight	右端をそろえて配置する
android:layout_alignTop	上端をそろえて配置する
android:layout_alignBottom	下端をそろえて配置する
//}

値は、すべてid名となります。

@<list>{rltv-textalign}の2つめのTextViewを、画像の上側に配置するように変更します。

//listnum[rltv-textalign-above][10行目に上側に配置する属性を追加した]{
    <TextView
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:text="このイラストはドロイドくんです"
        android:layout_below="@id/droid" />
    <TextView
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:text="上側に配置します"
        android:layout_above="@id/droid" />
//}

このままでは、@<img>{rltv-textalign-default}のように、テキストは画像の上下に配置されるが、左端に寄ってしまっている状態になります。


//image[rltv-textalign-default][]{
//}

テキストをこの位置で中央にそろえたいときは、@<list>{rltv-textalign-center}のように、6行目と12行目に「android:layout_centerHorizontal="true"」を追加します。

//listnum[rltv-textalign-center][6行目と12行目に中央にそろえる属性を追加した]{
    <TextView
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:text="このイラストはドロイドくんです"
        android:layout_below="@id/droid"
        android:layout_centerHorizontal="true" />
    <TextView
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:text="上側に配置します"
        android:layout_above="@id/droid"
        android:layout_centerHorizontal="true" />
//}


//image[rltv-textalign-center][]{
//}


//listnum[rltv-textalign-left][6行目と12行目を左端にそろえる属性に変更した]{
    <TextView
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:text="このイラストはドロイドくんです"
        android:layout_below="@id/droid"
        android:layout_toLeftOf="@id/droid" />
    <TextView
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:text="上側に配置します"
        android:layout_above="@id/droid"
        android:layout_toLeftOf="@id/droid" />
//}


//image[rltv-textalign-left][]{
//}

=== 演習
RelativeLayoutは、何かを基準としてビューを配置することを学びました。
基準とするものは、親ビューでも、任意のビューでもかまいません。自分で決めることができます。

それでは、この章で学んだRelativeLayoutを使って@<img>{rltv-input}のようなレイアウトを作成してみましょう。
画面下部にテキストフィールドがあり、その右側に送信ボタンがあるというものです。SNSのアプリでは、メッセージや近況を書くときのお決まりのレイアウトです。


//image[rltv-input][]{
//}





== LinearLayout

LinearLayoutは、絶対配置ともいわれます。
配置したビューは重なることなく、横方向に、または縦方向に並びます。

LinearLayoutに必要な属性は、「android:layout_width」「android:layout_height」の他に、「android:orientation」という属性が必要になります。

//table[][「android:orientation」に設定する値]{
horizontal	中のコンテンツを横方向に並べる
vertical	中のコンテンツを縦方向に並べる
//}

まず、新規Androidプロジェクトを作成し、fragment_main.xmlの中身を、@<list>{lnr-sample}のように書き換えます。

//list[lnr-sample][]{
<LinearLayout xmlns:android="http://schemas.android.com/apk/res/android"
    xmlns:tools="http://schemas.android.com/tools"
    android:layout_width="match_parent"
    android:layout_height="wrap_content"
    android:orientation="horizontal" >
    <TextView
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:text="この内容で登録します。よろしいですか？" />
    <Button
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:text="登録する" />
</LinearLayout>
//}

「android:orientation="horizontal"」が指定されていますので、テキストとボタンは横並びに配置されます。


//image[lnr-horizontal][]{
//}

この場合、LinearLayout内にたくさんのビューを配置してしまうと、横幅内に入りきらないものは表示されなくなってしまうので注意してください。
試しに、@<list>{lnr-btn5}のように、LinearLayout内に5つのボタンを入れてみます。

//listnum[lnr-btn5][]{
<Button
    android:layout_width="wrap_content"
    android:layout_height="wrap_content"
    android:text="登録する1" />
<Button
    android:layout_width="wrap_content"
    android:layout_height="wrap_content"
    android:text="登録する2" />
<Button
    android:layout_width="wrap_content"
    android:layout_height="wrap_content"
    android:text="登録する3" />
<Button
    android:layout_width="wrap_content"
    android:layout_height="wrap_content"
    android:text="登録する4" />
<Button
    android:layout_width="wrap_content"
    android:layout_height="wrap_content"
    android:text="登録する5" />
//}

そうすると、スマートフォンの横幅には入りきらないので、実際には@<img>{lnr-btn5}のように表示されます。


//image[lnr-btn5][「登録する4」はギリギリ入っているが改行されて崩れる。「登録する5」は表示されない]{
//}

こうならないために、LinearLayoutを使うときには、中のコンテンツの量に注意しましょう。

次に、縦並びです。
@<list>{lnr-sample}の5行目を@<list>{lnr-vertical}のように変えると、コンテンツが縦方向に並ぶため、テキストとボタンは縦並びに配置されます。

//list[lnr-vertical][]{
android:orientation="vertical" 
//}


//image[lnr-vertical][]{
//}

この場合は、たくさんのビューを配置すると縦方向にどんどん並んでいきます。


=== 領域の大きさを比率で指定する

LinearLayoutの大きな特徴は、領域の大きさを比率で指定できることです。
たとえば、3つのボタンを横方向に並べます。

//listnum[][]{
<LinearLayout xmlns:android="http://schemas.android.com/apk/res/android"
    xmlns:tools="http://schemas.android.com/tools"
    android:layout_width="match_parent"
    android:layout_height="match_parent"
    android:orientation="horizontal" >
    <Button
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:text="A" />
    <Button
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:text="B" />
    <Button
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:text="C" />
</LinearLayout>
//}


//image[lnr-btn3][]{
//}

「android:layout_width」がwrap_contentですので、指定されたテキストの文字数分しか横幅を持ちません。
それぞれのボタンを、この横幅いっぱいに均等配列（3等分に配置）したい場合、それぞれのボタンを@<list>{lnr-layoutweight}のように書き換えます。

//listnum[lnr-layoutweight][]{
<Button
        android:layout_width="0dp"
        android:layout_height="wrap_content"
        android:layout_weight="1"
        android:text="A" />
//}

2行目の「android:layout_width」を「0dp」にし、4行目に「android:layout_weight="1"」を追加しました。
3つのボタンすべてをこのように変更すると、横幅いっぱいに3等分されて配置されます。


//image[lnr-btn3-equall][]{
//}

「android:layout_weight="1"」とは、割合を決める属性です。
すべてに1を設定している場合、A：B：Cの割合が、1：1：1であることになります。
また「android:layout_width」を「0dp」にするのは、いってみれば、「横幅における、ビューを占める領域の設定を無効にする」ということに近い意味になります。

では次に、A：B：Cの割合を、1：2：4にしてみましょう。
それぞれの「android:layout_weight」の値を「1」「2」「4」に変えるだけでいいですね。

//listnum[][]{
    <Button
        android:layout_width="0dp"
        android:layout_height="wrap_content"
        android:layout_weight="1"
        android:text="A" />
    <Button
        android:layout_width="0dp"
        android:layout_height="wrap_content"
        android:layout_weight="2"
        android:text="B" />
    <Button
        android:layout_width="0dp"
        android:layout_height="wrap_content"
        android:layout_weight="4"
        android:text="C" />
//}


//image[lnr-btn3-124][A：B：Cの割合が1：2：4になった]{
//}

横方向について設定をしましたが、これは縦方向についても同じことができます。
画面全体を使って、@<img>{lnr-verticalbtn}のように、縦方向にボタンを並べ、ボタンの高さがA：B：C：D＝1：2：2：4になるようにしてみましょう。


//image[lnr-verticalbtn][]{
//}

これらの例では、ビューのすべてに「android:layout_weight」を設定しましたが、すべてに設定をする必要はありません。
一部の高さがすでに決まっている場合は、その他の部分について、@<b>{残った領域を比率によって配分する}ことになります。

たとえば、@<img>{lnr-verticalbtn}のAとDはコンテンツの成り行きの高さにあわせます（android:height="wrap_content"）。
そうすると、BとCで残った領域を比率によって配分することになります。
@<list>{lnr-weightarea}のように、B：C＝1：3にすると、@<img>{lnr-weightarea}のように表示されます。

//listnum[lnr-weightarea][]{
    <Button
        android:layout_width="match_parent"
        android:layout_height="wrap_content"
        android:text="A" />
    <Button
        android:layout_width="match_parent"
        android:layout_height="0dp"
        android:layout_weight="1"
        android:text="B" />
    <Button
        android:layout_width="match_parent"
        android:layout_height="0dp"
        android:layout_weight="3"
        android:text="C" />
    <Button
        android:layout_width="match_parent"
        android:layout_height="wrap_content"
        android:text="D" />
//}


//image[lnr-weightarea][]{
//}

== いろいろな行ぞろえ/列ぞろえを決めるGravity

さまざまなパーツを内包したLinearLayoutや、個々のビューに「そろえ」を指定することができるGravityを知っておきましょう。
Gravityとは、重力の意味です。
「そろえ」という説明をしましたが、@<b>{ビューに重力をかけて、配置する位置を決める}というと、イメージしやすいかもしれません。

Gravityには2種類あります。

//table[][]{
android:gravity	親となるビューに指定することで、中に入るコンテンツのそろえを設定する
android:layout_gravity	それぞれのビューに指定することで、そのビューのそろえを設定する
//}

=== android:gravity

android:gravityは、親となるビューに指定する属性です。これを指定すると、中に入るコンテンツのそろえを設定することができます。

まず、@<list>{gravity-simple}のXMLで考えてみます。
縦方向にコンテンツが並んでいくLinearLayoutの中に、ボタンが4つ入っているシンプルなものです。

//listnum[gravity-simple][]{
<LinearLayout xmlns:android="http://schemas.android.com/apk/res/android"
    xmlns:tools="http://schemas.android.com/tools"
    android:layout_width="match_parent"
    android:layout_height="match_parent"
    android:orientation="vertical"
    android:gravity="xxxxxx" >
    <Button
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:text="A" />
    <Button
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:text="B" />
    <Button
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:text="C" />
    <Button
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:text="D" />
</LinearLayout>
//}

@<list>{gravity-simple}の6行目の「android:gravity="xxxxxx"」の「xxxxxx」に入るもののうち、よく使うものは次のとおりです。

//table[][]{
top	上に寄せる
bottom	下に寄せる
left	左に寄せる
right	右に寄せる
center_vertical	垂直方向中央にそろえる
center_hotizontal	水平方向中央にそろえる
center	画面の中央にそろえる
//}


//image[gravity-simple][親のLinearLayoutに対するandroid:gravityの指定]{
//}

また、@<img>{gravity-simple}にはない右下に配置したいときには、「android:gravity=”right|bottom”」といった具合に、値を「 |（パイプ）」でつなぐと、複数指定することができます。

コードで覚えるとわかりづらい人は、イメージで覚えましょう。
このgravityは、先ほども述べたとおり、@<b>{重力}の意味があります。親となるLinearLayoutにどのような重力をかけるか、ということを考えるとイメージしやすいのではないでしょうか。

また、この重力は、ButtonやTextViewといったビューにも指定できます。

たとえば、@<list>{gravity-simple}のBのボタンをTextViewに変えました。このTextViewについて、横方向にmatch_parentにします。

//listnum[gravity-textnormal][]{
<LinearLayout xmlns:android="http://schemas.android.com/apk/res/android"
    xmlns:tools="http://schemas.android.com/tools"
    android:layout_width="match_parent"
    android:layout_height="match_parent"
    android:orientation="vertical"
    android:gravity="right" >
    <Button
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:text="A" />
    <TextView
        android:layout_width="match_parent"
        android:layout_height="wrap_content"
        android:text="テキストを設定します" />
    <Button
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:text="C" />
    <Button
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:text="D" />
</LinearLayout>
//}

そうすると、横方向にwrap_contentであるA、C、Dのボタンは、親のLinearLayoutの「右ぞろえ（@<list>{gravity-textnormal}の6行目の「android:gravity="right"」）」を受けてすべて右端に寄っていますが、TextViewだけmatch_parentなために、横幅いっぱいに領域が伸びています@<img>{gravity-text100per}。


//image[gravity-text100per][]{
//}

ここで、TextViewに「android:gravity="right"」を付け加えると、テキストが右寄せになります@<img>{gravity-text100per-right}。

//listnum[][]{
<TextView
        android:layout_width="match_parent"
        android:layout_height="wrap_content"
        android:gravity="right"
        android:text="テキストを設定します" />
//}


//image[gravity-text100per-right][]{
//}

これらのことから、「android:gravity」は、横方向であっても縦方向であっても「match_parent」を使っている方向に対して有効だ、ということがいえます@<fn>{gravity-match}。
また、相対的にレイアウトを考えるRelativeLayout自体には使えないことも想像できるでしょう。

//footnote[gravity-match][この場合、厳密には、TextViewのandroid:layout_widthをwrap_contentにしても、ちゃんと右寄せになります]

=== android:layout_gravity

android:layout_gravityは、それぞれのビューに指定することで、そのビューのそろえを設定する属性です。これらのビューには、そろえる方向についてwrap_contentが指定されている必要があります。
たとえば、@<img>{layoutgravity-simple}には「A」〜「D」の4つのボタンがありますが、すべて横方向にwrap_contentが指定されています。そして、デフォルトの左寄せになっています@<list>{layoutgravity-simple}。


//image[layoutgravity-simple][]{
//}

//listnum[layoutgravity-simple][]{
<LinearLayout xmlns:android="http://schemas.android.com/apk/res/android"
    xmlns:tools="http://schemas.android.com/tools"
    android:layout_width="match_parent"
    android:layout_height="match_parent"
    android:orientation="vertical" >
    <Button
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:text="A" />
    <Button
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:text="B" />
    <Button
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:text="C" />
    <Button
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:text="D" />
</LinearLayout>
//}

この中で、Bのボタンのみを右寄せにしたい場合、Bのコードに「android:layout_gravity="right"」を追加します。

//listnum[][4行目を追加した]{
<Button
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:layout_gravity="right"
        android:text="B" />
//}


//image[layoutgravity-right-b][]{
//}

gravity関連については、実際にレイアウトを組んでいく上で「こういうときにはどうするべきか」というのをその都度考え、実装していくのがよいでしょう。

== Androidのレイアウト方法

この章では、Androidの基本的なレイアウト方法を学びました。
RelativeLayout/LinearLayoutは、それぞれの考え方が違いますが、さまざまな画面サイズの端末に簡単に対応できる、という点では共通しています。

RelativeLayoutであれば、親ビューを基準にしたり、もしくは任意のビューを基準にして配置します。
LinearLayoutであれば、ビューの占める領域を比率によって指定することができます。
これらは、HTML/CSSによるWebサイトのレイアウトや、iPhoneアプリの画面レイアウトとはまったく違った考え方を持っていて、画面サイズがどんなに変わってもそのときに応じてできるというのが特徴です。

これは、AndroidというOSはオープンソースであり、どのような画面サイズの端末にも入れることができるということにつながります。中には、縦横比すらまったく違うような、正方形のような画面サイズのものもあるでしょう。

レイアウトを作成するときには、画面サイズが変わってもある程度は許容できるようなレイアウトを心がけましょう。

== 実習

次のレイアウトを作成しましょう。

//image[sample][]{
//}