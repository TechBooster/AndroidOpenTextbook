= レイアウトを作成する

だいたいのパーツがつくれるようになったら、次は複数のパーツを組み合わせてレイアウトを作成していきます。
XMLにおいてレイアウトを作成し、パーツを配置していくためには、主に次の2つの方法があります。

//table[][パーツを配置していくための方法]{
レイアウト名	説明
RelativeLayout  相対レイアウト。それぞれのパーツにおいて基準となるビューを決めながら配置していく。
LinearLayout    線形レイアウト。画面の左上を中心にして、ビューを右方向または下方向へ順番に配置していく。
//}

これらの方法を、適宜組み合わせながら配置していきます。

== RelativeLayout

RelativeLayoutは、相対レイアウトともいわれます。
それぞれのパーツを配置していくときに、基準となるビューを決めながら配置をしていきます。それでは実際に作成しながら進めていきましょう。

ADTを起動し、新たにAndroidプロジェクトを作成します。

//image[new1][UIBasic3という新規プロジェクトを作成する]{
//}

//image[new2][Empty Activityを選択する]{
//}


新規に作成したAndroidプロジェクトのXMLレイアウトは、「activity_main.xml」ですが、ここにはあらかじめRelativeLayoutが記述されていますので、このまま使っていきましょう（@<img>{rltv-default}）。


//image[rltv-default][activity_main.xml]{
//}


=== 基準を親ビューにする

RelativeLayoutは相対レイアウトですから、基準となるビューが必要です。
まずは、基準を親ビューにすることを考えます。

@<img>{rltv-default}にある、すでに書かれているRelativeLayoutを使い、その中にImageViewを入れます（@<list>{rltv-imagecenter}）。
この場合の確認は、シミュレータや実機でなくても、Graphycal Layoutでもかまいません。

//listnum[rltv-imagecenter][activity_main.xml]{
<RelativeLayout xmlns:android="http://schemas.android.com/apk/res/android"
    xmlns:tools="http://schemas.android.com/tools"
    android:layout_width="match_parent"
    android:layout_height="match_parent"
    tools:context="${packageName}.${activityClass}" >
    <ImageView
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:src="@drawable/ic_launcher"  />
</RelativeLayout>
//}

1〜5行目と、10行目は、あらかじめ書かれているコードです。
中に書かれていたTextViewを一旦削除し、ImageViewを追加しましょう（@<img>{rltv-imageonly}）。

//image[rltv-imageonly][画像を表示]{
//}

==== 親ビューの中央に配置する

次に、@<img>{rltv-imagecenter}のように、画像を中央に配置するために、@<list>{rltv-imagecenter}のImageViewを次のようにします（@<list>{rltv-centerinparent}）。

//list[rltv-centerinparent][5行目に配置の指示を追加]{
<ImageView
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:src="@drawable/ic_launcher"
        android:layout_centerInParent="true" />
//}

「android:layout_centerInParent」という属性を追加しました。値は「true」です@<fn>{boolean-default}。
これは、「親ビューに対して中央に配置する」という指示になります。

//footnote[boolean-default][値がtrueかfalseの場合、デフォルト値はfalseですので、通常はtrueを指定する場合のみ書きます]


@<img>{rltv-imagecenter}のように、ドロイドくんが中央に配置されたら成功です。


//image[rltv-imagecenter][ドロイドくんを中央に配置]{
//}


//image[rltv-imagecenter-o][親ビューに対して中央に配置されている]{
//}

==== 親ビューの端にそろえて配置する

次に、中央だけではなく、親ビューの端にそろえてドロイドくんを配置していきます。
具体的には、@<img>{rltv-allposition}のように、上端、下端、左端、右端、またそれらの中央も含めて配置する方法です。


//image[rltv-allposition][親ビューの端にそろえてさまざまな場所に配置]{
//}

前述の@<list>{rltv-centerinparent}の5行目に追加した「android:layout_centerInParent="true" 」を、@<table>{rltv-parentposition}の属性を参考にしながら書き換えていきます。

//table[rltv-parentposition][親ビューに対する配置を決める属性一覧]{
属性	説明
android:layout_centerInParent   親ビューの中央に配置する
android:layout_centerHorizontal 親ビューの水平方向中央に配置する
android:layout_centerVertical   親ビューの垂直方向中央に配置する
android:layout_alignParentLeft  親ビューの左端にそろえて配置する
android:layout_alignParentRight 親ビューの右端にそろえて配置する
android:layout_alignParentTop   親ビューの上側にそろえて配置する
android:layout_alignParentBottom    親ビューの下側にそろえて配置する
//}

@<img>{rltv-allposition}のA〜Hの場合を実際に作ってみて確認してください。

=== あるビューを基準にして配置する

親を基準にしていろいろな場所に配置をしました。今度は親ビューではなく任意のビューを基準にして配置します。

@<img>{rltv-sample}のような配置を考えてみましょう。
画像（ImageView）の下にテキスト（TextView）があるような配置です。


//image[rltv-sample][画像の下にテキストがある配置]{
//}

先ほどと同じように、すでに書かれているRelativeLayoutを使い、その中にImageViewとTextViewを入れ、確認します（@<list>{rltv-normal}）。

//listnum[rltv-normal][activity_main.xml]{
<RelativeLayout xmlns:android="http://schemas.android.com/apk/res/android"
    xmlns:tools="http://schemas.android.com/tools"
    android:layout_width="match_parent"
    android:layout_height="match_parent"
    tools:context="${packageName}.${activityClass}" >
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

@<list>{rltv-normal}のとおりに記述すると@<img>{rltv-double}のように、画像とテキストが重なってしまいます。

//image[rltv-double][画像とテキストが重ってしまう]{
//}

RelativeLayoutで囲んだものは、@<b>{配置に関する設定をしなければ、中のビューは常に同じ場所に配置され、重なって表示されてしまう}という特性があるということを覚えておきましょう。

RelativeLayoutにおいて今回のような配置にする場合は、「画像を基準として、その下にテキストを配置する」という考え方をします。

==== 基準とするものにid名をつける

基準とするものについては、目印として名前をつけます。
@<list>{rltv-normal}のImageViewを次のようにします（@<list>{rltv-normal2}）。

//listnum[rltv-normal2][5行目にidを追加]{
<ImageView
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:src="@drawable/ic_launcher"
        android:id="@+id/droid" />
//}

@<list>{rltv-normal2}では「android:id」という属性を追加しました。値は、「@+id/droid」と書き、id名は「droid」になります。

これで、@<b>{このImageViewに「droid」という名前をつけた}ということになります。

==== 基準となるものに対して、どこに配置するかを指定する

引き続き、画像の下にテキストを配置しましょう。
これは表示するテキストに対して、@<b>{「droid」という名前のついたビューの下にテキストを配置する}という指示をします。

TextViewを次のように変更します（@<list>{rltv-normal3}）。

//listnum[rltv-normal3][5行目に配置の指示を追加]{
<TextView
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:text="このイラストはドロイドくんです"
        android:layout_below="@id/droid" />
//}

「android:layout_below」という属性を追加しました。値は、「@id/droid」と書き@<fn>{idname}、「droid」というid名@<fn>{idjava}を参照するものになります。

//footnote[idname][「@」と「id」の間に「+」があるかどうかに注意してください。「+」がある場合は「id名をつける」という意味になりますし、ない場合は「id名を参照する」という意味になります]

//footnote[idjava][つけたid名は、プログラムから参照するときにも使います]

「layout_below」は、基準となるパーツのどこに配置するかを指示するもので、この場合は「下に配置する」という意味になります。

このような指定をすることで、@<img>{rltv-sample}のレイアウトを作成することができます。

==== ドロイドくんを中央に置き、そのまわりにテキストを配置する

次は、ドロイドくんの画像を画面の中央に置き、そのまわりにテキストを配置します。
前述したImageViewについて、画面の中央に配置するように6行目に「android:layout_centerInParent="true"」を追加します（@<list>{rltv-normal4}）。

//listnum[rltv-normal4][6行目に配置を指示する属性を追加]{
<ImageView
    android:layout_width="wrap_content"
    android:layout_height="wrap_content"
    android:src="@drawable/ic_launcher"
    android:id="@+id/droid"
    android:layout_centerInParent="true" />
//}

また、テキストも追加しましょう(@<list>{rltv-textalign})。

//listnum[rltv-textalign][先ほど作ったテキスト以外に、さらにテキストを追加]{
<ImageView
    android:layout_width="wrap_content"
    android:layout_height="wrap_content"
    android:src="@drawable/ic_launcher"
    android:id="@+id/droid"
    android:layout_centerInParent="true" />
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

@<list>{rltv-textalign}の5行目の「android:layout_below="@id/droid"」は、@<b>{「droid」というid名がついたビューの下に配置する}という意味です。
それ以外にも、基準となるビューに対してどこに配置するかを指定する属性があります。

//table[rltv-tagposition][基準となるビューに対してどこに配置するかを決める属性一覧]{
属性	説明
android:layout_above    上側に配置する
android:layout_below    下側に配置する
android:layout_toLeftOf 左側に配置する
android:layout_toRightOf    右側に配置する
android:layout_alignLeft    左端をそろえて配置する
android:layout_alignRight   右端をそろえて配置する
android:layout_alignTop 上端をそろえて配置する
android:layout_alignBottom  下端をそろえて配置する
//}

それでは、@<list>{rltv-textalign}で追加した2つめのテキスト（TextView）を、画像の上側に配置するように変更します（@<list>{rltv-textalign-above}）

//listnum[rltv-textalign-above][16行目に上側に配置する属性を追加]{
<ImageView
    android:layout_width="wrap_content"
    android:layout_height="wrap_content"
    android:src="@drawable/ic_launcher"
    android:id="@+id/droid"
    android:layout_centerInParent="true" />
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

このままでは、@<img>{rltv-textalign-default}のように、テキストは画像の上下に配置されますが、左端に寄ってしまっている状態になります。


//image[rltv-textalign-default][テキストが画像の上下に配置]{
//}

テキストをこの位置で中央にそろえたいときは、@<list>{rltv-textalign-center}のように、6行目と12行目に「android:layout_centerHorizontal="true"」を追加します。

//listnum[rltv-textalign-center][12行目と18行目に中央にそろえる属性を追加]{
<ImageView
    android:layout_width="wrap_content"
    android:layout_height="wrap_content"
    android:src="@drawable/ic_launcher"
    android:id="@+id/droid"
    android:layout_centerInParent="true" />
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


//image[rltv-textalign-center][テキストが中央にそろえられる]{
//}


//listnum[rltv-textalign-left][12行目と18行目を左端にそろえる属性に変更]{
<ImageView
    android:layout_width="wrap_content"
    android:layout_height="wrap_content"
    android:src="@drawable/ic_launcher"
    android:id="@+id/droid"
    android:layout_centerInParent="true" />
<TextView
    android:layout_width="wrap_content"
    android:layout_height="wrap_content"
    android:text="このイラストはドロイドくんです"
    android:layout_below="@id/droid"
    android:layout_alignLeft="@id/droid" />
<TextView
    android:layout_width="wrap_content"
    android:layout_height="wrap_content"
    android:text="上側に配置します"
    android:layout_above="@id/droid"
    android:layout_alignLeft="@id/droid" />
//}


//image[rltv-textalign-left][画像の左端にテキストがそろう]{
//}

=== 演習問題

RelativeLayoutは、何かを基準としてビューを配置することを学びました。
基準とするものは、親ビューでも、任意のビューでもかまいません。自分で決めることができます。

それでは、ここで学んだRelativeLayoutを使って@<img>{rltv-input}のようなレイアウトを作成してみましょう。
言わずと知れた、日本地図です。

どのようなやり方でもかまいません。素材ファイルにあるバラバラになった画像（@<img>{rltv-images}）を使い、ImageViewを組み合わせてこのレイアウトを完成させましょう。

//image[rltv-input][完成サンプル]{
//}

//image[rltv-images][使う素材：バラバラの画像]{
//}


演習用に、まず「UIBasicMap」という新規プロジェクトを作成します。

//image[new-map1][UIBasicMapという新規プロジェクトを作成する]{
//}

//image[new-map2][Empty Activityを選択する]{
//}

素材ファイルの中身を、「res -> drawable-xhdpi」の中にコピーします。
コピー時に、「コピーをしますか？リンクをしますか？」と聞かれるので、「Copy files」を選択し、「OK」をクリックします。

//image[rltv-sample-copy][素材をコピーする]{
//}

//image[rltv-sample-copyalert][「Copy files」を選択し、「OK」]{
//}

ここまでできたら、<ImageView>を作って配置していきましょう。


== LinearLayout

LinearLayoutは、線形レイアウトの意味で、配置したビューが単純に直線的に並びます。
配置したビューは重なることなく、横方向に、または縦方向に並びます。

LinearLayoutに必要な属性は、「android:layout_width」「android:layout_height」の他に、「android:orientation」という属性が必要になります。これは、どの方向にビューを並べていくかを決める属性で、次の2つの値があります。

//table[][「android:orientation」に設定する値]{
値	説明
horizontal  中のコンテンツを水平方向（横方向）に並べる
vertical    中のコンテンツを垂直方向（縦方向）に並べる
//}

スマートフォンは、縦長の方向で使われるとは限らないので、「横方向」「縦方向」という言い方をしません。代わりに、水平/垂直という言葉がよく出てきます。

ADTを起動し、新たにAndroidプロジェクトを作成します。

//image[new3][UIBasic4という新規プロジェクトを作成する]{
//}

//image[new4][Empty Activityを選択する]{
//}

activity_main.xmlの中身を、@<list>{lnr-sample}のように書き換えます。

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

「android:orientation="horizontal"」を指定することで、テキストとボタンは横並びに配置されます（@<img>{lnr-horizontal}）。


//image[lnr-horizontal][テキストとボタンが横並びに配置]{
//}

この場合、LinearLayout内にたくさんのビューを配置してしまうと、横幅内に入りきらないものは表示されなくなってしまうので注意してください。
試しに、@<list>{lnr-btn5}のように、LinearLayout内に5つのボタンを入れてみます。

//listnum[lnr-btn5][ボタンを5つ作成]{
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

ボタンが多すぎるとスマートフォンの横幅には入りきらないので、実際には@<img>{lnr-btn5}のように表示されます。


//image[lnr-btn5][「登録する4」はギリギリ入っているが改行されて崩れる。「登録する5」は表示されない]{
//}

このようにレイアウトが崩れないために、LinearLayoutを使うときには、レイアウトに含まれるコンテンツの量に注意しましょう。

次に、縦並びを試してみましょう。
@<list>{lnr-sample}の5行目を@<list>{lnr-vertical}のように変えると、コンテンツが縦方向に並ぶため、テキストとボタンは縦並びに配置されます。

//list[lnr-vertical][5行目を変更]{
android:orientation="vertical" 
//}


//image[lnr-vertical][テキストとボタンが縦並びに配置]{
//}

@<img>{lnr-vertical}で確認できるように、たくさんのビューを配置すると縦方向にどんどん並んでいきます。


=== 領域の大きさを比率で指定する

LinearLayoutの大きな特徴は、領域の大きさを比率で指定できることです（@<list>{lnr-scale}）。
たとえば、3つのボタンを横方向に並べます。

//listnum[lnr-scale][3つのボタンを横並びに配置]{
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

「android:layout_width」がwrap_contentですので、指定されたテキストの文字数分しか横幅を持ちません（@<img>{lnr-btn3}）。

//image[lnr-btn3][ボタンの横幅は指定されたテキストの文字数分]{
//}

それぞれのボタンを横幅いっぱいに均等配列（3等分に配置）したい場合、ボタン要素を@<list>{lnr-layoutweight}のように書き換えます。

//listnum[lnr-layoutweight][ボタン要素を変更]{
<Button
        android:layout_width="0dp"
        android:layout_height="wrap_content"
        android:layout_weight="1"
        android:text="A" />
//}

2行目の「android:layout_width」を「0dp」にし、4行目に「android:layout_weight="1"」を追加しました。
3つのボタンすべてにこの変更を適用すると、横幅いっぱいに3等分されて配置されます（@<img>{lnr-btn3-equall}）。

//image[lnr-btn3-equall][ボタンが横幅いっぱいに3等分されて配置]{
//}

ここで利用した「android:layout_weight="1"」とは、割合を決める属性です。
すべてに1を設定している場合、A：B：Cの割合が、1：1：1であることになります。
また「android:layout_width」を「0dp」にするのは、いってみれば、「横幅における、ビューを占める領域の設定を無効にする」ということに近い意味になります。

では次に、A：B：Cの割合を、1：2：4にしてみましょう。
それぞれの「android:layout_weight」の値を「1」「2」「4」に変えるだけでいいですね（@<list>{lnr-layoutweight2}）。

//listnum[lnr-layoutweight2][A：B：Cの割合を1：2：4にする]{
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


//image[lnr-btn3-124][A：B：Cの割合を1：2：4に変更]{
//}

@<img>{lnr-btn3-124}では横方向について設定をしましたが、これは縦方向についても同じことができます。

つぎは画面全体を使って、@<img>{lnr-verticalbtn}のように、縦方向にボタンを並べ、ボタンの高さがA：B：C：D＝1：2：2：4になるようにしてみましょう。

//image[lnr-verticalbtn][ボタンを縦方向に並べ、高さがA：B：C：D＝1：2：2：4になるように配置]{
//}

これらの例では、ビューのすべてに「android:layout_weight」を設定しましたが、すべてに設定をする必要はありません。
一部の高さがすでに決まっている場合は、その他の部分について、@<b>{残った領域を比率によって配分する}ことになります。

たとえば、@<img>{lnr-verticalbtn}のAとDはコンテンツの成り行きの高さにあわせます（android:height="wrap_content"）。
そうすると、BとCで残った領域を比率によって配分することになります。
@<list>{lnr-weightarea}のとおり、BとCの比率をB：C＝1：3に変更すると、@<img>{lnr-weightarea}のように表示されます。

//listnum[lnr-weightarea][AとCはコンテンツの成り行きの高さになり、残りをB：C＝1：3となるように配分]{
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


//image[lnr-weightarea][AとCはコンテンツの成り行きの高さになり、残りをB：C＝1：3となるように配分]{
//}

== いろいろな行ぞろえ/列ぞろえを決めるGravity

Gravityとは、重力の意味です。Androidでは、さまざまなパーツを内包したLinearLayoutや、個々のビューに「揃え（そろえ）」を指定することができる要素です。
「そろえ」という説明をしましたが、@<b>{ビューに重力をかけて、配置する位置を決める}というと、イメージしやすいかもしれません。

実際に利用できるGravityには2種類あります。

//table[][Gravityの種類]{
属性	説明
android:gravity 親となるビューに指定することで、中に入るコンテンツのそろえを設定する
android:layout_gravity  それぞれのビューに指定することで、そのビューのそろえを設定する
//}

=== android:gravity

android:gravityは、親となるビューに指定する属性です。これを指定すると、中に入るコンテンツのそろえを設定することができます。

まず、@<list>{gravity-simple}のXMLで考えてみます。
縦方向にコンテンツが並んでいくLinearLayoutの中に、ボタンが4つ入っているシンプルなものです。

//listnum[gravity-simple][ボタン4つを並べる]{
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

//table[][android:gravityの値]{
値	説明
top 上に寄せる
bottom  下に寄せる
left    左に寄せる
right   右に寄せる
center_vertical 垂直方向中央にそろえる
center_hotizontal   水平方向中央にそろえる
center  画面の中央にそろえる
//}

指定なし、bottom、rightをそれぞれ指定したものが@<img>{gravity-simple}です。


//image[gravity-simple][親のLinearLayoutに対するandroid:gravityの指定]{
//}

右下に配置したいときには、「android:gravity="right|bottom"」といった具合に、値を「 |（ビットOR）」でつなぐと、複数指定することができます。

コードで覚えるときに想像しづらい人はなるべく自分の中にイメージを持てるように工夫するといいでしょう。
たとえば、Gravityは、先ほども述べたとおり、@<b>{重力}の意味があります。親となるLinearLayoutにどのような重力をかけるか、ということを考えるとイメージしやすいかもしれません。

またGravityは、レイアウトだけでなくボタン（Button）やテキスト（TextView）といったビューにも指定できます。

たとえば、@<list>{gravity-simple}のBのボタンをテキスト（TextView）に変えました。このテキストについて、横方向にmatch_parentにします（@<list>{gravity-textnormal}）。

//listnum[gravity-textnormal][BのボタンをTextViewに変え、「android:layout_width="match_parent"」に変更]{
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

そうすると、横方向にwrap_contentであるA、C、Dのボタンは、親のLinearLayoutの「右ぞろえ（@<list>{gravity-textnormal}の6行目の「android:gravity="right"」）」を受けてすべて右端に寄っていますが、テキストだけmatch_parentなために、横幅いっぱいに領域が伸びています（@<img>{gravity-text100per}）。


//image[gravity-text100per][テキストの領域は横幅いっぱいに伸びる]{
//}

ここで、TextViewに「android:gravity="right"」を付け加えると、テキストが右寄せになります（@<img>{gravity-text100per-right}）。

//listnum[][TextViewに「android:gravity="right"」を追加]{
<TextView
        android:layout_width="match_parent"
        android:layout_height="wrap_content"
        android:gravity="right"
        android:text="テキストを設定します" />
//}


//image[gravity-text100per-right][テキストのみ右寄せになる]{
//}

「android:gravity」は、どの要素にも設定することができますが、「箱全体に重力をかける」というイメージをもっておくとよいでしょう。箱全体に特定の方向の重力をかけると、中の要素が指定した方向に寄っていくのは、自然界でも容易に想像できますよね。

=== android:layout_gravity

android:layout_gravityは、それぞれのビューに指定することで、そのビューの配置を設定する属性です。これらのビューには、配置する方向にwrap_contentが指定されている必要があります。
たとえば、@<img>{layoutgravity-simple}には「A」〜「D」の4つのボタンがありますが、すべて横方向にwrap_contentが指定されています。そして、デフォルトの左寄せになっています@<list>{layoutgravity-simple}。


//image[layoutgravity-simple][4つのボタンを配置]{
//}

//listnum[layoutgravity-simple][4つのボタンを配置]{
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

ここでBのボタンのみを右寄せに変更したい場合、ボタン「B」のコード（属性）に「android:layout_gravity="right"」を追加します。

//listnum[][android:layout_gravityを追加する]{
<Button
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:layout_gravity="right"
        android:text="B" />
//}


//image[layoutgravity-right-b][Bのボタンのみ右寄せに配置]{
//}

「android:layout_gravity」に指定できる値のうち、よく使うものは次のとおりです。

//table[][android:layout_gravityの値]{
値	説明
top 上に寄せる
bottom  下に寄せる
left    左に寄せる
right   右に寄せる
center_vertical 垂直方向中央にそろえる
center_hotizontal   水平方向中央にそろえる
center  画面の中央にそろえる
//}

ほとんどが、「android:gravity」と同じと考えてもよいでしょう。
違うのは、箱全体に重力をかけるのか、または箱の中の各要素に個別に重力をかけるのか、の違いです。

//image[gravity-all][gravityとlayout_gravityの違い]{
//}


== Androidのレイアウト方法

ここまでで、Androidの基本的なレイアウト方法を学びました。
RelativeLayout/LinearLayoutは、それぞれの考え方が違いますが、さまざまな画面サイズの端末に簡単に対応できる、という点では共通しています。

RelativeLayoutであれば、親ビューを基準にしたり、もしくは任意のビューを基準にして配置します。
LinearLayoutであれば、ビューの占める領域を比率によって指定することができます。
これらは、HTML/CSSによるWebサイトのレイアウトや、iPhoneアプリの画面レイアウトとはまったく違った考え方を持っていて、画面サイズがどんなに変わってもそのときに応じて配置できるというのが特徴です。

これは、AndroidというOSはオープンソースであり、多くのキャリア（携帯事業者）やメーカーが利用しており、どのような画面サイズの端末でも動く、ということにもつながります。
極端なものでは、縦横比すらまったく違うような、正方形のような画面サイズのものもあるでしょう。

多種多様な画面サイズがある中、すべてで完璧に動作させることは難しいですが、レイアウトを作成する段階で、画面サイズの変化に柔軟に対応できるように心がけましょう。

== 演習問題

いままでの内容の復習として次のレイアウトを作成してみましょう。

//image[sample][完成サンプル]{
//}

演習用に、まず「UIBasicPhone」という新規プロジェクトを作成します。

//image[new-phone1][UIBasicPhoneという新規プロジェクトを作成する]{
//}

//image[new-phone2][Empty Activityを選択する]{
//}

ここまでできたら、ボタン（Button）を配置してレイアウトを作成していきましょう。
