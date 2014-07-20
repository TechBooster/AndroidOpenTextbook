= Andoridでのグラフィック処理

//lead{
モバイル端末は「電話」としての役割の他に、ゲーム等のマルチメディア用途でも多く使用されています。

グラフィック処理の章では、Androidプラットフォームでの高速な描画には欠かすことが出来ない「OpenGL ES」について取り上げます。
//}

== モバイル端末での描画処理

モバイル端末での描画処理は大別すると２種類あります。それはCPU(Central Processing Unit)を使用した描画と、GPU(Graphics Processing Unit)を使用した描画です。

どちらもAndroidには古くから搭載されており、現在においても現役で活躍しています。また、「端末のディスプレイに図柄を出力できる」という意味ではどちらも変わりありません。

しかし、それらは全く違う特徴があり、明確な使い分けを行わなければなりません。

=== CPU描画

CPUでの描画はAndroidではCanvasクラスやViewでの描画に取り入れられています。

CPU描画の特徴は描画のほぼ全てがソフトウェアによって記述されているということです。ソフトウェア制御であることから柔軟性が高く、多くの端末で同様に動作します。

モバイル用途とは少し離れますが、ハリウッド映画等で多用される3DCGも、最終的にはソフトウェア描画によって出力が行われます。

特に、2014年現在のGPUが不得意とする「曲線描画」はCPU描画の得意分野といえます。例えば多くの曲線によって出来上がる「文字」の描画がそれにあたります。

この描画方法の弱点は、そもそも論として「CPUで描画を行うこと」です。「高負荷な処理」の代表例と言ってもいいグラフィック処理をCPUで行わせるということは、それだけOSやアプリ自体の処理に割く余力が減ることであり、アプリの動作を妨げてしまう恐れがあります。

=== GPU描画

AndroidではOpenGL ESを使うことにより、GPUでの描画を行えます。

GPU描画の特徴はGPUという「描画に特化したハードウェア」を使用して、高速な描画が行えるということです。「CPU」「GPU」という独立したハードウェアを使用することで、アプリ側の処理を妨げること無く描画処理を行えます。

昨今の3DゲームはGPUを使用することにより、美麗なゲーム画面を構築しています。

GPUは同時並列での浮動小数点処理に特化しており、

GPU描画の弱点は、やはり「GPUという専用ハードウェアを使用すること」です。専用ハードウェアを使用することで高速な描画が行える一方、メーカーごとの微妙な実装差異により「同じプログラムなのに見た目が違う」という問題に発展する場合があります。

== OpenGL ES 2.0 概要

単刀直入にいえば、OpenGL ESは主に3D描画を行うためのAPIです。それを応用することで2D描画を行うことが出来ますし、CPU描画やその他の技術を組み合わせることで文字列の描画もこなすことができます。

ゲームエンジンとして有名な"Unity"や"Unreal Engine"も、描画部分はOpenGL ESを使用しています。

OpenGL ESは2014年現在いくつかのバージョンがあり、Android端末では（OSやGPUの制限はありますが）全てのバージョンを使用することができます（@<table>{OpenGLES}）。

//table[OpenGLES][OpenGL ESの各バージョンと特徴]{
バージョン名	使用できるAndroidバージョン	特徴
1.0	すべて	OpenGL 1.3のサブセット
1.1	1.6以上	OpenGL 1.5のサブセット
2.0	2.0以上	プログラマブルパイプラインの導入
3.0	4.3以上	OpenGL ES 2.0との互換性を保つアップデート
3.1	L以上	コンピュートシェーダーの導入
//}

本書では2014年現在に普及しているほぼ全てのAndroid端末で動作可能なOpenGL ES 2.0を用いて解説を行います。

== Hello OpenGL ES 2.0!

本書では「見本」となるプロジェクトと、読者が「演習」として実装を行う@<em>{skelton}プロジェクトの２種類を用意しています。Skeltonを加工し、各節ごとの課題を進めていきましょう。

各節には基本となる演習と、それを練習課題となる"TRY"、さらに難しく応用的な知識を試される"CHALLENGE"が用意されています。プログラムの方法は幾通りもあるため、どのような方法で実現しても構いません。

=== 演習：OpenGL ES 2.0の初期化を行う


各サンプルはFragmentを用いて実装します（@<list>{Chapter01_01}）。

//listnum[Chapter01_01][Chapter01_01.java]{
public class Chapter01_01 extends Fragment implements GLSurfaceView.Renderer {

    protected GLSurfaceView glSurfaceView;

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
====================================================== [1] ここから
        glSurfaceView = new GLSurfaceView(getActivity());
        glSurfaceView.setEGLContextClientVersion(2);
        glSurfaceView.setRenderer(this);

        // SurfaceViewを使う場合のおまじない
        // ※ これはWindowSystemの都合で、詳細は割愛
        glSurfaceView.setZOrderOnTop(false);

        return glSurfaceView;
====================================================== [1] ここまで
    }

    @Override
    public void onPause() {
        super.onPause();
====================================================== [2] ここから
        glSurfaceView.onPause();
====================================================== [2] ここまで

        Log.i(getClass().getSimpleName(), "onPause");
    }

    @Override
    public void onResume() {
        super.onResume();
====================================================== [3] ここから
        glSurfaceView.onResume();
====================================================== [3] ここまで

        Log.i(getClass().getSimpleName(), "onResume");
    }

====================================================== [4] ここから
    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {

    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {

    }

    @Override
    public void onDrawFrame(GL10 gl) {
====================================================== [5] ここから
        GLES20.glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
        GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT);
====================================================== [5] ここまで
    }
====================================================== [4] ここまで
}
//}


まず、[1]はFragmentの描画用のViewを作成します。ここではGLSurfaceViewの生成と初期化を行い、描画用のViewとして返しています。

AndroidでOpenGL ESの描画を行うためには、@<em>{SurfaceHolder}か@<em>{SurfaceTexture}というオブジェクトを作成しなければなりません。Androidの初期から存在し、SurfaceHolderを内包しているのが@<em>{SurfaceView}です。多くのゲームはこのSurfaceViewを使用して開発されています@<fn>{現在のView}。

//footnote[現在のView][現在はNativeActivityというC/C++で直接扱え、かつウィンドウシステムと直結した仕組みを使用するケースも増えています]

Androidでは"EGL"という細やかな制御を行えるAPIと、"GLSurfaceView"というそれらをラッピングした簡易的なAPIが用意されています。本書はOpenGL ESの基本的な部分を学ぶという内容であるため、GLSurfaceViewを使用し、初期化部分の詳細については言及しません。

GLSurfaceViewは初期状態でOpenGL ES 1.1用に初期化を行います。今回はOpenGL ES 2.0を対象としますので、初期化バージョンを2へと切り替えます（@<list>{setEGLContextClientVersion}）。

//list[setEGLContextClientVersion][OpenGL ESのバージョンを指定する]{
glSurfaceView.setEGLContextClientVersion(2);
//}

GLSurfaceViewは@<em>{GLSurfaceView.Renderer}インターフェースの適切なメソッドを適切なタイミングで呼び出してくれます。逆にいえば、このインターフェースをsetしない限りGLSurfaceViewは描画を行ってくれません。

Chapter01_01クラスはFragmentを継承すると共に、GLSurfaceView.Rendererインターフェースの実装を行っています。

GLSurfaceView.Rendererインターフェースで実装すべきメソッドは３つあり、それらについては後述します。

最後に、SurfaceViewのZオーダーを変更します。これはAndroidのウィンドウシステムと密接に関わっており、他のViewとの位置関係を示します。ここでは"false"を指定することで他のViewよりも後ろに表示されるようにします（@<list>{setZOrderOnTop}）。

//list[setZOrderOnTop][SurfaceViewのZオーダーを指定する]{
glSurfaceView.setZOrderOnTop(false);  
//}

初期値は端末により異なるため、必ずtrue/falseの指定は行うようにしてください。


Androidのライフサイクルに合わせ、GLSurfaceViewもライフサイクル処理を行わなければなりません。必要のある箇所は２つで、Activity#onPauseとActivity#onResumeです。呼び出すべきメソッドも同じ名前であり、[2][3]の部分で呼び出しを行っています。

[4]の範囲にあるのが、GLSurfaceView.Rendererインターフェースで実装すべきメソッドです。

GLSurfaceView自体はAndroidの最初期から存在するAPIということもあり、メソッドには全てGL11インターフェースが第１引数として渡されます。GL11インターフェースはOpenGL ES 1.1を使用するためのクラスです。ですが今回はOpenGL ES 2.0を使用するため、GL11インターフェースの出番はありません。

onSurfaceCreatedは、GLSurfaceView用の描画メモリが確保されたタイミングで呼び出されます。後述する「画像の読み込み」等はここで行う必要があります。


onSurfaceChangedは、onSurfaceCreatedの直後の他、Android端末の縦横を切り替えたタイミングやSurfaceViewの大きさが変わったタイミングで呼び出されます。

前置きが長くなりましたが、いよいよ[5]がOpenGL ESの@<em>{コマンド}呼び出しです。OpenGL ESのAPIはコマンドと呼ばれ、C言語やJava言語等、さまざまな言語で実装されています。

AndroidではJava言語とC/C++言語で呼び出すことができ、どちらもAPIはほぼ同じです。ですが、言語ごとの仕様に基づいた多少の差異があります。

OpenGL ES 2.0のAPIは全て@<em>{GLES20}クラスのstaticメソッドとして実装されています。

最初のサンプルは「画面を単色で塗りつぶす」というものです。単色で塗りつぶすためには、OpenGL ESにたいして「どんな色で塗りつぶすか」を伝えなければなりません。

glClearColorコマンドはR(赤)、G(緑)、B(青)、A(透過)を引数に指定します。引数の順番はRGBAで、OpenGL ESのAPIで「色」を扱う場合は必ずこの順番となります。ここでの注意点は、色の範囲はPCやゲームの世界では一般的になっている0〜255の値ではなく、0.0〜1.0の浮動小数点で現さなければならないということです（@<list>{glClearColor}）。

//listnum[glClearColor][画面の塗りつぶし色を指定]{
GLES20.glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
//}

画面を実際に塗りつぶすのがglClearコマンドです（@<list>{glClear}）。このコマンドの引数には「OpenGL ESが管理するどのバッファをクリアするか」を指定します。

バッファとは、OpenGL ESが持つキャンバスのようなものだと考えてください。実際に画面に反映される「色」を管理しているのがカラーバッファで、それを示す引数がGL_COLOR_BUFFER_BITです。

//listnum[glClear][画面を実際に塗りつぶす]{
GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT);
//}

このコマンドを呼び出すことで、実際に画面の塗りつぶしが行われます。

==== TRY：画面を任意の色で描画する

コマンドの引数を変更し、画面を任意の色で塗りつぶしてみましょう。

==== TRY：画面をランダムな色で描画する

GLSurfaceViewは毎秒60回ほど、onDrawFrameメソッドを呼び出します。ですがずっと同じ色を描画していたら、「本当に60回も描画してるの？」と思うかもしれません。

そこで、乱数を使用して描画する毎に画面の色を変更してみましょう。Javaでの乱数生成は、下記のメソッドを使用します（@<list>{random}）。

//list[random][0.0〜1.0の乱数を生成する]{
Math.random()
//}

== プリミティブの描画を行う

OpenGL ES 2.0は、「プログラマブルパイプライン」という仕組みを導入しています。これはプログラマが描画の仕組みを「プログラマブル」に、つまりプログラムによって柔軟に書き換えが行えるということです。ですがそれと同時に、プログラマに対してその「パイプラインを構築する」という義務を課しています。

「やれること」が増えた代わりに、「やらなければいけないこと」もまた増えたわけです。

Hello Worldのプログラムから多くの変更がありますが、ひとつひとつはあまり大きくありません。


=== 演習１：三角形を描画する

サンプルコードは次の通りです（@<list>{Chapter01_02}）。

//listnum[Chapter01_02][Chapter01_02.java]{
import static android.opengl.GLES20.*;

public class Chapter01_02 extends Chapter01_01 {
====================================================== [1] ここから
    /**
     * プログラムオブジェクト
     */
    protected int program = 0;
====================================================== [1] ここまで

====================================================== [2] ここから
    /**
     * attr_pos
     */
    protected int attr_pos;

    /**
     * ポリゴン色
     */
    protected int unif_color;
====================================================== [2] ここまで

    /**
     * Surfaceが生成されたタイミングの処理
     */
    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        {
====================================================== [3] ここから
            final String vertexShaderSource =
                            "attribute mediump vec4 attr_pos;" +
                            "void main() {" +
                            "   gl_Position = attr_pos;" +
                            "}";

            final String fragmentShaderSource =
                            "uniform lowp vec4 unif_color;" +
                            "void main() {" +
                            "   gl_FragColor = unif_color;" +
                            "}";
====================================================== [3] ここまで

====================================================== [4] ここから
            final int vertexShader = ES20Util.compileShader(GL_VERTEX_SHADER, vertexShaderSource);
            final int fragmentShader = ES20Util.compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
====================================================== [4] ここまで

====================================================== [5] ここから
            this.program = ES20Util.linkShader(vertexShader, fragmentShader);
====================================================== [5] ここまで
        }
====================================================== [6] ここから
        // locationを取得する
        {
            attr_pos = glGetAttribLocation(program, "attr_pos");
            assert attr_pos >= 0;

            unif_color = glGetUniformLocation(program, "unif_color");
            assert unif_color >= 0;
        }
====================================================== [6] ここまで

====================================================== [7] ここから
        glUseProgram(program);
====================================================== [7] ここまで
    }

====================================================== [8] ここから
    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
        glViewport(0, 0, width, height);
    }
====================================================== [8] ここまで

    @Override
    public void onDrawFrame(GL10 gl) {
        glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL10.GL_COLOR_BUFFER_BIT);

====================================================== [9] ここから
        // ポリゴン色をアップロードする
        // 色はRGBAでアップロードする
        glUniform4f(unif_color, 1.0f, 0.0f, 0.0f, 1.0f);
====================================================== [9] ここまで

====================================================== [10] ここから
        // attr_posを有効にする
        glEnableVertexAttribArray(attr_pos);
====================================================== [10] ここまで

====================================================== [11] ここから
        // 画面中央へ描画する
        final float[] position = {
                // v0
                0.0f, 1.0f,
                // v1
                1.0f, -1.0f,
                // v2
                -1.0f, -1.0f};


        glVertexAttribPointer(attr_pos, 2, GL_FLOAT, false, 0, ES20Util.wrap(position));
====================================================== [11] ここまで

====================================================== [12] ここから
        glDrawArrays(GL_TRIANGLES, 0, 3);
====================================================== [12] ここまで
    }
}

//}

//listnum[][ES20Utilクラス 抜粋]{
    /**
     * 頂点シェーダー/フラグメントシェーダーのコンパイルを行う
     */
    public static int compileShader(int GL_XXXX_SHADER, String source) {
        final int shader = glCreateShader(GL_XXXX_SHADER);

        glShaderSource(shader, source);
        glCompileShader(shader);

        // コンパイルエラーをチェックする
        {
            int[] compileSuccess = new int[]{0};
            glGetShaderiv(shader, GL_COMPILE_STATUS, compileSuccess, 0);
            if (compileSuccess[0] == GL_FALSE) {
                throw new RuntimeException(glGetShaderInfoLog(shader));
            }
        }

        return shader;
    }

    /**
     * 頂点シェーダとフラグメントシェーダをリンクさせる
     */
    public static int linkShader(int vertexShader, int fragmentShader) {
        final int program = glCreateProgram();
        glAttachShader(program, vertexShader);
        glAttachShader(program, fragmentShader);

        glLinkProgram(program);


        // リンクエラーをチェックする
        {
            int[] linkSuccess = new int[]{0};
            glGetProgramiv(program, GL_LINK_STATUS, linkSuccess, 0);
            if (linkSuccess[0] == GL_FALSE) {
                throw new RuntimeException(glGetProgramInfoLog(program));
            }
        }

        // delete
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        return program;
    }

//}

==== importを工夫してコーディングをしやすくする

このサンプルコードを見て気づいたかもしれませんが、今回のサンプルではOpenGL ES 2.0コマンドの呼び出しから"GLES20."の部分が省略されています。これはJavaの機能で、次のように記述することでstaticメソッドやstatic変数へのアクセスを容易に行えます（@<list>{static}、@<list>{static_before}、@<list>{static_after}）。

//list[static][GLES20クラス内の全staticメソッドとフィールドにアクセスする]{
import static android.opengl.GLES20.*;
//}

//list[static_before][変更前のコード]{
GLES20.glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT);
//}

//list[static_after][変更後のコード]{
glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
glClear(GL10.GL_COLOR_BUFFER_BIT);
//}

"GLES20."を省略することで、多くのOpenGL ES関連書籍と同様の構文で記述でき、今後の理解もより深まるでしょう。

==== 初期化を行う

OpenGL ES 2.0では、「GPUで実行される、描画専用のプログラム」を最低2種類記述する必要があります。それが「頂点シェーダー」と「フラグメントシェーダー」です。フラグメントシェーダーは、「ピクセルシェーダー」と言い換えればご存知の方もいるかもしれません。フラグメントシェーダーとピクセルシェーダーはどちらも特に違いはありませんが、OpenGL ESではピクセルを「フラグメント」と呼ぶため、フラグメントシェーダーと名付けられています。

ただし、このフラグメントとはAndroidプログラミング的な意味のandroid.app.Fragmentクラスとは全く意味合いが異なるため、注意してください。グラフィックの章内でカタカナ表記の「フラグメント」とはOpenGL ESのピクセルを示し、英語表記のFragmentとはAndroid SDKに含まれるandroid.app.Fragmentクラスを示します。

[1][3][4][5]の箇所が、シェーダーの生成とプログラム内への保持を行っている箇所です。まずは頂点シェーダー・フラグメントシェーダーのプログラムを見てみましょう（@<list>{vertexShaderSource}、@<list>{fragmentShaderSource}）。

//list[vertexShaderSource][頂点シェーダー]{
            final String vertexShaderSource =
                            "attribute mediump vec4 attr_pos;" +
                            "void main() {" +
                            "   gl_Position = attr_pos;" +
                            "}";
//}

//list[fragmentShaderSource][フラグメントシェーダーシェーダー]{
            final String fragmentShaderSource =
                            "uniform lowp vec4 unif_color;" +
                            "void main() {" +
                            "   gl_FragColor = unif_color;" +
                            "}";
//}

これらは全てString型の変数、つまりただの文字列としてJava言語のプログラム内に保持されています。もちろん、Javaがコンパイルされてアプリとなっても文字列は文字列のままで、このままでは実行できません。

OpenGL ES 2.0のシェーダーはGPUで動作させる必要がありますが、GPUは無数に種類があり、なおかつ機械語単位で見ると互換性がありません。そのため、GPUで実行するプログラムをコンパイルするためには、そのGPUが載っている端末でコンパイルを行わなければならないのです@<fn>{シェーダー事前コンパイル}。

//footnote[シェーダー事前コンパイル][厳密にいえば、事前コンパイルが出来ますが機種ごとに互換性が失われるためAndroidではあまり使用されていません]

シェーダーは「GLSL ES」というC言語に似た言語で記述することができ、エントリーポイントとなるのは"main"関数です。このプログラムはC言語に似ていますが、GPUにより同時並列的な実行が行われるため、さまざまな制約があります。ですが、まずは細かいことを考えずに少しずつ理解していってください。

シェーダーで最初に覚えるべきは２つあります。

一つは、「頂点シェーダーでは必ずgl_Positionに頂点座標を書き込むこと」。もう一つは、「gl_FragColorに最終的なフラグメント色を書き込むこと」です。

サンプルの頂点シェーダーは「gl_Positionに対し、Java側のプログラムから与えられたattr_posを代入する」ことを行い、フラグメントシェーダーでは「gl_FragColorに対し、Java側のプログラムから与えられたunif_colorを代入する」という処理を行います。

[4]の部分では頂点シェーダーとフラグメントシェーダーのコンパイルを行っています。ES20Util.compileShaderは筆者が用意した補助メソッドです。内部ではコンパイルとエラーチェックを行っています。詳細な解説を行うためにはページ数が足りませんので、興味がある方は実装を覗いてみるとよいでしょう。

シェーダーのコンパイル結果はそれぞれint型の変数として取得できます。OpenGL ESはオブジェクト指向のAPIではなく、OpenGL ESが管理するメモリは全て"ID"が割り振られ、そのIDによって管理する必要があります。

ですが、まだこの状態では実行をすることが出来ません。例えばAndroidアプリは".java"拡張子のプログラムを記述し、コンパイルによって".class"ファイルが出来上がります。実際にAndroid端末上で実行するためにはそれらを結合して".dex"ファイルに変換しなければなりません。これらは（厳密にいえば違いますが）、"コンパイル"と"リンク"という別々の作業であり、そのどちらも実行には欠かすことが出来ません。

Eclipseのような統合開発環境はそれらを隠ぺいしているため、「コンパイル＝実行できる状態にする」ように見えますが、実際には複数の処理を行っているのです。

「コンパイル〜実際に実行可能な状態にする」という一連の流れを、一般的に"ビルド(build)"と呼びます。

シェーダーも同じように、リンクを行わなければ実行することが出来ません。リンクを行っているのが[5]のES20Util.linkShaderメソッドです。ES20Util.compileShaderとES20Util.linkShaderはどちらも定型的な処理で、エラー処理を除けばほぼ書き換えることはありません。

[2][6]ではシェーダーから変数の"Location(番号)"を取得します。シェーダーはGPUという特殊な環境下で動作する特殊なプログラムです。そのため、Java側のコードから値をアップロードするためには、変数の管理番号を取得して「何番の変数にこの値を入力してください」という命令を行わなければなりません。

このサンプルでシェーダーで宣言している変数は2つです（@<list>{attr_pos}、@<list>{unif_color}）。

//list[attr_pos][頂点シェーダーで定義している変数]{
attribute mediump vec4 attr_pos;
//}

//list[unif_color][フラグメントシェーダーで定義している変数]{
uniform lowp vec4 unif_color;
//}

attr_posとunif_colorにそれぞれ値をアップロードしなければ、シェーダーは正常に計算を行うことが出来ません。そしてアップロードを行うためにはLocationを取得するしか無く、Locationを取得したら使用するためにはどこかに保持して置かなければなりません。

取得したLocationを保持しているのがメンバ変数attr_posとunif_colorです。今回はそれぞれシェーダー内の変数名と同じ変数名をそれぞれ名づけていますが、この名前はプログラマがわかりやすければ何でも構いません。

Locationを取得するためにはそれぞれ"glGetAttribLocation"と"glGetUniformLocation"を使用する必要があります。どちらも第１引数は取得を行うプログラムオブジェクト、第２引数は変数名です。正常にLocationが取得できると0以上の値が返却されます。もし変数名が間違っていたり、使用されていない変数の場合は"-1"がエラーとして返却されます。

最後に[7]の"glUseProgram"コマンドを使用し、「このシェーダーで描画を行いますよ」とGPUに対して宣言します。

これで初期化は完了です。

==== Viewportの変更を行う

onSurfaceChangedでは、"glViewport"コマンドを呼び出しています。OpenGL ESは、「画面内のどの領域を描画に使用するか(Viewport)」という設定を自由に行うことが出来ます。Androidではデフォルトで「画面全体を描画に使用する」としています。

ですがAndroid端末によっては初期値が"0"であったり、端末の縦横を切り替えた際に問題が発生します。

そのため、「サーフェイスの大きさが変更された」というメッセージであるonSurfaceChangedでViewportを変更しています。

このViewportの注意点として、一般的なディスプレイの座標系（左上が原点0, 0）ではなく、左下を原点（0, 0）とする座標系となります。

==== 三角形描画を行う

[9]の部分では、uniform変数へ「プリミティブの色」をアップロードしています。

サンプルでフラグメントシェーダーで定義しているのが、"uniform変数"です。uniform変数へ値をアップロード@<fn>{アップロード}するためには、glUniform4fコマンドを使用します。

アップロードは第１引数がuniform変数のLocation、第２〜第５引数がそれぞれX/Y/Z/W要素に渡される値です。XYZWはgl_FlagColorに書き込まれるとRGBAそれぞれの色として認識されます。ここではRGBAそれぞれR(1.0f)G(1.0f)B(0.0f)A(1.0f)となり、赤い色をアップロードしています（@<list>{glUniform4f}）。

//list[glUniform4f][アップロード部分]{
glUniform4f(unif_color, 1.0f, 0.0f, 0.0f, 1.0f);
//}

//footnote[アップロード][OpenGL ESではCPU＝クライアント、GPU＝サーバーとして考えるため、値の設定や取得を"アップロード"・"ダウンロード"と呼びます]

glUniformXXXXコマンドは変数の型ごとに用意されており、今回宣言している"unif_color"変数はvec4型であるため（@<list>{uniform}）、glUniform4fコマンドを使用します。この他にvec3型の変数をアップロードするglUniform3fコマンドや、後述するmat4型の変数をアップロードするための"glUniformMatrix4fv"コマンド等があります。

//list[uniform][uniform変数の宣言]{
uniform lowp vec4 unif_color;
//}

[10]の部分で行っているのは、glEnableVertexAttribArrayコマンドによる変数の有効化です。attribute変数は、GPUによって「使用する」「使用しない」という２つの状態があります。これは不要な変数は使用しないほうがGPUにとって負荷が減り、描画速度の向上やバッテリー消費を抑えることに役立ちます。

頂点シェーダー内で使用されるattributeは、アプリが用意した"頂点"と"GPUの変数"とを結びつける重要な変数です。このサンプルでは三角形の各頂点の座標をattribute変数としてGPUへアップロードします。

また、attribute変数の内容は頂点シェーダーのmain()関数が呼び出されるごとに変化します。

全ての変数は初期状態で「使用しない」状態となるため、必要に応じてこのコマンドを呼び出す必要があります。

初心者にとってわかりにくいのが、"uniform変数"と"attribute変数"の違いです。

uniform変数は頂点シェーダーとフラグメントシェーダーの両方で使用することが出来ます。また、描画処理（後述のglDrawArraysコマンド）実行中は一切変数の値が変化しません。

それに対し、attribute変数は頂点シェーダーでしか使用することが出来ません。attribute変数は「頂点データ専用」の変数と考えてください。

GPUは１頂点につき１回のmain関数が実行され、さらに頂点数だけ繰り返されます。三角形は３つの頂点で構成されるため、頂点シェーダーの呼び出し回数は３回です。attribute変数の内容はmain関数呼び出しの度に「頂点０のデータ」「頂点１のデータ」「頂点２のデータ」...という具合に変化します。

[11]では、実際に使用する頂点データを定義しています。"頂点データ"といっても、専用のクラスは用意されていません。なぜなら、GPUにとって"頂点"とはメモリの塊でしか無いからです。

OpenGL ES 2.0では、頂点の内容をプログラマが自由に決めることができます。サンプルでの頂点データは「１つの頂点は浮動小数点型のX座標とY座標を持つ」という内容になっています。三角形ですので頂点は３つ無ければなりません。

合計すると、2要素(XY)×3頂点=6つのデータがあるということです。

それを定義しているのが[11]のposition配列です。配列のindex0とindex1が頂点0、index2とindex3が頂点1、index4とindex5が頂点2をそれぞれ構成します。この時、頂点データは「1つの配列」でなければなりません（@<list>{defined_data}）。

//list[defined_data][定義している頂点データ]{
        final float[] position = {
                // v0
                0.0f, 1.0f,
                // v1
                1.0f, -1.0f,
                // v2
                -1.0f, -1.0f};
//}

この時、座標の値は-1.0〜1.0になるということに注目してください。GPUが描画に使用できる空間は-1.0〜1.0であり、ピクセル座標（現代のモバイル端末の多くはHD以上の解像度を持ちます）ではありません。。Viewport内が-1.0〜1.0として扱われ、最終的な画面座標として出力されます。

今回の三角形は「画面（Viewport）いっぱいに表示する」という内容のため、座標系の目一杯端まで使っていることがわかります。

次に行うのが、attribute変数と頂点データの関連付けです。頂点データは非常に大きいデータ（例えば、最近のゲームでは1キャラクターにつき数万の頂点が使用されます）であるため、一括でGPUが処理できません。

そのため、「読み出し場所」をGPUに教えておくことで、必要なデータを逐次処理（ストリーミング）することが出来るのです。

その「読み出し場所」を指定するのがglVertexAttribPointerコマンドです。

第１引数には関連付けるattribute変数を指定します。

第２引数は頂点の要素数を指定します。ここではXYの2要素ですので、"2"を指定します。

第３引数は頂点の型を指定します。今回はfloat型ですので、それを示す定数のGL_FLOATを指定します。

第４引数は頂点データの正規化有無を指定します。これは省メモリ・速度重視の設計では重要になりますが、本書では説明しません。GL_FLOATを使う限り、この引数は"false"を指定すれば問題ありません。

第５引数には頂点間のオフセット（byte単位）を指定しますが、現在は0を指定すれば問題ありません。この引数についての詳細は後述します。

最後の引数でようやくデータを渡します。この時、引数の型は"java.nio.Buffer"であり、配列型ではありません。

==== java.nio.Buffer型

Android SDKによって記述されたアプリは、Dalvik VM（もしくはART）という仮想マシンの上で動作します。配列に格納されたデータは仮想マシン上で読み込むために最適化された状態で保存されますが、これらはGPUから読み込むためには適した状態ではありません@<fn>{VMのメモリ}。

//footnote[VMのメモリ][GCによってRAM上のアドレスが変更されたり、そもそもCPU/GPUが求めるエンディアンに従っていない場合もあります]

Buffer型はGPU（や、C/C++言語のコード）から読みやすい場所へメモリを配置します。配列からBufferへの変換は画一的な動作です。注意するのは、配列からBufferを生成すると「コピー処理」が発生するため、メモリの使用量が2倍になってしまうということです。

サンプルでは可読性を上げるため効率的なBufferの生成を行っていません。サンプルを実行すると、毎秒60回のペースで配列とBufferオブジェクトが生成・廃棄を繰り返すことになります。

Bufferオブジェクトの生成自体は次のようになります（@<list>{ByteBuffer}）。

//list[ByteBuffer][float配列からBufferオブジェクトの生成]{
    public static FloatBuffer wrap(float[] buffer) {
        return (FloatBuffer) ByteBuffer.allocateDirect(buffer.length * 4).order(ByteOrder.nativeOrder()).asFloatBuffer().put(buffer).position(0);
    }
//}

ここまできたらもう一息です。あとは描画コマンドを発行すれば完了です。描画を行うためには、[12]のようにglDrawArraysコマンドを呼び出します。

第１引数は頂点をどのようにつなげて三角形を構築するかを指定します。これは2つ以上の三角形を効率的なメモリ量・速度で描画するのに役立ちますが、サンプルでは最も基本的な形である"GL_TRIANGLES"となります。

第２引数と第３引数は、「何番目の頂点から何個分の頂点を使用するか」を指定します。今回は0番目の頂点から3個分の頂点で描画を行わせますので、それぞれ"0"、"3"となります。この時、第３引数が3未満だと描画が正常に行えません。なぜなら、「三角形を構築するには最低でも3つの頂点が必要」だからです。

==== TRY：端末の縦横を切り替えてみよう

OpenGL ESの3D描画は「ピクセル座標に依存しない」座標系です。そのため、「縦長」「横長」のViewportに関係なく、座標系は-1.0〜1.0となります。端末の縦横を切り替え、どのように描画内容が変化するか確かめてみましょう。

==== TRY：Viewportを変更しよう

画面の縦横切替と同じく、Viewportを自由に変更してどのような変化があるか確認してみましょう。

==== TRY：三角形の色を変えてみよう

サンプルでは赤い色で三角形を描画していました。これを修正し、任意の色で三角形を描画できるようにしてみましょう。修正箇所はサンプル内のどこでも構いません。

==== TRY：三角形の大きさや位置を変更しよう

サンプルではViewportいっぱいに三角形が表示されています。頂点位置を自由に修正し、どのように三角形の見え方が変わるか確認してみてください。

==== TRY：ES20Util.wrapの呼び出し回数を最小限にしよう

前述のように、サンプルでは毎秒60回程度Util.wrapメソッドが呼び出されています。呼び出し回数が増えればそれだけGC対象が増えますし、バッテリーへの負担も大きくなります。そこで、Util.wrapの実行回数がなるべく少なくなるように修正してみてください。

==== CHALLENGE："glUniform4f"やassertの部分を変更せずに、シェーダーだけで任意の色のポリゴンを表示させてみよう

glUniform4fでアップロードする値を変更すれば、任意の色の描画を行うことが出来ます。そこで、glUniform4f(unif_color, 1.0f, 0.0f, 0.0f, 1.0f)の部分を変更せず、uniform変数も使用した上で「緑」「青」の三角形を描画してみましょう。

=== 演習２：四角形を描画する

サンプルコードは次の通りです（@<list>{Chapter01_03}）

//listnum[Chapter01_03][Chapter01_03.java]{
public class Chapter01_03 extends Chapter01_02 {
    @Override
    public void onDrawFrame(GL10 gl) {
        中略...

        {
            final float[] position = {
                    // triangle 0
                    // v0(left top)
                    -0.75f, 0.75f,
                    // v1(left bottom)
                    -0.75f, -0.75f,
                    // v2(right top)
                    0.75f, 0.75f,

                    // triangle 1
                    // v3(right top)
                    0.75f, 0.75f,
                    // v4(left bottom)
                    -0.75f, -0.75f,
                    // v5(right bottom)
                    0.75f, -0.75f,
            };
            GLES20.glVertexAttribPointer(attr_pos, 2, GLES20.GL_FLOAT, false, 0, ES20Util.wrap(position));
            GLES20.glDrawArrays(GLES20.GL_TRIANGLES, 0, 6);
        }
    }
}

//}

OpenGL ESは四角形以上のn角形の描画も行うことが出来ます。例えば、四角形は2つの三角形に分割できますし、五角形は3つの三角形に分割することが出来ます。サンプルでは三角形を2つ描画することで、四角形を構築しています。

今回は2つの三角形を描画するため、頂点データを6(三角形)×2=12個用意しています。

glVertexAttribPointerは全く変わりありません。なぜなら、glVertexAttribPointerコマンドには「いくつの頂点を渡すか」という情報は付随しないからです。そのため、頂点数が6個でも、12個でも、さらに増えて数万あったとしてもこのコマンドには影響がありません。

glDrawArraysコマンドの第３引数は"6"に変更されています。これは四角形＝三角形2つで構築される形状であるため、6つの頂点を描画に使用することを示しています。

==== TRY：右側の三角形、左側の三角形だけをそれぞれ描画してみよう

glDrawArraysの引数を修正して、「前半の三角形」だけを描画してみましょう。また、同じくglDrawArraysの引数を修正して「後半の三角形」だけを描画してみましょう。

==== TRY：五角形や六角形を描画してみよう

三角形を組み合わせることで、複雑な図形を描画することが可能です。三角形の配置や頂点座標を工夫し、五角形や六角形を構築してみましょう。

== VRAMの利用

GPUは描画専用のメモリ＝VRAMを持ちます。VRAMにあるデータはGPUから効率的にアクセスが行えるため、描画速度を重視する場合は非常に重要です。特に描画時に使用する2次元画像="Texture"はVRAMに配置した状態でしかアクセスすることが出来ません。

VRAMはJavaの仮想マシンとは別に確保されるため、例えば数百MBもの量を使用しても仮想マシンがOutOfMemoryによって強制終了される心配はありません。

=== 演習３：テクスチャを読み込む

サンプルコードは次の通りです（@<list>{Chapter01_04}）。

//listnum[Chapter01_04][Chapter01_04.java]{
public class Chapter01_04 extends Chapter01_01 {

    中略...

====================================================== [1] ここから
    /**
     * UV座標
     */
    protected int attr_uv;

    /**
     * テクスチャUniform
     */
    protected int unif_texture;
====================================================== [1] ここまで

====================================================== [2] ここから
    /**
     * テクスチャオブジェクト
     */
    protected int texture;
====================================================== [2] ここまで

    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        {
====================================================== [3] ここから
            final String vertexShaderSource =
                            "attribute mediump vec4 attr_pos;" +
                            "attribute mediump vec2 attr_uv;" +
                            "varying mediump vec2 vary_uv;" +
                            "void main() {" +
                            "   gl_Position = attr_pos;" +
                            "   vary_uv = attr_uv;" +
                            "}";

            final String fragmentShaderSource =
                            "uniform sampler2D unif_texture;" +
                            "varying mediump vec2 vary_uv;" +
                            "void main() {" +
                            "   gl_FragColor = texture2D(unif_texture, vary_uv);" +
                            "}";
====================================================== [3] ここまで

            // コンパイルとリンクを行う
            this.program = ES20Util.compileAndLinkShader(vertexShaderSource, fragmentShaderSource);
        }

        // locationを取り出す
        {
            attr_pos = glGetAttribLocation(program, "attr_pos");
            assert attr_pos >= 0;
            attr_uv = glGetAttribLocation(program, "attr_uv");
            assert attr_uv >= 0;
            unif_texture = glGetUniformLocation(program, "unif_texture");
            assert unif_texture >= 0;
        }

        // テクスチャを読み込む
        {
            Bitmap bitmap = ES20Util.decodeBitmapFromAssets(getActivity(), "sample512x512.png");

====================================================== [4] ここから
            int[] textureId = {0};
            glGenTextures(1, textureId, 0);
            this.texture = textureId[0];
            assert this.texture != 0;
====================================================== [4] ここまで

====================================================== [5] ここから
            glBindTexture(GL_TEXTURE_2D, texture);
            {
                GLUtils.texImage2D(GL_TEXTURE_2D, 0, bitmap, 0); // テクスチャをバインド
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            }
            glBindTexture(GL_TEXTURE_2D, 0); // テクスチャをアンバインド
====================================================== [5] ここまで

            ES20Util.assertGL();
        }

        glUseProgram(program);
        ES20Util.assertGL();
    }

    中略...

    @Override
    public void onDrawFrame(GL10 gl) {
        glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // attr_posを有効にする
        glEnableVertexAttribArray(attr_pos);
        glEnableVertexAttribArray(attr_uv);

====================================================== [6] ここから
        // 使用するテクスチャをバインドする
        glBindTexture(GL_TEXTURE_2D, texture);
====================================================== [6] ここまで

        {
            final float[] position = {
                    // triangle 0
                    // v0(left top)
                    -0.75f, 0.75f,
                    // v1(left bottom)
                    -0.75f, -0.75f,
                    // v2(right top)
                    0.75f, 0.75f,

                    // triangle 1
                    // v3(right top)
                    0.75f, 0.75f,
                    // v4(left bottom)
                    -0.75f, -0.75f,
                    // v5(right bottom)
                    0.75f, -0.75f,
            };


            glVertexAttribPointer(attr_pos, 2, GL_FLOAT, false, 0, ES20Util.wrap(position));

====================================================== [7] ここから
            final float[] uv = {
                    // triangle 0
                    // v0(left top)
                    0, 0,
                    // v1(left bottom)
                    0, 1,
                    // v2(right top)
                    1, 0,

                    // triangle 1
                    // v3(right top)
                    1, 0,
                    // v4(left bottom)
                    0, 1,
                    // v5(right bottom)
                    1, 1,
            };

            glVertexAttribPointer(attr_uv, 2, GL_FLOAT, false, 0, ES20Util.wrap(uv));
====================================================== [7] ここまで
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }
    }
}

//}

==== テクスチャオブジェクトの生成

今回のシェーダーでは今まで使用していた「色」を保存したuniform変数"unif_color"に変わり、テクスチャを示す型である"sampler2D型"が使用されています。

OpenGL ESによって生成されたテクスチャオブジェクトはシェーダーのようにint型の変数によって"ID"が管理されています（@<list>{TextureObject}）。

//list[TextureObject][テクスチャオブジェクトはint型変数で管理される]{
    /**
     * テクスチャオブジェクト
     */
    protected int texture;
//}

テクスチャの読み込み部分は[4][5]です。Androidでは画像を示すクラスとしてBitmapクラスがありますが、Bitmapクラスだけではテクスチャとして使用することが出来ません。Bitmapクラスからテクスチャオブジェクトへアップロードを行う必要があります。

まず、テクスチャオブジェクトの生成です（@<list>{glGenTextures}）。

//list[glGenTextures][テクスチャオブジェクト生成]{
            int[] textureId = {0};
            glGenTextures(1, textureId, 0);
            this.texture = textureId[0];
//}

テクスチャオブジェクトの生成はglGenTexturesコマンドで行います。このコマンドは第１引数に「何枚のテクスチャを確保するか」を指定し、第２引数には戻り値の格納先となる配列を指定します。最後に、第３引数で「配列のどのindexから使用するか」を指定します。特別な理由がない限り、第３引数は常に0で問題ありません。

Android SDKに組み込まれたOpenGL ESのコマンドは、多くの場合この「配列のどのindexから使用するか」を指定することになります。これはJava言語がポインタ（メモリ）を直接扱えないための救済措置ともいえますが、本書では特別なことをしないため全て"0"になります。

サンプルでは1枚のテクスチャをtextureId配列のindex0移行に格納するため、glGenTextures内部では"textureId[0] = テクスチャオブジェクトのID"のような処理が行われます。

textureIdは一時変数であるため、this.textureに対して返却値を保存してテクスチャオブジェクトの生成処理は完了です。

テクスチャオブジェクトは生成しただけでは使用することが出来ません。OpenGL ESに対して「これから使用を開始するよ」ということを宣言しなければならないのです。テクスチャオブジェクトの使用を開始するコマンドが"glBindTexture"です。

glBindTextureの第1引数には"GL_TEXTURE_2D"を指定します。第２引数には、使用を開始するテクスチャオブジェクトを指定します。また、第２引数に"0"を指定することで「使用を終了するよ」というのを明示できます。

テクスチャの使用開始を"バインド"、使用終了を"アンバインド"とそれぞれ呼びます@<fn>{バインドの詳細}。

バインド処理は描画直前である[6]でも行っています。なぜなら、「これからの描画でこのテクスチャを使用するよ」ということをGPUに伝えなければならないからです。

//footnote[バインドの詳細][テクスチャは最大32個存在するテクスチャユニットにバインドが出来、どれを操作(Active)するかの切り替えも行います。また、シェーダーに関連付けるテクスチャオブジェクトはglUniform1iコマンドとテクスチャユニット番号によって制御できますが、ページの都合上ここでは割愛しています]

バインド中に行われるのが次の処理です（@<list>{texImage2D}）。

//list[texImage2D][バインド中に行われる処理]{
GLUtils.texImage2D(GL_TEXTURE_2D, 0, bitmap, 0); // テクスチャをアップロード
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//}

GLUtils.texImage2Dメソッドは、Androidが用意している補助メソッドです。このメソッドを呼び出すと、Bitmapにある画像情報をテクスチャオブジェクトへアップロードすることが出来ます。

glTexParameteriコマンドはテクスチャがGPUによってどのようにアクセスされるかを指定することが出来ます@<fn>{glTexParameteri補足}。詳細の解説は割愛しますが、サンプルにある4つは「お約束」として呼び出すようにしてください。もし「真っ黒なテクスチャ」が描画されてしまった場合、この初期設定が適切に行われなかった可能性があります。

これでテクスチャオブジェクトの初期化は完了です。

//footnote[glTexParameteri補足][WRAPはUVが0.0〜1.0を超えた場合の補完方法、FILTERは拡大縮小時のピクセル色の補完方法をそれぞれ指定します]

==== UV座標

今回のシェーダーでは新たな種類の変数である"varying変数"が登場しました。varying変数は「頂点シェーダーからフラグメントシェーダーに対して値を渡す」場合に使用される変数です@<fn>{varying変数}。サンプルではvec2型varying変数の"vary_uv"が頂点シェーダー・フラグメントシェーダー両方に定義されており、頂点シェーダーは「書き込み用」、フラグメントシェーダーは「読み込み用」としてそれぞれ使用しています。

//footnote[varying変数][varying変数を使用すると、頂点間の値が自動的に補完されます]

テクスチャを使用するためには「UV座標」について知らなければなりません。UV座標とは、「テクスチャ内の位置」を示す情報で、横方向の座標を"U"、縦方向の座標を"V"としてそれぞれ扱います。

座標は0.0〜1.0で、テクスチャ解像度に依存しない仕組みとなっています。

OpenGL ESで「ポリゴンにテクスチャを貼り付ける」というのは、「一つ一つの頂点に対して、どの頂点をどのUV座標に関連付けるか」を指定するということです。

頂点単位で渡されたUV座標は、varying変数を通してフラグメントシェーダーへと渡されます。フラグメントシェーダーはそのUV座標から、実際に「テクスチャの色」を取得しなければなりません。UV座標からテクスチャの色を取得する関数が"texture2D"です。第１引数にはアクセスするテクスチャを、第2２引数にはUV座標をそれぞれ指定します（@<list>{vary_uvVertex}、@<list>{vary_uvFragment}）。

//list[vary_uvVertex][頂点シェーダー]{
            final String vertexShaderSource =
                            "attribute mediump vec4 attr_pos;" +
                            "attribute mediump vec2 attr_uv;" +
                            "varying mediump vec2 vary_uv;" +
                            "void main() {" +
                            "   gl_Position = attr_pos;" +
                            "   vary_uv = attr_uv;" +
                            "}";
//}

//list[vary_uvFragment][フラグメントシェーダー]{
            final String fragmentShaderSource =
                            "uniform sampler2D unif_texture;" +
                            "varying mediump vec2 vary_uv;" +
                            "void main() {" +
                            "   gl_FragColor = texture2D(unif_texture, vary_uv);" +
                            "}";
//}

==== 頂点にUV座標を追加する

頂点にUV座標を追加するため、[7]では新たなデータであるfloat型変数"uv"を追加しています。UV座標はU/Vの2要素で構成されるため、2要素×3頂点×2ポリゴンで、合計12要素の配列となります。

直後にあるglVertexAttribPointerコマンドでは、attr_uvに対する設定を行っています。頂点の要素（位置、UV、etc）が増えても、基本的には配列を用意する→glVertexAttribPointerでattribute変数と関連付けるという流れは変わりません。

最後に、glDrawArraysで描画を行います。これも、「三角形の数」自体は同じであるため、呼び出しに変化はありません。

==== TRY：UV座標を操作してテクスチャの一部を描画してみよう

UV座標を変更することで、描画されるテクスチャの範囲を自由に決めることが出来ます。歪んだ形状にするなど、UV座標によるテクスチャ表示のされ方の変化を確かめてみましょう。

また、UVの範囲が0.0〜1.0を超えた場合にどのような描画が行われるか確かめてみましょう。

==== TRY：シェーダーを使ってテクスチャの「ネガ」を描画してみよう

texture2D関数によって読みだした値はシェーダー内の計算処理で自由に変化させることが出来ます。そこで、計算処理によって写真の「ネガ」のような画像を描画してみましょう。

