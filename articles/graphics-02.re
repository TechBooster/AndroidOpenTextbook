= 3D描画への基礎知識

== シェーダーでのシンプルな計算

=== 演習：ポリゴンを平行移動させる

このサンプルコードはポリゴンを平行移動させます。順番に見ていきましょう（@<list>{Chapter02_01}）。

//listnum[Chapter02_01][Chapter02_01.java]{
public class Chapter02_01 extends Chapter01_01 {
    中略...

    /**
     * 平行移動
     * unif_translate
     */
    protected int unif_translate;

    中略...

    /**
     * X方向の平行移動量
     */
    protected float translateX;

    /**
     * Y方向の平行移動量
     */
    protected float translateY;

    /**
     * Surfaceが生成されたタイミングの処理
     */
    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        {
====================================================== [1] ここから
            final String vertexShaderSource =
                            "uniform mediump vec4 unif_translate;" +
                            "attribute mediump vec4 attr_pos;" +
                            "void main() {" +
                            "   gl_Position = attr_pos + unif_translate;" +
                            "}";
====================================================== [1] ここまで

            final String fragmentShaderSource =
                            "uniform lowp vec4 unif_color;" +
                            "void main() {" +
                            "   gl_FragColor = unif_color;" +
                            "}";

            // コンパイルとリンクを行う
            this.program = ES20Util.compileAndLinkShader(vertexShaderSource, fragmentShaderSource);
        }

        // attributeを取得する
        {
            attr_pos = glGetAttribLocation(program, "attr_pos");
            assert attr_pos >= 0;

            unif_color = glGetUniformLocation(program, "unif_color");
            assert unif_color >= 0;

            unif_translate = glGetUniformLocation(program, "unif_translate");
            assert unif_translate >= 0;
        }


        glUseProgram(program);
        ES20Util.assertGL();
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
        glViewport(0, 0, width, height);
    }

    /**
     * 毎描画時の処理
     */
    @Override
    public void onDrawFrame(GL10 gl) {
        glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL10.GL_COLOR_BUFFER_BIT);

        // attr_posを有効にする
        glEnableVertexAttribArray(attr_pos);

        // ポリゴン色をアップロードする
        // 色はRGBAでアップロードする
        glUniform4f(unif_color, 1.0f, 0.0f, 0.0f, 1.0f);

====================================================== [2] ここから
        // 平行移動を行う
        {
            glUniform4f(unif_translate, translateX, translateY, 0, 0);

            translateX += 0.01f;
            translateY += 0.005f;

            // 適当なところで元に戻す
            if (translateX > 1) {
                translateX = 0;
            }
            if (translateY > 1) {
                translateY = 0;
            }
        }
====================================================== [2] ここまで


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
        glDrawArrays(GL_TRIANGLES, 0, 6);
====================================================== [3] ここまで
        // デバッグ用メッセージを表示する
        SampleUtil.setDebugText(getActivity(), String.format("translate(%.2f, %.2f)", translateX, translateY));
====================================================== [3] ここまで
    }
}
	
//}

==== シンプルな平行移動を行う

このサンプルでは、頂点シェーダーに「頂点の座標を移動させるためのuniform変数」を追加しています。静的なデータである「頂点情報」を移動させる手段はいくつもあります。その中で最もシンプルな方法は、「任意の値だけ座標を加算する」というものです。

具体的に、頂点シェーダーでは"gl_Position"に頂点位置を書き込む際、uniform変数unif_translateと加算を行っています（@<list>{gl_PositionWrite}）。

//list[gl_PositionWrite][gl_Position書き込み部分]{
gl_Position = attr_pos + unif_translate;
//}

[2]では描画ごとに"unif_translate"に対してアプリが持つメンバ変数"translateX"と"translateY"の値をアップロードしています。

それと同時に、translateX/translateYは少しずつ値が変化しています。「少しずつ値を変化して連続描画する」というのは、"アニメーション"を実現するために必要となる技術です。

なお、サンプルでは少しずつX座標とY座標を加算していますが、それを続けているといつしか画面外へ四角形が消えていってしまいます（@<list>{translateXY}）。

//list[translateXY][四角形の移動処理]{
translateX += 0.01f;
translateY += 0.005f;
//}

ずっと消えていったままでは寂しいですので、ある程度値が大きくなったところで値をリセットする処理も入れています（@<list>{reset}）。

//list[reset][値のリセット処理]{
if (translateX > 1) {
    translateX = 0;
}

if (translateY > 1) {
    translateY = 0;
}  
//}

最後に、このサンプル以後は「アニメーション」として毎秒60回も変換する変数が登場します。そのような場合、ブレークポイントで値を追いかけるのは非常に効率が悪い場合があります。ブレークポイントというのはある「瞬間」値を捉えますが、アニメーションは「瞬間の繰り返し」によって完成します。

そのため、[3]では常に値を画面に出力することで、値の変化をわかりやすくしています。

SampleUtil.setDebugTextメソッドは、Activity内にあるデバッグ用TextViewに対してsetTextを呼び出すだけのシンプルなメソッドです（@<list>{setDebugText}）。この表示を行うことで、translateの値が加算されるにしたがって少しずつ四角形が動いているということがわかりやすくなるのでは無いでしょうか。

//list[setDebugText][デバッグ用メッセージ表示部分]{
SampleUtil.setDebugText(getActivity(), String.format("translate(%.2f, %.2f)", translateX, translateY));
//}

== 行列演算を行う

3D描画にとって"行列（Matrix）"は欠かせない存在です。高校数学では学科によっては学習しますが、そうでない場合もあります。

そのため、もし現時点で"行列"を知らなかったとしても恥ずかしいことではありません。行列とは、図のように「縦横に数字を並べたもの」とまずは考えてください。

行列の基本的な形は、「斜めの要素がすべて1.0、それ以外は0.0」です。この状態を"単位行列（Identity）"と呼びます。

#@# 行列の画像

OpenGL ESでは、多くの場合横4つ、縦4つに数字を並べた4×4行列を使用します。その他にも縦3つ、横3つ並べた3×3行列、同様に2×2行列も使用できます。この時、縦横の数は一致しなければなりません。プログラム上、これらはfloat型数値の配列として表されることが多いです。

シェーダーの処理で特に重要になるため、"mat4"、"mat3"、"mat2"型等が用意されています。

行列の特徴として、「頂点を3D空間内で任意座標に簡単に移動できる」というものがあります。移動というとシンプルなのは前述のような「加算処理」ですが、それ以外にも「回転」「拡大縮小」という処理も「頂点を任意座標に移動させる」といえます。

行列はたとえるなら「便利な計算機」です。頂点座標をその「計算機」に渡す（乗算する、適用するといいます）と、自動的に任意の座標に移動した状態の行列が取り出せます。計算機の内部構造は知らなくてもプログラミングは出来ます。

Androidには行列演算を簡単に行うためのメソッドが大量に用意されているため、Androidで開発を行う限りは行列の複雑な処理を覚える必要はありません。単に「行列の使い方」に注力して覚えてください。

=== 演習：行列で平行移動を行う

移動（Translate）を行うための行列は次のようになります（@<list>{hapter02_02}）。

#@# 図を入れる

//listnum[Chapter02_02][Chapter02_02.java]{

public class Chapter02_02 extends Chapter01_01 {
    中略...

    /**
     * 頂点に適用する行列
     * unif_matrix
     */
    protected int unif_matrix;

    中略...

    /**
     * Surfaceが生成されたタイミングの処理
     */
    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        {
            final String vertexShaderSource =
====================================================== [1] ここから
                            "uniform mediump mat4 unif_matrix;" +
                            "attribute mediump vec4 attr_pos;" +
                            "void main() {" +
                            "   gl_Position = unif_matrix * attr_pos;" +
====================================================== [1] ここまで
                            "}";

            final String fragmentShaderSource =
                            "uniform lowp vec4 unif_color;" +
                            "void main() {" +
                            "   gl_FragColor = unif_color;" +
                            "}";

            // コンパイルとリンクを行う
            this.program = ES20Util.compileAndLinkShader(vertexShaderSource, fragmentShaderSource);
        }

        // attributeを取得する
        {
            attr_pos = glGetAttribLocation(program, "attr_pos");
            assert attr_pos >= 0;

            unif_color = glGetUniformLocation(program, "unif_color");
            assert unif_color >= 0;

            unif_matrix = glGetUniformLocation(program, "unif_matrix");
            assert unif_matrix >= 0;
        }

        glUseProgram(program);
        ES20Util.assertGL();
    }

    中略...

    @Override
    public void onDrawFrame(GL10 gl) {

        中略...

        // 平行移動を行う
        {
====================================================== [2] ここから
            float[] matrix = ES20Util.createMatrixIdentity();
            matrix[4 * 3 + 0] = translateX;
            matrix[4 * 3 + 1] = translateY;
            glUniformMatrix4fv(unif_matrix, 1, false, matrix, 0);
====================================================== [2] ここまで

            translateX += 0.01f;
            translateY += 0.005f;

            // 適当なところで元に戻す
            if (translateX > 1) {
                translateX = 0;
            }
            if (translateY > 1) {
                translateY = 0;
            }
        }


        中略...
    }
}

//}

[1]の部分では、頂点シェーダーでmat4型変数"unif_matrix"を追加しています。行列は通常、「頂点ごとに違う内容」ではなく、「頂点すべてに同じ行列を適用」しなければなりません。そのため、uniform変数として頂点シェーダーに渡すのが基本となります。

unif_matrixは次のようにattr_posと乗算されて、gl_Positionに渡されます。このとき、unif_matrixを「前方」、attr_posを「後方」に配置して計算してください（@<list>{unif_matrix}）。

//list[unif_matrix][行列と頂点座標を乗算する]{
gl_Position = unif_matrix * attr_pos;
//}

この計算は「掛け算」ですが、行列計算に交換法則（A×B＝B×A）は成り立たず、掛け算の順番によって計算結果が全く異なってしまいます。行列は便利ですが、順番を間違えただけであっさりと想定外の動作をするため、計算順には常に細心の注意が必要です。

行列の計算は必ず「先に適用したいものほど右側に配置する」と覚えてください。

[2]では実際の行列生成とアップロードを行っています。Android SDKでプログラムを組む際、行列はfloat型の１次元配列となります。行列は見た目上2次元配列ですが、GPUにアップロードするという都合上１次元配列でないと問題が発生するためです@<fn>{行列の次元数}。

//footnote[行列の次元数][C言語ではメモリの配置がコントロールできるため、2次元配列を使用できます。]

単位行列の生成処理は非常にシンプルなため、サンプルではUtilメソッドを提供しています（@<list>{Identify}）。

//list[Identify][単位行列の生成]{
    public static float[] createMatrixIdentity() {
        return new float[]{
                1, 0, 0, 0,
                0, 1, 0, 0,
                0, 0, 1, 0,
                0, 0, 0, 1,
        };
    }
//}

移動行列の生成自体も非常に簡単なため、サンプルではUtilメソッドを使用せずに直接配列に書き込んでいます。行列の次の箇所へ「X移動量」「Y移動量」「Z移動量」を書き込むだけです。

#@# 図を入れる

図は２次元ですが、Java言語上では１次元配列となるため、配列のindexを指定する際に少しだけ工夫が必要です。ですが非常に簡単なため、覚えることは容易でしょう。

今回は２D描画ですので、translateXとtranslateYをそれぞれ所定のindexへ書き込んでいます（@<list>{matrix}、@<list>{matrix2}）。

//list[matrix][一次元配列を使用する際の書き込み位置の計算方法]{
matrix[4 * 縦のインデックス + 横のインデックス];
//}

//list[matrix2][2D移動行列の作成]{
matrix[4 * 3 + 0] = translateX;
matrix[4 * 3 + 1] = translateY;
//}

最後に、行列をシェーダーへアップロードすれば完了です。行列のアップロードは"glUniformMatrix4fv"コマンドで行います。第１引数はアップロード先のLocation。第２引数はアップロードする行列の個数（今回は１つなので"1"）。第３引数は転置有無を指定しますが、必ずfalseを指定します。第４引数は行列を示した配列を指定します。第５引数は例によって配列のオフセットを指定しますが、ここでは"0"を指定します。


=== 演習：ポリゴンを行列で拡大縮小させる

次のサンプルでは、ポリゴンを拡大縮小させています（@<list>{Chapter02_03}）。注目すべきは、シェーダー部分に一切の変更を加えないということです。行列の内容を変化させるだけで、「移動」ではなく「拡大縮小」を行わせています。

拡大縮小（Scale）を行うための行列は次の図のようになります。これも移動行列と同じく非常にシンプルで、斜め方向に拡大縮小の「倍率」を指定するだけです。普段（Identity）では1.0のため、拡大縮小が行われません（数値に1.0を乗算しても元の数字のままですね）。

#@# 図を入れる

//listnum[Chapter02_03][Chapter02_03.java]{
public class Chapter02_03 extends Chapter01_01 {
    
    中略...

    /**
     * X方向の拡大縮小
     */
    protected float scaleX = 1;

    /**
     * X方向の拡大縮小
     */
    protected float scaleY = 1;

    中略...

    /**
     * 毎描画時の処理
     */
    @Override
    public void onDrawFrame(GL10 gl) {
        
        中略...

        // 平行移動を行う
        {
            float[] matrix = ES20Util.createMatrixIdentity();
====================================================== [1] ここから
            matrix[4 * 0 + 0] = scaleX;
            matrix[4 * 1 + 1] = scaleY;
====================================================== [1] ここまで
            glUniformMatrix4fv(unif_matrix, 1, false, matrix, 0);

====================================================== [2] ここから
            scaleX -= 0.01f;
            scaleY -= 0.005f;
====================================================== [2] ここまで
        }


        中略...

    }
}
//}

[1]の部分では、拡大縮小用の行列を作成しています。これは前述のとおり、行列の所定位置に拡大率を指定するだけです。1.0より大きければ拡大、1.0より小さければ縮小となります。

[2]の部分では、拡大率を少しずつ小さくしています。当然、この計算を続ければ0よりも小さくなりますが、計算してもエラーにはなりません。なぜなら、負の値を乗算したら「座標の正負が反転する」だけで計算エラーにはならないからです。

また、行列は内部的に"乗算"と"加算"しか行わないため、計算結果が不正になることは基本的にありません@<fn>{行列の不正な演算}。

//footnote[行列の不正な演算][NaNやInfinity等を指定した場合、不正な行列が生成される場合があります]

=== 演習：ポリゴンを行列で回転させる

ポリゴンの回転(Rotate)は、前２つの行列に比べて非常に複雑です。初見では行列の式を見ただけで「関わりたくない」と思うかもしれません。実際のところ、Androidではサポート用のメソッドが用意されているためこの式を覚える必要もなく、それを呼び出すだけで済みます（@<list>{Chapter02_04}）。

ですので、図は参考程度に考えてください。

#@# 図を入れる

//listnum[Chapter02_04][Chapter02_04.java]{

public class Chapter02_04 extends Chapter01_01 {

    中略...

    /**
     * 回転量
     */
    protected float rotate;

    中略...

    @Override
    public void onDrawFrame(GL10 gl) {
        
        中略...

        // 回転を行う
        {
            float[] matrix = ES20Util.createMatrixIdentity();
====================================================== [1] ここから
            Matrix.setRotateM(matrix, 0, rotate, 0, 0, 1.0f);
====================================================== [1] ここまで
            glUniformMatrix4fv(unif_matrix, 1, false, matrix, 0);

            rotate += 1;
        }

        中略...

    }
}

//}

回転行列の生成は"android.opengl.Matrix"クラスのstaticメソッド"setRotateM"で行えます。Androidでは別packageに同名の"android.graphics.Matrix"クラスが存在するため、importの指定には注意してください。

回転行列の引数は４つあり、指定方法が独特です。今は2Dの回転であるため、内容はシンプルに考えられます。しかし、このメソッドは本来３次元空間での回転を行うためにあります。

３次元空間にはX/Y/Zの軸がそれぞれあります。X軸で回転するというのは、鉄棒の前回りや逆上がりを行うようなものです。Y軸での回転はコマの回転と同じです。Z方向の回転とは、壁時計の短針や長針のような動きになります。

さて、もう一度考えてください。３次元空間の回転軸の種類とはXYZの３種類だけでしょうか？

答えはNOで、たとえば地球の地軸は公転面に対して傾いています。XYZだけでなく、そのような傾いた軸も３次元空間では扱わなければなりません。

そこで、setRotateMメソッドでは「回転軸の方向ベクトル」と「回転量」で回転行列を生成します。方向ベクトルとは、「ある地点からある地点への向き」を示したベクトル（XYZの値）です。あくまで「向き」であるため、物体がどれだけ離れていてもベクトルは同じです。

たとえば、あなたが建物の1階にいるとして、2階にいる人も3階にいる人も、60階にいる人も、果ては屋上にいたとしても「上にいる」という意味で同じです。方向だけを指し示す場合、「距離」は関係ないのです。

そのため、方向ベクトルは長さ（Xの二乗＋Yの二乗＋Zの二乗）の値が必ず1.0になります。

前置きが長くなりましたが、2Dの回転は前述の軸でいえば「時計の針」と同じZ軸の回転です。Z軸の方向ベクトルはXYZそれぞれ（0, 0, 1）となります（@<list>{setRotateM}）。

//list[setRotateM][回転行列の生成]{
Matrix.setRotateM(matrix, 0, rotate, 0, 0, 1.0f);
//}

Matrix.setRotateMの第１引数には結果を格納する配列、第２引数は配列のオフセットを指定します。第３引数は回転量を指定します。これはdegree、つまり360度系です。一般的なプログラミングの回転量はラジアンで表されることが多いですが、このメソッドは360度系が採用されていることに注意してください。

第４引数以降が回転軸の方向ベクトルです。前述の図のような計算はすべてメソッド内部に隠ぺいされているため、シンプルに使用できます。

==== TRY：X軸・Y軸の回転を試してみよう

前述のとおり、回転軸は無数にあります。X軸、Y軸、そしてその他の「斜め」の軸で回転させ、どのような見た目になるか確認してみましょう。

== OpenGL ESにとっての「カメラ」

=== 演習：カメラを行列で指定する

今までは「ポリゴンを何らかの場所へ移動させる」ための行列作成を行ってきました。ポリゴン自体を明確に動かすだけでなく、「3D空間内にカメラを置いて、撮影する」という処理も行列によって記述できます。

言い方を変えれば、「ポリゴンをカメラから見た相対的な位置へ移動させる」という処理も行列で記述できます（@<list>{Chapter02_05}）。

//listnum[Chapter02_05][Chapter02_05.java]{
public class Chapter02_05 extends Chapter01_01 {

    中略...

    /**
     * 頂点に適用するワールド行列
     * unif_matrix
     */
    protected int unif_world;


    /**
     * look at行列
     */
    protected int unif_look;

    /**
     * 射影行列
     */
    protected int unif_projection;

    中略...

    /**
     * カメラ位置
     * 初期位置は適当に決める
     */
    protected Vector3 cameraPos = new Vector3(10, 3, 10);

    
    中略...

    /**
     * スクリーン幅
     */
    protected float screenWidth;

    /**
     * スクリーン高
     */
    protected float screenHeight;

    /**
     * Surfaceが生成されたタイミングの処理
     */
    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        {
            final String vertexShaderSource =
====================================================== [1] ここから
                            "uniform mediump mat4 unif_world;" +
                            "uniform mediump mat4 unif_look;" +
                            "uniform mediump mat4 unif_projection;" +
                            "attribute mediump vec4 attr_pos;" +
                            "void main() {" +
                            "   gl_Position = unif_projection * unif_look * unif_world * attr_pos;" +
                            "}";
====================================================== [1] ここまで

            中略...
        }

        // attributeを取得する
        {
            attr_pos = glGetAttribLocation(program, "attr_pos");
            assert attr_pos >= 0;

            unif_color = glGetUniformLocation(program, "unif_color");
            assert unif_color >= 0;

            unif_world = glGetUniformLocation(program, "unif_world");
            assert unif_world >= 0;

            unif_look = glGetUniformLocation(program, "unif_look");
            assert unif_look >= 0;

            unif_projection = glGetUniformLocation(program, "unif_projection");
            assert unif_projection >= 0;
        }

        glUseProgram(program);
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
        glViewport(0, 0, width, height);
====================================================== [2] ここから
        this.screenWidth = width;
        this.screenHeight = height;
====================================================== [2] ここまで
    }

    /**
     * ワールド行列設定
     */
    private void setupWorld() {
        float[] matrix = ES20Util.createMatrixIdentity();
        Matrix.setRotateM(matrix, 0, rotate, 0, 0, 1.0f);
        glUniformMatrix4fv(unif_world, 1, false, matrix, 0);

        rotate += 1;
    }

    /**
     * カメラ情報のセットアップを行う
     */
    private void setupCamera() {
====================================================== [3] ここから
        // カメラ注視
        float cameraLookX = 0;
        float cameraLookY = 0;
        float cameraLookZ = 0;
====================================================== [3] ここまで

====================================================== [4] ここから
        // カメラ天地
        float cameraUpX = 0;
        float cameraUpY = 1;
        float cameraUpZ = 0;
====================================================== [4] ここまで

====================================================== [5] ここから
        // 画角
        float fovY = 45.0f;
====================================================== [5] ここまで

====================================================== [6] ここから
        // アスペクト比
        float aspect = screenWidth / screenHeight;

        // ニアクリップ/ファークリップ
        float nearClip = 1.0f;
        float farClip = 100.0f;
====================================================== [6] ここまで

====================================================== [7] ここから
        {
            // look行列を生成する
            float[] matrix = ES20Util.createMatrixIdentity();
            Matrix.setLookAtM(matrix, 0, cameraPos.x, cameraPos.y, cameraPos.z, cameraLookX, cameraLookY, cameraLookZ, cameraUpX, cameraUpY, cameraUpZ);

            // アップロード
            glUniformMatrix4fv(unif_look, 1, false, matrix, 0);

            // カメラを移動する
            cameraPos.x -= 0.01f;
            cameraPos.z -= 0.005f;
        }
====================================================== [7] ここまで

====================================================== [8] ここから
        {
            // projection行列を生成する
            float[] matrix = ES20Util.createMatrixIdentity();
            Matrix.perspectiveM(matrix, 0, fovY, aspect, nearClip, farClip);

            // アップロード
            glUniformMatrix4fv(unif_projection, 1, false, matrix, 0);
        }
====================================================== [8] ここまで

        // デバッグ用メッセージを表示する
        SampleUtil.setDebugText(getActivity(), String.format("camera\n pos(%.2f, %.2f, %.2f)\n look(%.2f, %.2f, %.2f)\n fovy(%.1f) aspect(%.2f)",
                cameraPos.x, cameraPos.y, cameraPos.z,
                cameraLookX, cameraLookY, cameraLookZ,
                fovY, aspect));
    }

    @Override
    public void onDrawFrame(GL10 gl) {

        中略...

        // 各行列の設定を行う
        setupWorld();
        setupCamera();

        中略...
    }
}
//}

皆さんは、デジタルカメラを所有していますか。スマートフォン搭載のカメラだけではなく、「一眼レフ」や「ミラーレス一眼」、「コンパクトデジタルカメラ」と呼ばれる種類のカメラです。2014年現在は数千円〜と非常に安価に買えるため、一台〜数台程度所有している方が多いのではないでしょうか。

現実世界でデジカメの「撮影」を行う際に行う動作と、OpenGL ESにおけるカメラの処理は非常に似ています。そのため、まずは現実世界の「カメラ」について考えてみましょう。

3DCGというのは、仮想空間に浮かぶ「ジオラマ」のようなものです。限りある空間@<fn>{仮想空間の限界}の中に3DCGの「物体」を浮かべ、それを画面に映し出すことが3DCGにおける「描画」です。ですので、博物館にあるジオラマや、プラモデルを並べたジオラマを撮影することを想像してください。

//footnote[仮想空間の限界][GPUは浮動小数点演算を行います。精度は4byte程度ですので、それ以上の広大な空間を扱うことが出来ません。]

あなたは撮影をする際、まずは配置すべき"建物"や"人形"といったミニチュアを作成します。時には数十種類ものミニチュア(Model)を用意しなければなりません。現実世界では"プロモデラー"という方々や造形師がこれを行います。ゲーム開発等では、これは3dsMaxやMayaといった専用のツールを使った上で、デザイナーがモデルのデータを作成します。

次に、箱庭の中に"建物"や"人形"を「適切に配置する」必要があるでしょう。これは作られたデータを先ほどの"移動行列"、"回転行列"、"拡大縮小行列"を駆使して配置する作業です。これが今まで行っていた行列の作業です。

これでまず被写体となるジオラマが出来上がりました。OpenGL ESでいえば、行列によって「どこに配置するか」を確定できたということです。

ですが、まだ撮影を行えません。なぜなら、カメラの準備がまったく終わっていないからです。

撮影を行うためには、まずカメラを三脚に固定します。つまり「位置を決め」ます。

次に、被写体に対してカメラを向けます。つまり「注視場所を決め」ます。

さらに、ズームイン・ズームアウトをして被写体をどれくらいの大きさで表示させるか考えるでしょう。一眼レフで言うならば、小さいレンズからバズーカ砲と呼ばれるサイズまで、自在にレンズを切り替えたりするでしょう。これらはすべて「画角を決定」「レンズ設定の決定」に相当します。

シャッターを押すと、その情景をイメージセンサーが捉えます。これがglDrawArraysコマンドに相当します。カメラとジオラマとの位置関係によって、当然ながら映り込む内容が異なります。これは「相対的な位置が違う」といえます。この相対的な座標を計算するのが頂点シェーダーであり、行列です。

もしかしたら、あなたのデジカメには「セピア色で撮影」等のモードが付いているかもしれません。これはフラグメントシェーダーの計算処理です。

今回のシェーダーから、行列が3つに増えています。そのため、今までは単純に「unif_matrix」と種別のない書き方をしていましたが、今回からは"unif_world"、"unif_look"、"unif_projection"という３つの行列が登場しています。

World行列は、「ジオラマの中の配置」を担当します。これは今までのunif_matrixに相当します。

Look行列は、「カメラの配置」を担当します。

Projection行列は、「カメラのレンズ」を担当します。

まずは[1]のシェーダーに注目してください。gl_Positionの計算方法は、次のようになっています。前述のように、シェーダー内の計算は「先に適用するものを右に書く」という法則があります。

現実世界での処理順番は、例に倣うと次のとおりです（@<list>{unif_projection}）。カッコ内はOpenGL ESの相当箇所です。それらがすべて「右から順番に」記述されているのがわかりますね。

 1. モデルの作成（attr_pos）
 1. モデルの配置（World行列）
 1. カメラの位置決め（Look行列）
 1. カメラのレンズ設定（Projection行列）

//list[unif_projection][gl_Positionの計算部分]{
gl_Position = unif_projection * unif_look * unif_world * attr_pos;
//}

[2]では、Viewportの高さと幅を保存しています。カメラでは端末の縦横サイズと「レンズ」の画角の両方を計算しなければならないため、ここではメンバ変数に保存を行っています。

[3]〜[8]の"setupCamera"メソッドが実際にカメラの行列を生成している部分です。

[3]の部分ではカメラが「どこを注視するか」を指定しています。今回はオブジェクトが原点（XYZ座標0, 0, 0）に配置されているため、常にカメラは原点を見つめるようになっています。

[4]の部分は、「カメラの天井方向」を指定しています。これは少しだけわかりにくいかもしれません。ですが、カメラで撮影をする際にカメラを横に持ったり、縦に持ったりしたことはあるかと思います。その「縦に持つ」「横に持つ」を表現するのがこの"天井方向"です。カメラを正しい向きで構えた状態は「カメラの天井が上を向いている」といえます。カメラを縦に構えた場合は、「カメラの天井が横方向を向いている」といえます。

もちろん、斜め等もこの天井方向として表現可能です。

[5]では、レンズの画角を指定します。サンプルでは45.0度の画角です。一眼レフやカメラのExif情報では"◯◯mm（ミリ）"として表現されることが多いですが、計算の都合上ここでは「角度」になっています。もちろん、ここで指定するのはdegreeです。

[6]はOpneGL ES独特の事情によるパラメータです。まず、カメラは最終的に「画面（Viewport）」へと映像を投影しなければなりません。シンプルに計算してしまうと、Viewportの解説を行った時のように「縦に広がる」「横に広がる」等の「歪み」が出てしまいます。

それを抑えるため、OpenGL ESのカメラには「画面(Viewport)のアスペクト比」を与えて、歪みを抑えるのです。

次に、ニアクリップとファークリップです。「3DCGの空間」という限りある空間を効率的に使用するためのパラメーターです。たとえば、「これ以上遠くには何も映っていない」「これ以上近くには何も無い」という情報をカメラに伝えれば、不要な計算をしなくて済みます。

ニアクリップよりも近くにあるピクセルは表示されず、ファークリップよりも遠くにあるピクセルもまた表示されません。

[7]の部分で実際にLook行列を生成しています。なお、カメラの位置はアニメーションさせる都合上メンバ変数として定義されています。その際のVector3型は筆者が提供しているクラスで、xyzの3要素があるだけの簡素なものです（@<list>{Vector3}）。

//list[Vector3][Vector3クラスの実装]{
public class Vector3 {
    public float x = 0;
    public float y = 0;
    public float z = 0;

    public Vector3(float x, float y, float z) {
        this.x = x;
        this.y = y;
        this.z = z;
    }
}
//}

@<list>{setLookAtM}にあるようにLook行列を作成するには、Matrix.setLookAtMメソッドを使用します。

//list[setLookAtM][Look行列を生成する]{
Matrix.setLookAtM(matrix, 0, cameraPos.x, cameraPos.y, cameraPos.z, cameraLookX, cameraLookY, cameraLookZ, cameraUpX, cameraUpY, cameraUpZ);
//}


第１、第２引数は結果を格納する配列を指定します。

第３～第５引数は、それぞれカメラ位置のXYZ座標を指定します。

第６～第８引数は、それぞれカメラの注視点のXYZ座標を指定します。

第９～第１１引数は、それぞれカメラの天井方向の方向ベクトルを指定します。

[8]ではProjection行列を生成しています（@<list>{perspectiveM}）。

//listnum[perspectiveM][Projection行列を生成する]{
Matrix.perspectiveM(matrix, 0, fovY, aspect, nearClip, farClip);
//}

第１、第２引数は結果を格納する配列を指定します。

第３引数は画角を指定します。この画角は"Y"というキーワードが付いているように、"Y(縦)方向の画角"です。たとえば、映画のように「横長の画面」では縦方向と横方向の画角が一致しません。そこで、OpenGL ESでは「縦方向の画角」を基本として使用します。

横方向の画角は第４引数である画面のアスペクト比により自動で計算されます。

第５引数はニアクリップを指定し、カメラから見てこの数値よりも近いオブジェクトは画面に映りません。

第６引数はファークリップを指定し、カメラから見てこの数値よりも近いオブジェクトは画面に映りません。

アップロードについてはいつものように"glUniformMatrix4fv"で行います。

この状態で描画を行うと、各種行列により「カメラから見た内容」のポリゴンが写し出されます。

=== TRY：ポリゴンにテクスチャを貼り付けてみよう

このままではあまりにシンプルで面白みに欠ける画面です。シェーダー等を工夫し、ポリゴンに再度テクスチャを貼り付けてみてください。

=== CHALLENGE：シェーダーに記述する行列を１つだけにしてみよう

サンプルでは頂点シェーダーに3つの行列が記述されています。これでも十分な速度で処理が行えますが、さらなる高速化のために頂点シェーダー内に記述する行列を1つだけにしてみましょう。


