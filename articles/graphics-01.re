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

AndroidではOpenGL ESを使うことにより、GPUでの描画を行わせることが出来ます。

GPU描画の特徴はGPUという「描画に特化したハードウェア」を使用することで、高速な描画が行えるということです。「CPU」「GPU」という独立したハードウェアを使用することで、アプリ側の処理を妨げること無く描画処理を行えます。

昨今の3DゲームはGPUを使用することにより、美麗なゲーム画面を構築しています。

GPUは同時並列での浮動小数点処理に特化しており、

GPU描画の弱点は、やはり「GPUという専用ハードウェアを使用すること」です。専用ハードウェアを使用することで高速な描画が行える一方、メーカーごとの微妙な実装差異により「同じプログラムなのに見た目が違う」という問題に発展する場合があります。

== OpenGL ES 2.0 概要

単刀直入に言えば、OpenGL ESは主に3D描画を行うためのAPIです。それを応用することで2D描画を行うことが出来ますし、CPU描画やその他の技術を組み合わせることで文字列の描画もこなすことが出来ます。

ゲームエンジンとして有名な"Unity"や"Unreal Engine"も、描画部分はOpenGL ESを使用しています。

OpenGL ESは2014年現在いくつかのバージョンがあり、Android端末では（OSやGPUの制限はありますが）全てのバージョンを使用することが出来ます。

//table[][OpenGL ESの各バージョンと特徴]{
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

=== 演習: OpenGL ES 2.0の初期化を行う

//listnum[][Chapter01_01.java]{
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

各サンプルはFragmentを用いて実装します。

まず、[1]はFragmentの描画用のViewを作成します。ここではGLSurfaceViewの生成と初期化を行い、描画用のViewとして返しています。

AndroidでOpenGL ESの描画を行うためには、@<em>{SurfaceHolder}か@<em>{SurfaceTexture}というオブジェクトを作成しなければなりません。Androidの初期から存在し、SurfaceHolderを内包しているのが@<em>{SurfaceView}です。多くのゲームはこのSurfaceViewを使用して開発されています@<fn>{現在のView}。

//footnote[現在のView][現在はNativeActivityというC/C++で直接扱え、かつウィンドウシステムと直結した仕組みを使用するケースも増えています]

Androidでは"EGL"という細やかな制御を行えるAPIと、"GLSurfaceView"というそれらをラッピングした簡易的なAPIが用意されています。本書はOpenGL ESの基本的な部分を学ぶという内容であるため、GLSurfaceViewを使用し、初期化部分の詳細については言及しません。

GLSurfaceViewは初期状態でOpenGL ES 1.1用に初期化を行います。今回はOpenGL ES 2.0を対象としますので、初期化バージョンを2へと切り替えます。

//list[][OpenGL ESのバージョンを指定する]{
glSurfaceView.setEGLContextClientVersion(2);
//}

GLSurfaceViewは@<em>{GLSurfaceView.Renderer}インターフェースの適切なメソッドを適切なタイミングで呼び出してくれます。逆に言えば、このインターフェースをsetしない限りGLSurfaceViewは描画を行ってくれません。

Chapter01_01クラスはFragmentを継承すると共に、GLSurfaceView.Rendererインターフェースの実装を行っています。

GLSurfaceView.Rendererインターフェースで実装すべきメソッドは３つあり、それらについては後述します。

最後に、SurfaceViewのZオーダーを変更します。これはAndroidのウィンドウシステムと密接に関わっており、他のViewとの位置関係を示します。ここでは"false"を指定することで他のViewよりも後ろに表示されるようにします。

初期値は端末により異なるため、必ずtrue/falseの指定は行うようにしてください。

//list[][SurfaceViewのZオーダーを指定する]{
glSurfaceView.setZOrderOnTop(false);  
//}

Androidのライフサイクルに合わせ、GLSurfaceViewもライフサイクル処理を行わなければなりません。必要のある箇所は２つで、Activity#onPauseとActivity#onResumeです。呼び出すべきメソッドも同じ名前であり、[2][3]の部分で呼び出しを行っています。

[4]の範囲にあるのが、GLSurfaceView.Rendererインターフェースで実装すべきメソッドです。

GLSurfaceView自体はAndroidの最初期から存在するAPIということもあり、メソッドには全てGL11インターフェースが第１引数として渡されます。GL11インターフェースはOpenGL ES 1.1を使用するためのクラスです。ですが今回はOpenGL ES 2.0を使用するため、GL11インターフェースの出番はありません。

onSurfaceCreatedは、GLSurfaceView用の描画メモリが確保されたタイミングで呼び出されます。後述する「画像の読み込み」等はここで行う必要があります。


onSurfaceChangedは、onSurfaceCreatedの直後の他、Android端末の縦横を切り替えたタイミングやSurfaceViewの大きさが変わったタイミングで呼び出されます。

前置きが長くなりましたが、いよいよ[5]がOpenGL ESの@<em>{コマンド}呼び出しです。OpenGL ESのAPIはコマンドと呼ばれ、C言語やJava言語等、様々な言語で実装されています。

AndroidではJava言語とC/C++言語で呼び出すことができ、どちらもAPIはほぼ同じです。ですが、言語ごとの仕様に基づいた多少の差異があります。

OpenGL ES 2.0のAPIは全て@<em>{GLES20}クラスのstaticメソッドとして実装されています。

最初のサンプルは「画面を単色で塗りつぶす」というものです。単色で塗りつぶすためには、OpenGL ESにたいして「どんな色で塗りつぶすか」を伝えなければなりません。

glClearColorコマンドはR(赤)、G(緑)、B(青)、A(透過)を引数に指定します。引数の順番はRGBAで、OpenGL ESのAPIで「色」を扱う場合は必ずこの順番となります。ここでの注意点は、色の範囲はPCやゲームの世界では一般的になっている0〜255の値ではなく、0.0〜1.0の浮動小数点で現さなければならないということです。

//listnum[][画面の塗りつぶし色を指定]{
GLES20.glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
//}

画面を実際に塗りつぶすのがglClearコマンドです。このコマンドの引数には「OpenGL ESが管理するどのバッファをクリアするか」を指定します。

バッファとは、OpenGL ESが持つキャンバスのようなものだと考えてください。実際に画面に反映される「色」を管理しているのがカラーバッファで、それを示す引数がGL_COLOR_BUFFER_BITです。

//listnum[][画面を実際に塗りつぶす]{
GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT);
//}

このコマンドを呼び出すことで、実際に画面の塗りつぶしが行われます。

==== TRY: 画面を任意の色で描画する

コマンドの引数を変更し、画面を任意の色で塗りつぶしてみましょう。

==== TRY: 画面をランダムな色で描画する

GLSurfaceViewは毎秒60回ほど、onDrawFrameメソッドを呼び出します。ですがずっと同じ色を描画していたら、「本当に60回も描画してるの？」と思うかもしれません。

そこで、乱数を使用して描画する毎に画面の色を変更してみましょう。Javaでの乱数生成は、下記のメソッドを使用します。

//list[][0.0〜1.0の乱数を生成する]{
Math.random()
//}

== プリミティブの描画を行う

OpenGL ES 2.0は、「プログラマブルパイプライン」という仕組みを導入しています。これはプログラマが描画の仕組みを「プログラマブル」に、つまりプログラムによって柔軟に書き換えが行えるということです。ですがそれと同時に、プログラマに対してその「パイプラインを構築する」という義務を課しています。

「やれること」が増えた代わりに、「やらなければいけないこと」もまた増えたわけです。

Hello Worldのプログラムから多くの変更がありますが、一つ一つはあまり大きくありません。


=== 演習1: 三角形を描画する

//listnum[][Chapter01_02.java]{

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

        ES20Util.assertGL();
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
        // attr_posを有効にする
        glEnableVertexAttribArray(attr_pos);
====================================================== [9] ここまで

====================================================== [10] ここから
        // ポリゴン色をアップロードする
        // 色はRGBAでアップロードする
        glUniform4f(unif_color, 1.0f, 0.0f, 0.0f, 1.0f);
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

OpenGL ES 2.0では、「GPUで実行される、描画専用のプログラム」を最低2種類記述する必要があります。それが「頂点シェーダー」と「フラグメントシェーダー」です。フラグメントシェーダーは、「ピクセルシェーダー」と言い換えればご存知の方もいるかもしれません。フラグメントシェーダーとピクセルシェーダーはどちらも特に違いはありませんが、OpenGL ESではピクセルを「フラグメント」と呼ぶため、フラグメントシェーダーと名付けられています。

ただし、このフラグメントとはAndroidプログラミング的な意味のandroid.app.Fragmentクラスとは全く意味合いが異なるため、注意してください。グラフィックの章内でカタカナ表記の「フラグメント」とはOpenGL ESのピクセルを示し、英語表記のFragmentとはAndroid SDKに含まれるandroid.app.Fragmentクラスを示します。

[1][3][4][5]の箇所が、シェーダーの生成とプログラム内への保持を行っている箇所です。まずは頂点シェーダー・フラグメントシェーダーのプログラムを見てみましょう。

//list[][頂点シェーダー]{
            final String vertexShaderSource =
                            "attribute mediump vec4 attr_pos;" +
                            "void main() {" +
                            "   gl_Position = attr_pos;" +
                            "}";
//}

//list[][フラグメントシェーダーシェーダー]{
            final String fragmentShaderSource =
                            "uniform lowp vec4 unif_color;" +
                            "void main() {" +
                            "   gl_FragColor = unif_color;" +
                            "}";
//}

これらは全てString型の変数、つまりただの文字列としてJava言語のプログラム内に保持されています。もちろん、Javaがコンパイルされてアプリとなっても文字列は文字列のままで、このままでは実行できません。

OpenGL ES 2.0のシェーダーはGPUで動作させる必要がありますが、GPUは無数に種類があり、なおかつ機械語単位で見ると互換性がありません。そのため、GPUで実行するプログラムをコンパイルするためには、そのGPUが載っている端末でコンパイルを行わなければならないのです@<fn>{シェーダー事前コンパイル}。

//footnote[シェーダー事前コンパイル][厳密に言えば、事前コンパイルが出来ますが機種ごとに互換性が失われるためAndroidではあまり使用されていません]

シェーダーは「GLSL ES」というC言語に似た言語で記述することができ、エントリーポイントとなるのは"main"関数です。このプログラムはC言語に似ていますが、GPUにより同時並列的な実行が行われるため、様々な制約があります。ですが、まずは細かいことを考えずに少しずつ理解していってください。

シェーダーで最初に覚えるべきは２つあります。

一つは、「頂点シェーダーでは必ずgl_Positionに頂点座標を書き込むこと」。もう一つは、「gl_FragColorに最終的なフラグメント色を書き込むこと」です。

サンプルの頂点シェーダーは「gl_Positionに対し、Java側のプログラムから与えられたattr_posを代入する」ことを行い、フラグメントシェーダーでは「gl_FragColorに対し、Java側のプログラムから与えられたunif_colorを代入する」という処理を行います。

[4]の部分では頂点シェーダーとフラグメントシェーダーのコンパイルを行っています。ES20Util.compileShaderは筆者が用意した補助メソッドです。内部ではコンパイルとエラーチェックを行っています。詳細な解説を行うためにはページ数が足りませんので、興味がある方は実装を覗いてみると良いでしょう。

シェーダーのコンパイル結果はそれぞれint型の変数として取得できます。OpenGL ESはオブジェクト指向のAPIではなく、OpenGL ESが管理するメモリは全て"ID"が割り振られ、そのIDによって管理する必要があります。

ですが、まだこの状態では実行をすることが出来ません。例えばAndroidアプリは".java"拡張子のプログラムを記述し、コンパイルによって".class"ファイルが出来上がります。ですが、実際にAndroid端末上で実行するためにはそれらを結合して".dex"ファイルに変換しなければなりません。これらは(厳密に言えば違いますが)"コンパイル"と"リンク"という別々の作業であり、そのどちらも実行には欠かすことが出来ません。

Eclipseのような統合開発環境はそれらを隠蔽しているため、「コンパイル＝実行できる状態にする」ように見えますが、実際には複数の処理を行っているのです。

「コンパイル〜実際に実行可能な状態にする」という一連の流れを、一般的に"ビルド(build)"と呼びます。

シェーダーも同じように、リンクを行わなければ実行することが出来ません。リンクを行っているのが[5]のES20Util.linkShaderメソッドです。ES20Util.compileShaderとES20Util.linkShaderはどちらも定型的な処理で、エラー処理を除けばほぼ書き換えることはありません。

[2][6]ではシェーダーから変数の"Location(番号)"を取得します。シェーダーはGPUという特殊な環境下で動作する特殊なプログラムです。そのため、Java側のコードから値をアップロードするためには、変数の管理番号を取得して「何番の変数にこの値を入力してください」という命令を行わなければなりません。


//list[][頂点シェーダーで定義している変数]{
attribute mediump vec4 attr_pos;
//}

//list[][フラグメントシェーダーで定義している変数]{
uniform lowp vec4 unif_color;
//}

このサンプルでシェーダーで宣言している変数は2つです。attr_posとunif_colorにそれぞれ値をアップロードしなければ、シェーダーは正常に計算を行うことが出来ません。そしてアップロードを行うためにはLocationを取得するしか無く、Locationを取得したら使用するためにはどこかに保持して置かなければなりません。

取得したLocationを保持しているのがメンバ変数attr_posとunif_colorです。今回はそれぞれシェーダー内の変数名と同じ変数名をそれぞれ名づけていますが、この名前はプログラマがわかりやすければ何でも構いません。

Locationを取得するためにはそれぞれ"glGetAttribLocation"と"glGetUniformLocation"を使用する必要があります。どちらも第1引数は取得を行うプログラムオブジェクト、第2引数は変数名です。正常にLocationが取得できると0以上の値が返却されます。もし変数名が間違っていたり、使用されていない変数の場合は"-1"がエラーとして返却されます。

最後に[7]の"glUseProgram"コマンドを使用し、「このシェーダーで描画を行いますよ」とGPUに対して宣言します。

これで初期化は完了です。



==== TRY: 端末の縦横を切り替えてみよう

==== TRY: 三角形の色を変えてみよう

==== TRY: 三角形の大きさや位置を変更しよう

==== TRY: Viewportを変更しよう

==== TRY: ES20Util.wrapの呼び出し回数を最小限にしよう

==== CHALLENGE: "glUniform4f"やassertの部分を変更せずに、シェーダーだけで任意の色のポリゴンを表示させてみよう

=== 演習2: 四角形を描画する

//listnum[][Chapter01_03.java]{
public class Chapter01_03 extends Chapter01_02 {
    @Override
    public void onDrawFrame(GL10 gl) {
        中略...

        {

====================================================== [1] ここから
            final float[] position = {
                    // triangle 0
                    // v0(left top)
                    -0.75f, 0.75f,
                    // v1(left bottom)
                    -0.75f, 0.25f,
                    // v2(right top)
                    -0.25f, 0.75f,

                    // triangle 1
                    // v3(right top)
                    -0.25f, 0.75f,
                    // v4(left bottom)
                    -0.75f, 0.25f,
                    // v5(right bottom)
                    -0.25f, 0.25f,
            };


            GLES20.glVertexAttribPointer(attr_pos, 2, GLES20.GL_FLOAT, false, 0, ES20Util.wrap(position));
            GLES20.glDrawArrays(GLES20.GL_TRIANGLES, 0, 6);
====================================================== [1] ここまで
        }


        {
====================================================== [2] ここから
            final float[] position = {
                    // v0(left top)
                    0.25f, 0.75f,
                    // v1(left bottom)
                    0.25f, 0.25f,
                    // v2(right top)
                    0.75f, 0.75f,
                    // v3(right bottom)
                    0.75f, 0.25f,
            };


            GLES20.glVertexAttribPointer(attr_pos, 2, GLES20.GL_FLOAT, false, 0, ES20Util.wrap(position));
            GLES20.glDrawArrays(GLES20.GL_TRIANGLE_STRIP, 0, 4);
====================================================== [2] ここまで
        }


        {
====================================================== [3] ここから
            final float[] position = {
                    // v0(right top)
                    -0.25f, -0.25f,
                    // v1(left top)
                    -0.75f, -0.25f,
                    // v2(left bottom)
                    -0.75f, -0.75f,
                    // v3(right bottom)
                    -0.25f, -0.75f,
            };


            GLES20.glVertexAttribPointer(attr_pos, 2, GLES20.GL_FLOAT, false, 0, ES20Util.wrap(position));
            GLES20.glDrawArrays(GLES20.GL_TRIANGLE_FAN, 0, 4);
====================================================== [3] ここまで
        }
    }
}

//}


==== TRY: 五角形や六角形を描画してみよう

==== CHALLENGE: 可能な限りきれいな「円」を描画してみよう

== VRAMの利用

=== 演習1: テクスチャを読み込む

//listnum[][Chapter01_04.java]{
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
        // unif_textureへテクスチャを設定する
        glBindTexture(GL_TEXTURE_2D, texture);
        glUniform1i(unif_texture, 0);
====================================================== [6] ここまで

        {
            final float[] position = {
                    // v0(left top)
                    -0.75f, 0.75f,
                    // v1(left bottom)
                    -0.75f, -0.75f,
                    // v2(right top)
                    0.75f, 0.75f,
                    // v3(right bottom)
                    0.75f, -0.75f,};

====================================================== [7] ここから
            final float[] uv = {
                    // v0(left top)
                    0, 0,
                    // v1(left bottom)
                    0, 1,
                    // v2(right top)
                    1, 0,
                    // v3(right bottom)
                    1, 1,};


            glVertexAttribPointer(attr_pos, 2, GL_FLOAT, false, 0, ES20Util.wrap(position));
            glVertexAttribPointer(attr_uv, 2, GL_FLOAT, false, 0, ES20Util.wrap(uv));
====================================================== [7] ここまで
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        }
    }
}

//}

==== TRY: シェーダーを使ってテクスチャの「ネガ」を描画してみよう

==== TRY: UV座標を操作してテクスチャの一部を描画してみよう
