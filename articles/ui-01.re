= UI応用
本章ではAndroidのViewシステムについて解説します。Androidでは、すべての画面がViewという要素によって構成されています。
もちろん、これらの動作原理を知らなくてもUIコンポーネント、レイアウトは利用できます。
しかし、システム負荷の小さなレイアウト配置、効率的なリソース構成を考える上では欠かすことのできない知識です。

これらの知識をベースとしてボタンやチェックボックスといったUIコンポーネントのカスタマイズ、
アニメーションの適用について解説していきます。

== Viewの役割
Viewは、Androidアプリケーションで最も基本的な単位で、UIコンポーネントはViewクラスを継承して作成されています。

#@# View → TextView, ImageView

TextViewやImageViewといったUIコンポーネントもViewクラスを継承しており、Viewは、もっとも基本的な要素のひとつです。
ViewにはUIを考える上で、開発者が欲しいと思う一般的な操作がまとめられています。

 * フォーカスの設定
 * Viewの表示設定
 * プロパティの設定
 * リスナーの設定

フォーカスの設定では、Androidはユーザー入力に応じてフォーカスを自動的に移動させます。
特定のViewへフォーカスを指定したい場合は、requestFocusメソッドを使います。

Viewの表示設定は、setVisibilityメソッドを使うことでViewの表示・非表示状態を変更できます。
それぞれViewを継承した各UIコンポーネントでも利用できるメソッドです。

プロパティ@<fn>{property}は、ソースコードまたはXMLから可能です。たとえばTextViewではsetTextメソッドとandroid:text属性
があります（@<list>{01}、@<list>{02}）。

//footnote[property][オブジェクトの保持しているデータ。Viewであれば高さや幅が必要などオブジェクトの性質を表すものが一般的]

//list[01][buttonのプロパティを設定する（Java）]{
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        
        Button btn  = (Button) findViewById(R.id.button);
        btn.setText("Hello!");
    }
//}
ソースコードから文字列を設定するには、setTextメソッドが利用できます。

XMLからはandroid:text属性で文字列を設定できます。
//list[02][buttonのプロパティを設定する（XML）]{
<RelativeLayout ...省略...
    <Button
        android:id="@+id/button"
        android:layout_width="match_parent"
        android:layout_height="wrap_content"
        android:layout_alignParentLeft="true"
        android:layout_alignParentRight="true"
        android:layout_centerVertical="true"
        android:text="Hello!" />
</RelativeLayout>
//}

このようにJavaのコードやXMLを使って、Viewを組み合わせることで画面が形作られています。

ソースコードを利用したほうが表現力の幅は広がり、できることも多いですが、
レイアウトがいろんなところから参照されることで複雑度が格段にあがります。

基本的にはXMLで構築し、制御が必要な場合にのみソースコードから参照するようにしましょう。

最後のリスナー@<fn>{listener}の設定は、Viewに変化があった場合に通知する仕組みです。

Viewには、フォーカスに変化があったときに、呼び出されるリスナーを登録するsetOnFocusChangeListenerメソッド、
Viewがクリックされたときに呼び出されるリスナーを登録するsetOnClickListenerメソッド、
タッチイベントが発生したときに呼び出されるリスナーを登録するsetOnTouchListenerなどが用意されています。

//footnote[listener][リスナーとは、イベントが発生したことを契機に呼び出される関数、ルーチン、またはその仕組みを指します]

=== UIコンポーネントのXML属性と関連するメソッド

この説ではViewのXML属性と関連するメソッドを解説します（@<table>{xml}）。

//table[xml][XML属性と関連するメソッド]{
XML要素		関連メソッド	概要
----
android:accessibilityLiveRegion		setAccessibilityLiveRegion(int)			アクセシビリティサービスへの通知種類
android:alpha						setAlpha(float)							Viewの透明度
android:background					setBackgroundResource(int)				背景に設定するdrawableを指定
android:clickable					setClickable(boolean)					クリックの有効判定
android:contentDescription			setContentDescription(CharSequence)		コンテンツの概要
android:drawingCacheQuality			setDrawingCacheQuality(int)				キャッシュの品質
android:duplicateParentState		なし									子ビューの状態を共有
android:fadeScrollbars				setScrollbarFadingEnabled(boolean)		フェードの有効判定
android:fadingEdgeLength			getVerticalFadingEdgeLength()			フェードアウトする領域の幅
android:filterTouchesWhenObscured	setFilterTouchesWhenObscured(boolean)	タッチフィルタリング
android:fitsSystemWindows			setFitsSystemWindows(boolean)			レイアウトでシステムウィンドウを考慮する
android:focusable					setFocusable(boolean)					フォーカスの有効判定
android:focusableInTouchMode		setFocusableInTouchMode(boolean)		タッチモード時のフォーカスの有効判定
android:hapticFeedbackEnabled		setHapticFeedbackEnabled(boolean)		触覚フィードバック
android:id							setId(int)								findViewByIdで取得できる識別子
android:importantForAccessibility	setImportantForAccessibility(int)		アクセシビリティで特別に必要な場合
android:isScrollContainer			setScrollContainer(boolean)				IMEが表示される場合のスクロール設定
android:keepScreenOn				setKeepScreenOn(boolean)				表示中はスクリーンをON
android:layerType					setLayerType(int,Paint)					ハードウェアレイヤが指定可能
android:layoutDirection				setLayoutDirection(int)					アラビア語圏などRTL設定
android:longClickable				setLongClickable(boolean)				長押し有効判定
android:minHeight					setMinimumHeight(int)					高さの最小値
android:minWidth					setMinimumWidth(int)					横幅の最小値
android:nextFocusDown				setNextFocusDownId(int)					FOCUS_DOWN移動時の遷移先View
android:nextFocusForward			setNextFocusForwardId(int)				FOCUS_FORWARD移動時の遷移先View
android:nextFocusLeft				setNextFocusLeftId(int)					FOCUS_LEFT移動時の遷移先View
android:nextFocusRight				setNextFocusRightId(int)				FOCUS_RIGHT移動時の遷移先View
android:nextFocusUp					setNextFocusUpId(int)					FOCUS_UP移動時の遷移先View
android:onClick						なし									クリック時のメソッド呼び出し
android:padding						setPaddingRelative(int,int,int,int)		パディングの設定
android:paddingBottom				setPaddingRelative(int,int,int,int)		パディング（下）の設定
android:paddingEnd					setPaddingRelative(int,int,int,int)		パディング（終端）の設定 
android:paddingLeft					setPadding(int,int,int,int)				パディング（左）の設定
android:paddingRight				setPadding(int,int,int,int)				パディング（右）の設定
android:paddingStart				setPaddingRelative(int,int,int,int)		パディング（始端）の設定 
android:paddingTop					setPaddingRelative(int,int,int,int)		パディング（上）の設定
android:requiresFadingEdge			setVerticalFadingEdgeEnabled(boolean)	フェード指定
android:rotation					setRotation(float)						Viewの回転角 
android:rotationX					setRotationX(float)						Viewの回転角 （X軸）
android:rotationY					setRotationY(float)						Viewの回転角 （Y軸）
android:saveEnabled					setSaveEnabled(boolean)					状態保存の有効判定
android:scaleX						setScaleX(float)						拡大率（X軸）
android:scaleY						setScaleY(float)						拡大率（Y軸）
android:scrollX						なし									スクロールの水平オフセット位置
android:scrollY						なし									スクロールの垂直オフセット位置
android:scrollbarAlwaysDrawHorizontalTrack		なし						水平方向の常時表示
android:scrollbarAlwaysDrawVerticalTrack		なし						垂直方向の常時表示
android:scrollbarDefaultDelayBeforeFade		setScrollBarDefaultDelayBeforeFade(int)		消え始めるまでの時間
android:scrollbarFadeDuration				setScrollBarFadeDuration(int)	消えるまでの時間
android:scrollbarSize				setScrollBarSize(int)					スクロールバーのサイズ
android:scrollbarStyle				setScrollBarStyle(int)					スクロールバーのスタイル指定
android:scrollbarThumbHorizontal	なし									水平方向のdrawable指定（色を変更）
android:scrollbarThumbVertical		なし									垂直方向のdrawable指定（色を変更）
android:scrollbarTrackHorizontal	なし									水平方向のdrawable指定（トラックを変更）
android:scrollbarTrackVertical		なし									垂直方向のdrawable指定（トラックを変更）
android:scrollbars					なし									スクロールバーの表示設定 
android:soundEffectsEnabled			setSoundEffectsEnabled(boolean)			効果音の設定
android:tag							なし									タグ名
android:textAlignment				setTextAlignment(int)					テキストの揃え 
android:textDirection				setTextDirection(int)					テキストの表示方向
android:transformPivotX				setPivotX(float)						反転方向（X軸）
android:transformPivotY				setPivotY(float)						反転方向（Y軸）
android:translationX				setTranslationX(float)					移動（X軸）
android:translationY				setTranslationY(float)					移動（Y軸）
android:visibility					setVisibility(int)						表示の初期状態
//}

ここで挙げたViewの属性値は、UIコンポーネント（TextViewやImageViewなど）でも共通です。
システムとして、共通のアクセシビリティ（ユーザー補助機能）、表示に関連したスクロール機能、レイアウト上必要となるパディング、
操作に必要なフォーカスなどが設定できます。
またViewを加工する回転、移動など表示に必要な設定項目を幅広く持っていることがわかります。

//cmd{
java.lang.Object
  ┗android.view.View
    ┗android.widget.TextView
//}
たとえばTextViewは、前述のクラス継承関係に基づいて、Viewを継承しています。
さらにTextView独自のメソッドやXML属性を持っています@<fn>{textview}。

//footnote[textview][http://developer.android.com/reference/android/widget/TextView.html]


UIコンポーネントのButtonをみてみると、TextViewを継承しているため、TextViewに加えてButton独自のメソッドを持っています@<fn>{button}。
//cmd{
java.lang.Object
  ┗android.view.View
    ┗android.widget.TextView
      ┗android.widget.Button
//}
このようにViewを継承し、機能を追加、設定してコンポーネントを構成しています。

//footnote[button][http://developer.android.com/reference/android/widget/Button.html]

=== ViewGroupの役割
FrameLayout@<fn>{frame}やRelativeLayout@<fn>{relative}といったレイアウトでは、
たくさんのViewを持ち、開発者の意図した場所に配置する役割を担っています。

そのための仕組みとしてViewGroupがあります。

//footnote[frame][http://developer.android.com/reference/android/widget/FrameLayout.html]
//footnote[relative][http://developer.android.com/reference/android/widget/RelativeLayout.html]

画面上に、たった１つのUIコンポーネントが存在しているという構成はあまりありません@<fn>{old}。
テキスト、画像、ボタンを配置して、画面を構成することが一般的です。

//footnote[old][CUIという環境も、まれに残っているんですけどね]

ViewGroupは、構成要素としてUIコンポーネントを持てる特殊なクラスです。
構成要素として持っているViewのことを子View（children）、保持している側を親（parent）と呼びます@<fn>{親子}。

//footnote[親子][またはツリー構造など関係の主従を意識した呼び方がされることがあります。しかし方言が多いので決まった言い方ではありません]

ここで、FrameLayoutのクラス継承関係をみてみましょう。
//cmd{
java.lang.Object
  ┗android.view.View
    ┗android.view.ViewGroup
      ┗android.widget.FrameLayout
//}

FrameLayoutが、ViewGroupを継承していることがわかります。
さらにViewを継承しているため、自分自身もViewであるといえます。
そのため、LayoutそのものをLayoutに内包することもでき、図のようなツリー構造で書き表せます（@<img>{layoutparams}@<fn>{layout}）。

//image[layoutparams][レイアウトの構造]{
//}

//footnote[layout][http://developer.android.com/guide/topics/ui/declaring-layout.html]

このような構造をツリー構造と呼んだり、ViewがViewのなかに入ることから、入れ子構造と呼んだりします（@<img>{Layouts}）。

//image[Layouts][レイアウトはViewを複数保持できる]{
//}

それぞれのViewが、お互いの位置関係を把握、自分の表示位置を計算するという順序で描画されます。

 1. Viewの位置、内容を計測する
 2. Viewを実際に描画する
 
これらの描画処理に関して、最終的な責任は、Activityが持っています。
Viewのツリー構造のroot（最も重要なベース）として、
表示するレイアウトやViewを切り替える責務を持っています。
端的にこの部分を表しているソースコードが次のサンプルコードです（@<list>{03}）。

//list[03][ActivityのonCreateメソッド]{
public class MainActivity extends Activity {
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
    }
}
//}

これはActivityの最小コードです。開発者は、ActivityクラスのsetContentViewメソッドをよく目にしていますが、
ここまでの知識から、このContentViewは表示対象となるViewを指定するためのメソッドであり、
Viewであればどんなものでも設定可能だ、ということがわかります。

サンプルコードではR.layout.activity_mainを指定してXMLから生成していますが、Viewであれば何でもよいため次のように直接生成しても構いません（@<list>{04}）。

//list[04][LayoutInflaterを利用してViewを生成する]{
public class MainActivity extends Activity {
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        
        View v = getLayoutInflater()
                .inflate(R.layout.activity_main, null);
        setContentView(v);
    }
}
//}

このサンプルコードは@<list>{03}のようなスマートな書き方ではありませんが、
LayoutがViewであることを示すための実証コードです。

ActivityクラスのgetLayoutInflaterメソッドは、XMLからLayout（LayoutはすべてViewを親クラスに持つ）を生成します。
生成したViewをsetContentViewメソッドで指定しているため、@<list>{03}と@<list>{04}は同じ意味を持ちます。

ViewGroupのなかにViewを持つ構造を示すソースコードをもうひとつ例示します（@<list>{05}）。

//list[05][findViewByIdメソッド]{
public class MainActivity extends Activity {
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        
        Button b = (Button) findViewById(R.id.button);
        b.setOnClickListener(new OnClickListener() {
            ...省略...
        });
    }
}
//}

このサンプルコードもよく見るソースコードです。
setContentViewメソッドでレイアウトを生成したあとに、findViewByIdメソッドでボタンを取得しています。

現時点の知識をベースに解説すると、
findViewByIdメソッドは、「ActivityをルートとするViewGroupのなかから指定のIDのViewを探してくる」という表現が可能です。

このことを理解しておくと、開発者がうっかりやってしまう、
次のようなViewのIDが見つからないエラーも、原因を説明できるようになります（@<list>{06}）。

//list[06][setContentViewメソッドよりはやくfindViewByIdメソッドを利用できない]{
public class MainActivity extends Activity {
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        Button b = (Button) findViewById(R.id.button);
        setContentView(R.layout.activity_main);
        
        b.setOnClickListener(new OnClickListener() {
            ...省略...
        });
    }
}
//}
このようなケースでは、findViewByIdメソッドはnullを返却します。

setContentViewメソッドによってレイアウトを生成する前のため、R.id.buttonをもつViewが見つかりません。
このまま実行すると次の通り、NullPointerExceptionが発生します。

//cmd{
08-31 06:11:16.400: E/AndroidRuntime(914): FATAL EXCEPTION: main
08-31 06:11:16.400: E/AndroidRuntime(914): Process: com.example.ui.sample00, PID: 914
08-31 06:11:16.400: E/AndroidRuntime(914): java.lang.RuntimeException: 
    Unable to start activity ComponentInfo{com.example.ui.sample00/
    com.example.ui.sample00.MainActivity}: java.lang.NullPointerException
//}

この場合はsetContentViewメソッドのまえにfindViewByIdメソッドを使い、
期待する動作を得られなかったため、NullPointerExceptionが発生していました@<fn>{view}。

//footnote[view][findViewByIdメソッドはActivityのみではなく、Viewクラスにも存在しています]

初心者にとっては、ViewのIDが見つからないというのは、
恐怖の対象でしかありませんでしたが、理由がわかれば迷うことなく、修正できます。

ViewとViewGroupの関係を正しく把握することがUIを把握する手助けとなるでしょう。


=== Viewをカスタマイズする
Viewクラスを利用することで、独自機能を追加できます。
カスタマイズのサンプルとして、Viewにペイント機能を追加してみましょう。

サンプルアプリケーションは次の図のとおりです（@<img>{draw}）。

//image[draw][ペイントアプリ]{
//}

今回、作成するアプリではViewの機能を拡張します。
タッチイベントを取得して、指の動きに合わせて線を描けるようにします。

==== CustomView.javaファイルを作成する

Viewクラスを継承した新しいCustomViewクラスを作成していきます。
Javaのファイルを新規に作成しましょう。Viewを継承するクラス名に応じてファイル名を決定します。

サンプルコードでは、CustomView.javaで統一しています（@<list>{07}）。
新規でクラスを作る場合でも、TextViewやImageViewのように、「～View」をつけることで、
どのような役割をもっているのか一目でわかりやすくする慣習があります。

//list[07][CustomView.java]{
package jp.androidopentextbook.ui.sample01_draw;

import android.content.Context;
import android.view.View;

class CustomView extends View {
    //コンストラクタ
    public CustomView(Context context) {
        super(context);
        // 何もしません
    }
//}

現時点のサンプルコードでは、コンストラクタが１つですが、Viewを作る際は、３つ用意します。

 * public View(Context context)
 * public View(Context context, AttributeSet attrs)
 * public View(Context context, AttributeSet attrs, int defStyle)

それぞれのコンストラクタには意味があり、用途に応じて使い分けられます。
XMLから参照される場合は、レイアウトパラメータであるAttributeSetが必要です。

setContentView(R.layout.activity_main)のように、XMLファイルから参照する場合は、
前述のコンストラクタのうち２番目のAttributeSetを持つコンストラクタが呼び出されます。

またスタイルを指定した場合はdefStyleを持つ３つめのコンストラクタが使われます。

今回は、activity_main.xmlで定義するので、public View(Context context, AttributeSet attrs)を実装します（@<list>{08}）。

//list[08][CustomView.java - コンストラクタの実装]{
class CustomView extends View {

    //コンストラクタ
    public CustomView(Context context) {
        super(context);
        //何もしません
    }

    //スタイルを適用する際のコンストラクタ
    public CustomView(Context context, AttributeSet attrs, int defStyle) {
        super(context, attrs, defStyle);
        //何もしません
    }

    //XMLより呼び出す際のコンストラクタ
    public CustomView(Context context, AttributeSet attrs)
    {
        super(context, attrs);
        setFocusable(true);
        initPaint();
    }

    // 描画用ビットマップ、キャンパス、パス、ペイント設定
    private Bitmap mBitmap;
    private Canvas mCanvas;
    private Path mPath;
    private Paint mPaint; //

    // 描画用Paintの初期化
    private void initPaint() {
    }
//}
コンストラクタでは、setFocusableメソッドを使ってフォーカスを有効にします。
ここでは中身はありませんが、ペイント機能のための初期化メソッドもあらかじめ、用意します。

==== レイアウトファイルに追加する

ここまでで最小限Viewとして動作するようになりました。
activity_main.xmlには、次のようにCustomViewを追加します（@<list>{09}）。

//list[09][activity_main.xml]{
<RelativeLayout xmlns:android="http://schemas.android.com/apk/res/android"
    xmlns:tools="http://schemas.android.com/tools"
    android:layout_width="match_parent"
    android:layout_height="match_parent"
    android:paddingBottom="@dimen/activity_vertical_margin"
    android:paddingLeft="@dimen/activity_horizontal_margin"
    android:paddingRight="@dimen/activity_horizontal_margin"
    android:paddingTop="@dimen/activity_vertical_margin"
    tools:context=".MainActivity" >

    <jp.androidopentextbook.ui.sample01_draw.CustomView
        android:id="@+id/paintView"
        android:layout_width="match_parent"
        android:layout_height="match_parent" />

</RelativeLayout>
//}
通常であればImageViewであったり、TextView、ButtonなどUIパーツ名であったりしますが、
ここではパッケージ名を含めてクラス名を記載します。

参照を間違えている場合、ClassNotFoundExceptionが発生します。その場合は、Logcatに次のようなエラーログが出力されます。

//cmd{
08-31 07:51:50.920: E/AndroidRuntime(855): 
  Caused by: java.lang.ClassNotFoundException: Didn't find class
  "android.view.CustomView" on path: DexPathList[[zip file
  "/data/app/jp.androidopentextbook.ui.sample01_draw-1.apk"],
  nativeLibraryDirectories=[/data/app-lib/
  jp.androidopentextbook.ui.sample01_draw-1, /system/lib]]
//}

エラーがある場合は、Didn't find classに続き、"android.view.CustomView"など、
誤ったクラス名が書かれているため、改めて確認しなおしてください。

==== ペインタ機能の実装

ペインタ機能を実装するにあたって、Canvasの用意とPaintなどリソースの初期化を行います。
Canvasクラスは、描いた内容が消えないように描画する内容を保存します。

Paintはどのような線を引くか、スタイルや色などリソースを保存するクラスです（@<list>{10}）。

//list[10][CustomView.java - 初期化と更新処理]{
class CustomView extends View {
    ...省略...
    
    // 描画用ビットマップ、キャンパス、パス、ペイント設定
    private Bitmap mBitmap;
    private Canvas mCanvas;
    private Path mPath;
    private Paint mPaint;

    // 描画用Paintの初期化
    private void initPaint() {
        mPath = new Path();

        mPaint = new Paint();
        mPaint.setAntiAlias(true);
        mPaint.setDither(true);
        mPaint.setColor(Color.RED);
        mPaint.setStyle(Paint.Style.STROKE);
        mPaint.setStrokeJoin(Paint.Join.ROUND);
        mPaint.setStrokeCap(Paint.Cap.ROUND);
        mPaint.setStrokeWidth(12);
    }

    @Override
    protected void onSizeChanged(int w, int h, int oldw, int oldh) {
        // 画面サイズ変更時の通知
        Log.v("View", "onSizeChanged Width:" + w + ",Height:" + h);

        // Canvasを作成
        mBitmap = Bitmap.createBitmap(w, h, Bitmap.Config.ARGB_8888);
        mCanvas = new Canvas(mBitmap);
    }
    
    @Override
    protected void onDraw(Canvas canvas) {
        // Viewの描画関数で、パスを描画する
        canvas.drawBitmap(mBitmap, 0, 0, mPaint);
        canvas.drawPath(mPath, mPaint);
    }
    ...省略...
}
//}

initPaintメソッドを変更して、PaintとPathを初期化します。おなじinitPaintメソッド内で線の種類、色、サイズなどを設定します。

onSizeChangedメソッドはViewサイズが変更された場合に呼び出されるメソッドです。
Viewのサイズが確定するタイミングでもあるので、サンプルではBitmapのサイズをここで決めています。

onDrawメソッドはViewの描画メソッドです。AndroidのViewシステムから呼び出され、Viewの表示内容を更新します。
CanvasクラスのdrawPathメソッドで指の動きを描画して画面へ反映します
（指の動きはCustomViewのメンバ変数mPathに保存しています）。

ペイント機能の基本操作となる描画部分は、タッチイベントを受け取るonTouchEventメソッドです。
指でスクリーンをタッチしたら描き始め、離すと描画を終了します（@<list>{11}）。

//list[11][CustomView.java - タッチイベント]{
class CustomView extends View {
    ...省略...
    @Override
    public boolean onTouchEvent(MotionEvent event) {
        float x = event.getX();
        float y = event.getY();

        switch (event.getAction()) {
            case MotionEvent.ACTION_DOWN:
                touch_start(x, y);
                break;
            case MotionEvent.ACTION_MOVE:
                touch_move(x, y);
                break;
            case MotionEvent.ACTION_UP:
                touch_up();
                break;
        }
        invalidate();//Viewの再描画
        return true;
    }
    ...省略...
}
//}
onTouchEventメソッドでは、ACTION_DOWNとACTION_UPをトリガーに、押下中状態(MOVE)で線を引く処理を実装しています。
invalidateメソッドは、再描画を指示するメソッドです。
invalidateメソッドを通じてonDrawメソッドが呼び出されます@<fn>{on}。
サンプルでは簡単化のために座標に動きがある場合は必ず再描画しています。

//footnote[on][onDraw、onTouchEventなどメソッド名にonXXXがつくメソッドはAndroidのフレームワークが実行する前提であることがほとんどです]

最後にタッチ開始・移動・終了の内部処理です。
次に解説するサンプルコードは、線を描画するために独自に追加したメソッドです（@<list>{12}）。

//list[12][CustomView.java - 描画処理]{
class CustomView extends View {
    ...省略...
    private float mX, mY;
    private static final float TOUCH_TOLERANCE = 4;

    private void touch_start(float x, float y) {
        //タッチ開始時
        Log.v("View", "touch_start");
        mPath.reset();
        mPath.moveTo(x, y);
        mX = x;
        mY = y;
    }

    private void touch_move(float x, float y) {
        //指が移動している間の処理
        Log.d("View", "touch_move");
        float dx = Math.abs(x - mX);
        float dy = Math.abs(y - mY);
        //閾値より移動量が多ければ線をつなぐ
        if (dx >= TOUCH_TOLERANCE || dy >= TOUCH_TOLERANCE) {
            mPath.quadTo(mX, mY, (x + mX) / 2, (y + mY) / 2);
            mX = x;
            mY = y;
        }
    }

    private void touch_up() {
        //タッチ終了時
        Log.v("View", "touch_up");
        //線を描く
        mPath.lineTo(mX, mY);
        mCanvas.drawPath(mPath, mPaint);
        mPath.reset();
    }
    ...省略...
}
//}
タッチによる描画のために、指の動きをmPathとして保存しています。
理想的には、受け取った値をそのまま使えばよいのですが、タッチイベントは、
誤差なども含むため、X座標、Y座標の値を確認しつつ、移動量を補正して線を引いています。

== UIコンポーネントの見た目をカスタマイズする

ここでは、UIコンポーネントの見た目を変更する手法について解説します。
Androidの標準UIコンポーネントを利用することには、アプリケーションごとの操作性にとまどわない利点があります。
新しくインストールしたアプリケーションであったとしても、見慣れたUIを操作できるわけです。

しかし、提供するサービスのブランドを考慮した結果、標準コンポーネントは使われないケースも増えています。
多くのアプリケーションで、標準UIコンポーネントの見た目を変えて、
デザイン性を高めるべくカスタマイズしています（@<img>{setting}、@<img>{wantedly}）。

//image[setting][Android標準の設定アプリ]{
//}

//image[wantedly][アプリケーション独自のカスタマイズ例（Wantedly）]{
//}

Android標準の設定アプリでは、黒を基調とした落ち着いた見た目です。
Wantedlyアプリをみてみると、白を基調とした見た目になっており、清潔感があります。

ボタンなどUIコンポーネントはフラットデザインが採用されており、すっきりとしたかたち@<fn>{flat}でまとめられています。
このようにアプリケーションのブランド、コンセプトに合わせて全体のデザインを変更すると、統一感を得られます。

//footnote[flat][現在のところ、モバイルアプリケーションの分野では、フラットデザインが主流になっています]

=== Shape Drawableを利用する
AndroidではXMLを使って図形を作成できます。

形状に対していくつかの属性を指定することで、簡単な図形であれば、画像を用意せずに作れます。
また画像と異なり拡大縮小に強く、ファイルサイズも小さい特徴があります。解像度が異なる場合も劣化はありません@<fn>{vector}。

//footnote[vector][画像と、Shape Drawableの差は、ラスターデータとベクターデータの差といえます]

Androidのリソースには、いくつかの種類がありますが、Shape Drawableは名前の通り、drawableとして扱われます。

 * animator/：プロパティアニメーション
 * anim/	：ビューアニメーション
 * color/	：色の定義
 * drawable/：画像、描画リソース
 * layout/	：レイアウトファイル
 * menu/	：メニュー項目
 * raw/		：ストリーム
 * values/	：テキスト、スタイル
 * xml/		：そのほかプログラムで利用するXML

Shape Drawableファイルは次の場所に配置します。

//cmd{
res/drawable/filename.xml
//}
このとき、filenameがリソースIDとして扱われます。
Shape Drawableの文法は次の通りです（@list{13}@<fn>{shape-element}）。

//list[13][Shape Drawableの文法]{
<?xml version="1.0" encoding="utf-8"?>
<shape
    xmlns:android="http://schemas.android.com/apk/res/android"
    android:shape=["rectangle" | "oval" | "line" | "ring"] >
    <corners
        android:radius="integer"
        android:topLeftRadius="integer"
        android:topRightRadius="integer"
        android:bottomLeftRadius="integer"
        android:bottomRightRadius="integer" />
    <gradient
        android:angle="integer"
        android:centerX="integer"
        android:centerY="integer"
        android:centerColor="integer"
        android:endColor="color"
        android:gradientRadius="integer"
        android:startColor="color"
        android:type=["linear" | "radial" | "sweep"]
        android:useLevel=["true" | "false"] />
    <padding
        android:left="integer"
        android:top="integer"
        android:right="integer"
        android:bottom="integer" />
    <size
        android:width="integer"
        android:height="integer" />
    <solid
        android:color="color" />
    <stroke
        android:width="integer"
        android:color="color"
        android:dashWidth="integer"
        android:dashGap="integer" />
</shape>
//}

//footnote[shape-element][http://developer.android.com/guide/topics/resources/drawable-resource.html]

Shape Drawableでは、四角、楕円、線、円の4つの形状から選択できます。
android:shape属性の取り得る値は、次の通りです。

 * rectangle：四角形
 * oval：楕円形
 * line：線
 * ring：円

corners要素では、左上、右上、左下、右下の角に対して半径を設定できます。丸角にする場合に利用します。
corners要素は次の属性値を持ち、細かく角の半径を指定可能です。

 * android:radius：すべての角に対して半径を指定
 * android:topLeftRadius：左上の角半径
 * android:topRightRadius：右上の角半径
 * android:bottomLeftRadius：左下の角半径
 * android:bottomRightRadius：右下の角半径


gradient要素ではグラデーションを設定できます。

 * android:angle：グラデーション方向
 * android:centerX：中心位置（X座標）
 * android:centerY：中心位置（Y座標）
 * android:startColor：始端の色
 * android:endColor：終端の色
 * android:centerColor：中間色
 * android:gradientRadius：半径を指定
 * android:type：グラデーションのパターン
 * android:useLavel：LevelListDrawableを利用する

android:angle属性値は、グラデーションの角度ですが0は左から右、90は下から上方向です。
角度は、45の倍数でなければ機能しません。

padding要素では上下左右のパディングを設定できます。

 * android:left：左方向のパディング
 * android:top：上方向のパディング
 * android:right：右方向のパディング
 * android:bottom：下方向のパディング

size要素では大きさを設定できます。

 * android:height：高さ
 * android:width：横幅
 
solid要素では色を設定できます。使える色は１色のみです。

 * android:color：指定色

stroke要素で枠線の形状を設定できます。

 * android:width：枠線の太さ
 * android:color：枠線の色
 * android:dashGap：点線の間隔
 * android:dashWidth：点線の幅

Shape Drawableでは、これらの要素をつかいわけてDrawableを作成します。
また要素はそれぞれ単純な図形、ルールで構成されています。

複雑な形状は苦手なため、ある程度、シンプルに記号化したり、色の変化でバリエーションを持ったりして、
アプリケーションのコンセプトに合わせます。

=== ボタンをカスタマイズする
Shape Drawableをつかってボタンを変更してみましょう。
ここでは画像ではなく、XMLをつかってボタンの背景を作成します（@<img>{shape}）。

//image[shape][標準ボタンとカスタマイズしたボタン]{  
//}
標準のUIコンポーネントを使っている場合、Android端末ごとのテーマやAndroid OSごとに、統一されたデザインが適用されます。
一方、ボタンをカスタマイズした場合は、常にアプリケーションの意図したデザインで変化はありません。

ボタンにはいくつかの状態があり、それらを管理するためにState List@<fn>{state}という仕組みを利用します。
State Listの文法を確認してみましょう（@<list>{14}）

//footnote[state][http://developer.android.com/guide/topics/resources/drawable-resource.html#StateList]

//list[14][State Listの文法]{
<?xml version="1.0" encoding="utf-8"?>
<selector xmlns:android="http://schemas.android.com/apk/res/android"
    android:constantSize=["true" | "false"]
    android:dither=["true" | "false"]
    android:variablePadding=["true" | "false"] >
    <item
        android:drawable="@[package:]drawable/drawable_resource"
        android:state_pressed=["true" | "false"]
        android:state_focused=["true" | "false"]
        android:state_hovered=["true" | "false"]
        android:state_selected=["true" | "false"]
        android:state_checkable=["true" | "false"]
        android:state_checked=["true" | "false"]
        android:state_enabled=["true" | "false"]
        android:state_activated=["true" | "false"]
        android:state_window_focused=["true" | "false"] />
</selector>
//}
State Listでは、selector要素とitem要素が必要です。ボタンが押されている、フォーカスがあるなど状態にあわせて
表示するDrawableを切り替えます。

 * android:drawable：Drawableリソースの指定
 * android:state_pressed：押下
 * android:state_focused：フォーカス
 * android:state_hovered：ホバー
 * android:state_selected：選択状態
 * android:state_checkable：チェック可能
 * android:state_checked：チェック
 * android:state_enabled：有効／無効
 * android:state_activated：アクティベート
 * android:state_window_focused：Windowフォーカス

ボタンの場合、android:state_pressed属性があれば、最低限の動作が可能です。
実際にはandroid:state_pressed、android:state_focused、android:state_enabledの3つを組み合わせる必要があります。

サンプルとして、ボタンのState Listを作ってみます（@<list>{15}）。
//list[15][res/drawable/button_mycolor.xml]{
<?xml version="1.0" encoding="utf-8"?>
<selector
    xmlns:android="http://schemas.android.com/apk/res/android">
 
    <!-- ボタンが押されたときの定義 -->
    <item android:state_pressed="true">
        ...省略...
    </item>
 
    <!-- ボタンが押されていないときの定義 -->
    <item android:state_pressed="false">
        ...省略...
    </item>
</selector>
//}
省略した部分はShape Drawableです（後述）。
State Listは、selector要素がitem要素を持ち、それぞれの状態を定義します。

ボタンが押されたときのShape Drawableは、次のとおりです（@<list>{16}）。
//list[16][res/drawable/button_mycolor.xml - ボタンが押されたとき]{
<?xml version="1.0" encoding="utf-8"?>
<selector
    xmlns:android="http://schemas.android.com/apk/res/android">
 
    <!-- ボタンが押されたときの定義 -->
    <item android:state_pressed="true">
        <solid android:color="#ffffff" />
        <stroke android:color="#01579b" android:width="2dp" />
        <corners android:radius="2dp" />
        <padding android:left="10dp"
            android:top="10dp"
            android:right="10dp"
            android:bottom="10dp" />
        <size android:height="30dp"
            android:width="30dp" />
    </item>
    ...省略...
</selector>
//}

ボタンが押されていないときのShape Drawableは次のとおりです（@<list>{17}）。
//list[17][res/drawable/button_mycolor.xml - ボタンが押されていないとき]{
<?xml version="1.0" encoding="utf-8"?>
<selector
    xmlns:android="http://schemas.android.com/apk/res/android">
     ...省略...
    <!-- ボタンが押されていないときの定義 -->
    <item android:state_pressed="false">
        <shape android:shape="rectangle">
            <solid android:color="#ffffff" />
            <stroke android:color="#03a9f4" android:width="2dp" />
            <corners android:radius="2dp" />
            <padding android:left="10dp"
                android:top="10dp"
                android:right="10dp"
                android:bottom="10dp" />
            <size android:height="30dp"
                android:width="30dp" />
        </shape>
    </item>
</selector>
//}

サンプルのボタンが押されたとき、押されていないときの違いは枠線（stroke）の色です。
どちらも共通して背景色（solid）を白、丸角（corners）2dp、パディングを10dp、サイズを30dpで設定しています。

大きく見た目が変わるとユーザーが驚くため、このサンプルコードでは意図的に変化を少なくしています。

ボタンを押した状態での見た目の変化には、ユーザーの操作に対するレスポンスにあたるため、注意が必要です。

このような場合は、ユーザーが想像できる範囲にShape Drawableの変化を納めると綺麗にみえます。
枠線ではなく背景色を変えても、分かりやすいボタンになるでしょう。

作成したDrawable（State Listを使ったオリジナルのボタン）を利用するにはレイアウトを次のように変更します（@<list>{18}）。

//list[18][res/layout/activity_main.xml]{
<RelativeLayout xmlns:android="http://schemas.android.com/apk/res/android"
    xmlns:tools="http://schemas.android.com/tools"
    android:layout_width="match_parent"
    android:layout_height="match_parent"
    android:paddingBottom="@dimen/activity_vertical_margin"
    android:paddingLeft="@dimen/activity_horizontal_margin"
    android:paddingRight="@dimen/activity_horizontal_margin"
    android:paddingTop="@dimen/activity_vertical_margin"
    tools:context=".MainActivity" >

    <Button
        android:id="@+id/button1"
        android:layout_width="match_parent"
        android:layout_height="wrap_content"
        android:layout_alignParentTop="true"
        android:layout_centerHorizontal="true"
        android:layout_marginBottom="20dp"
        android:text="Button" />

    <Button
        android:id="@+id/button2"
        android:layout_width="match_parent"
        android:layout_height="wrap_content"
        android:layout_alignLeft="@+id/button1"
        android:layout_below="@+id/button1"
        android:background="@drawable/button_mycolor"
        android:layout_marginBottom="20dp"
        android:text="Button" />

</RelativeLayout>
//}
ボタンの背景（android:background属性）に作成したDrawableを指定すると、見た目に反映されます。
サンプルアプリを動かして確認してみましょう。

また応用として、丸いボタンを作る場合のサンプルコードは、次のとおりです（@<list>{19}）。

//list[19][res/drawable/button_oval.xml]{
<?xml version="1.0" encoding="utf-8"?>
<selector xmlns:android="http://schemas.android.com/apk/res/android" >
    <item android:state_pressed="true">
        <shape android:shape="oval">
            <solid android:color="#01579b" />
        </shape>
    </item>

    <item android:state_pressed="false">
        <shape android:shape="oval">
            <solid android:color="#03a9f4" />
        </shape>
    </item>
</selector>
//}
状態にあわせてShape Drawableの背景を変更しています。
ここではandroid:color属性値を直値で入力していますが、color.xmlに定義する方法も利用できます。

作ったUIコンポーネントを修正しないケースは、まずありません。
出来上がったパーツを少し変更したい、色味を確認したいなど、細かな修正が必ず発生します。
その際、リソースを分割して管理すると変更箇所が少なくなり効率があがります。

button_mycolor.xmlの別解として、次のように書くこともできます（@<list>{20}）。

//list[20][res/drawable/button_mycolor.xml]{
<?xml version="1.0" encoding="utf-8"?>
<selector
    xmlns:android="http://schemas.android.com/apk/res/android">
 
    <!-- ボタンが押されたときの定義 -->
    <item android:state_pressed="true">
        <shape android:shape="rectangle">
            <stroke android:color="#01579b" android:width="2dp" />
        </shape>
    </item>
 
    <!-- ボタンが押されていないときの定義 -->
    <item android:state_pressed="false">
        <shape android:shape="rectangle">
            <stroke android:color="#03a9f4" android:width="2dp" />
        </shape>
    </item>
    
    <item>
        <shape android:shape="rectangle">
            <solid android:color="#ffffff" />
            <stroke android:color="#01579b" android:width="2dp" />
            <corners android:radius="2dp" />
            <padding android:left="10dp"
                android:top="10dp"
                android:right="10dp"
                android:bottom="10dp" />
            <size android:height="30dp"
                android:width="30dp" />
        </shape>
    </item>
</selector>
//}
ここでは差分のあるstroke要素のみを取り出して定義しています。
共通部分を属性を持たないitem要素にまとめて、記述することでXMLのコード記述量が減らせます。

== アニメーション
リソースが限られていた時代、アニメーションは必須の要素ではありませんでした。
すべての状態は静止画で表現され、画面のデザインは静的なものでした。
しかし、モバイルOSにおいて、アニメーションの果たす役割は大きく変わりつつあります。

ListViewを例にあげると、Itemの追加時にフェードをかければ、コンテンツの増加を意識させることができます。
またスクロール下端の場合、エフェクトをかけることで、
コンテンツが無いことを明示できるなどユーザー操作を補助する役割を担っています。

Googleが発表したMaterial Designでは、アニメーションは、
ユーザーの操作に対するフィードバックとして強く期待されています@<fn>{material}。

//footnote[material][https://www.google.com/design/]

アニメーションのタイプは大きく2つに分類できます。
Frameアニメーション（AnimationDrawable）とTweenアニメーション（Animation）です。
それぞれ次のような特徴があります。

 * Frameアニメーション：順番にイメージを並べて表示してアニメーションにするタイプ。順番に表示させる画像をあらかじめ用意して、パラパラ漫画のように表示します
 * Tweenアニメーション：1つのイメージを連続に変化させるタイプ。移動、フェード、回転、拡大縮小、それらを組み合わせて変化させます


=== Frameアニメーション
Frameアニメーション（AnimationDrawable）を使うと、XMLでリソースを用意するだけで簡単にアニメーションが作成できます。
時間ごとに切り替えてアニメーションを実現するパラパラ漫画のような手法です。
今回は次の画像を事前に用意して、回転アニメーションを実現します（@<img>{android_logo}）。

//image[android_logo][用意する画像一覧]{
//}

今回は6枚の画像を連続表示して回転しているように見えるアニメーションを作成します（@<img>{animationdrawable}）。

//image[animationdrawable][Frameアニメーションのサンプル]{
//}

Frameアニメ―ションに使うリソースファイルの保存場所は次の通りです。

//cmd{
res/drawable/filename.xml
//}

今回はXMLでアニメーションの設定を行うのでdrawableの下にXMLも一緒に配置します（@<img>{res}）。

//image[res][リソースの配置位置]{
//}

新規に作成するXMLファイル名はandroid_animation.xmlです。

//list[21][android\animation.xml]{
<?xml version="1.0" encoding="utf-8"?>
<animation-list xmlns:android="http://schemas.android.com/apk/res/android"
     android:oneshot="false">
    <item android:drawable="@drawable/android_logo" android:duration="50" />
    <item android:drawable="@drawable/android_logo2" android:duration="50" />
    <item android:drawable="@drawable/android_logo3" android:duration="50" />
    <item android:drawable="@drawable/android_logo4" android:duration="50" />
    <item android:drawable="@drawable/android_logo5" android:duration="50" />
    <item android:drawable="@drawable/android_logo6" android:duration="50" />
 </animation-list>
//}
animation-list要素がitem要素を持ち、1枚のFrameを表します。6枚分をすべて登録します。
android:duration属性はミリ秒（ms単位）で指定できます。サンプルでは50ミリ秒です。
android:oneshot属性がfalseのときは、リピート再生を意味します（trueで１度のみアニメーションを行います）。

animation-listの持つ属性は次のとおりです（@<table>{animation-list}）。

//table[animation-list][animation-list要素の属性一覧]{
属性	説明
-------------
android:drawable			drawableリソースへの参照（１フレーム分）
android:duration			アニメーションの間隔（ms単位）
android:oneshot				繰り返し設定
android:variablePadding		パディングを可変にする
android:visible				初期表示状態
//}

アニメーションを開始するためのサンプルコードは次のとおりです（@<list>{22}）。
実行のためには、事前にレイアウトファイル（activity_main.xml）にImageViewを用意してください。

//list[22][MainActivity.java]{
public class MainActivity extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
    }

    @Override
    public void onWindowFocusChanged(boolean hasFocus) {
     super.onWindowFocusChanged(hasFocus);
     ImageView img = (ImageView)findViewById(R.id.imageView);
     
     // AnimationDrawableのXMLリソースを指定
     img.setBackgroundResource(R.drawable.android_animation);
     // AnimationDrawableを取得
     AnimationDrawable frameAnimation = (AnimationDrawable) img.getBackground();
     // アニメーションの開始
     frameAnimation.start();
    }
}
//}
AnimationDrawableをImageViewの背景に設定しています。
注意すべきは、AnimationDrawableクラスのstart()メソッドはonCreateメソッドでは有効にならないことです。
onCreateメソッドで呼び出してもアニメーションしません。
画面表示の準備が完了するまでは無視されるためです。
このサンプルのようにonWindowFocusChangedメソッド（画面描画の準備が完了した段階）で呼び出します。

=== Tweenアニメーション
１つのViewを連続的に変化させるTweenアニメーションを解説します。
アニメーションの動きはXMLとソースコード、どちらでも定義できますが、
多くの場合、アニメーションがプログラマブル（可変）である必要はありません。
XMLで静的に保持したほうがメンテナンス性が高いでしょう
。
ここでは、Viewに対して透明な状態からフェードインするTweenアニメーションを考えます（@<img>{animation}）。

//image[animation][サンプルアプリ画面]{
//}

TweenアニメーションのXMLファイルは、次のとおり、anim以下に配置します。

//cmd{
res/anim/filename.xml
//}

文法は次のとおりです（@<list>{23}@<fn>{tween}）。

//footnote[tween][http://developer.android.com/guide/topics/resources/animation-resource.html#Tween]

//list[23][Tweenアニメーションの文法]{
<?xml version="1.0" encoding="utf-8"?>
<set xmlns:android="http://schemas.android.com/apk/res/android"
    android:interpolator="@[package:]anim/interpolator_resource"
    android:shareInterpolator=["true" | "false"] >
    <alpha
        android:fromAlpha="float"
        android:toAlpha="float" />
    <scale
        android:fromXScale="float"
        android:toXScale="float"
        android:fromYScale="float"
        android:toYScale="float"
        android:pivotX="float"
        android:pivotY="float" />
    <translate
        android:fromXDelta="float"
        android:toXDelta="float"
        android:fromYDelta="float"
        android:toYDelta="float" />
    <rotate
        android:fromDegrees="float"
        android:toDegrees="float"
        android:pivotX="float"
        android:pivotY="float" />
    <set>
        ...
    </set>
</set>
//}
TweenアニメーションをXMLファイルで記述する場合、
set、alpha、scale、translate、rotateいずれかの要素を持っている必要があります。

set要素やその他のアニメーション要素に使える属性は次のとおりです（@<table>{set}）。

//table[set][objectAnimator要素の属性一覧]{
属性	説明
------------
android:detachWallpaper	壁紙のアニメーションを停止する
android:duration		実行時間を指定
android:fillAfter		アニメーションが終わった後の状態を保つ
android:fillBefore		アニメーションが始まる前の状態を保つ
android:fillEnabled		fillAfter、fillBeforeを有効化する
android:interpolator	動きをスムーズに補完する
android:repeatCount		繰り返し回数
android:repeatMode		繰り返し動作設定（標準はループ）
android:startOffset		開始までのオフセット時間
android:zAdjustment		コンテンツ順序を奥行きで調整
//}
これらの属性値はset、alpha、scale、translate、rotateいずれの要素でも有効です。

alpha要素の属性は次のとおりです（@<table>{alpha}）。alpha要素は、透明度を変更する要素です。

//table[alpha][alpha要素の属性一覧]{
属性	説明
------------
android:fromAlpha	開始時の透明度
android:toAlpha		終了時の透明度
//}

scale要素の属性は次のとおりです（@<table>{scale}）。scale要素は、Viewの拡大・縮小を行う要素です。

//table[scale][scale要素の属性一覧]{
属性	説明
------------
android:fromXScale	開始時の拡大率
android:toXScale	終了時の拡大率
android:fromYScale	開始時の拡大率
android:toYScale	終了時の拡大率
android:pivotX		拡大の中心となる座標
android:pivotY		拡大の中心となる座標
//}

translate要素の属性は次のとおりです（@<table>{translate}）。translate要素は、Viewを水平または垂直方向へ移動する要素です。

//table[translate][translate要素の属性一覧]{
属性	説明
------------
android:fromXDelta	開始時のオフセット
android:toXDelta	終了時のオフセット
android:fromYDelta	開始時のオフセット
android:toYDelta	終了時のオフセット
//}

rotate要素の属性は次のとおりです（@<table>{rotate}）。rotate要素は、Viewを回転する要素です。

//table[rotate][rotate要素の属性一覧]{
属性	説明
------------
android:fromDegrees	開始時のオフセット
android:toDegrees	終了時のオフセット
android:pivotX		回転の中心となる座標
android:pivotY		回転の中心となる座標
//}

Viewに対してアニメーションを設定する場合、これらの要素、属性を利用してアニメーション用のXMLを作成します（@<list>{24}）。

//list[24][res/anim/motion.xml]{
<?xml version="1.0" encoding="utf-8"?>
<set xmlns:android="http://schemas.android.com/apk/res/android"
    android:interpolator="@android:anim/accelerate_interpolator"
    android:duration="5000">
    <alpha
        android:fromAlpha="0.0"
        android:toAlpha="1.0" />
</set>
//}
サンプルコードでは、透明な状態からフェードインしてViewが表示されます。
set要素でアニメーション時間を5秒と設定しているため、非常にゆっくり画面に表示されます。

作成したアニメーションXMLファイルはそのままでは適用されないため、ソースコード上で設定が必要です（@<list>{25}）。

//list[25][MainActivity.java]{
public class MainActivity extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        
        ImageView img = (ImageView) findViewById(R.id.imageView);
        //Tweenアニメーションの適用
        Animation animation= AnimationUtils.
                loadAnimation(this,R.anim.motion);
        img.startAnimation(animation);
    }
}
//}
ImageViewに対して、startAnimationメソッドを使ってTweenアニメーションを設定しています。
Tweenアニメーションそのものは、AnimationUtilsクラスのloadAnimationメソッドを使って、
リソースファイル（motion.xml）から生成しています。

アニメーションは、View（レイアウトを含む）の表示時、終了時などユーザー操作に応じて利用してください。
画面内で勝手にアニメーションすることやアニメーションの目的を感じない、無意味な動きでは逆効果となります。
アニメーションをたくさん使用するより、注目させたいアクションまたは注目してほしいレスポンスでワンポイントとして使うとよいでしょう。

#@# === ListViewをカスタマイズする


