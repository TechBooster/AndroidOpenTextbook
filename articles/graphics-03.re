= OpenGL ESの3D描画

前章までの知識さえあれば、すでに3DCGの複雑な描画さえ行うことが出来ます。ですが、「可能」というだけであって、それが現実的な描画速度を得られるまでには至っていませんでした。前章まででは、数万〜数十万ポリゴンにも及ぶ頂点やポリゴンを現実的な速度（１回の描画に16ms〜33ms）で行うことが出来ません。

この章では立方体を例に、より現実的な処理を行うための知識を提供します。

== 立方体での例

=== 課題：立方体を描画する

まずは「今までの知識」と少しだけの工夫を追加した状態で「テクスチャを貼り付けた立方体」を描画し、どのような問題点があるかを探ってみましょう（@<list>{Chapter03_01}）。

//listnum[Chapter03_01][Chapter03_01.java]{
public class Chapter03_01 extends Chapter01_01 {

    中略...

    /**
     * テクスチャUniform
     */
    protected int unif_texture;

    /**
     * テクスチャオブジェクト
     */
    protected int texture;

    中略...

    /**
     * Surfaceが生成されたタイミングの処理
     */
    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        {
            final String vertexShaderSource =
                            "uniform mediump mat4 unif_world;" +
                            "uniform mediump mat4 unif_look;" +
                            "uniform mediump mat4 unif_projection;" +
                            "attribute mediump vec4 attr_pos;" +
                            "attribute mediump vec2 attr_uv;" +
                            "varying mediump vec2 vary_uv;" +
                            "void main() {" +
                            "   gl_Position = unif_projection * unif_look * unif_world * attr_pos;" +
                            "   vary_uv = attr_uv;" +
                            "}";

            final String fragmentShaderSource =
                            "uniform sampler2D unif_texture;" +
                            "varying mediump vec2 vary_uv;" +
                            "void main() {" +
                            "   gl_FragColor = texture2D(unif_texture, vary_uv);" +
                            "}";

            // コンパイルとリンクを行う
            this.program = ES20Util.compileAndLinkShader(vertexShaderSource, fragmentShaderSource);
        }

        // attributeを取得する
        {
            attr_pos = glGetAttribLocation(program, "attr_pos");
            assert attr_pos >= 0;

            unif_world = glGetUniformLocation(program, "unif_world");
            assert unif_world >= 0;

            unif_look = glGetUniformLocation(program, "unif_look");
            assert unif_look >= 0;

            unif_projection = glGetUniformLocation(program, "unif_projection");
            assert unif_projection >= 0;

            attr_uv = glGetAttribLocation(program, "attr_uv");
            assert attr_uv >= 0;

            unif_texture = glGetUniformLocation(program, "attr_uv");
            assert attr_uv >= 0;
        }

        texture = ES20Util.loadTextureFromAssets(getActivity(), "sample512x512.png");
        assert texture != 0;

        glUseProgram(program);
    }

    中略...

====================================================== [1] ここから
    /**
     * サーフェイスの塗りつぶし処理
     */
    protected void clearSurface() {
        glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL10.GL_COLOR_BUFFER_BIT);
    }
====================================================== [1] ここまで

    /**
     * 毎描画時の処理
     */
    @Override
    public void onDrawFrame(GL10 gl) {
        // サーフェイスを単色で塗りつぶす
        clearSurface();

        中略...

        glBindTexture(GL_TEXTURE_2D, texture);

        {
            // キューブを構築する
            final float LEFT = -1.0f;
            final float RIGHT = 1.0f;
            final float FRONT = -1.0f;
            final float BACK = 1.0f;
            final float TOP = 1.0f;
            final float BOTTOM = -1.0f;

====================================================== [2] ここから
            final float[] cubeVertices = {
                    // 上下面
                    LEFT, BOTTOM, FRONT, 0, 0, RIGHT, BOTTOM, FRONT, 1, 0, LEFT, BOTTOM, BACK, 0, 1, //
                    LEFT, BOTTOM, BACK, 1, 0, RIGHT, BOTTOM, FRONT, 0, 1, RIGHT, BOTTOM, BACK, 1, 1, //
                    //
                    LEFT, TOP, FRONT, 0, 0, LEFT, TOP, BACK, 1, 0, RIGHT, TOP, FRONT, 0, 1, //
                    LEFT, TOP, BACK, 1, 0, RIGHT, TOP, BACK, 0, 1, RIGHT, TOP, FRONT, 1, 1, //

                    //左右面
                    RIGHT, TOP, FRONT, 0, 0, RIGHT, TOP, BACK, 1, 0, RIGHT, BOTTOM, FRONT, 0, 1, //
                    RIGHT, TOP, BACK, 1, 0, RIGHT, BOTTOM, BACK, 0, 1, RIGHT, BOTTOM, FRONT, 1, 1, //
                    //
                    LEFT, TOP, FRONT, 0, 0, LEFT, BOTTOM, FRONT, 1, 0, LEFT, TOP, BACK, 0, 1, //
                    LEFT, TOP, BACK, 1, 0, LEFT, BOTTOM, FRONT, 0, 1, LEFT, BOTTOM, BACK, 1, 1, //

                    // 前後面
                    LEFT, TOP, BACK, 0, 0, LEFT, BOTTOM, BACK, 1, 0, RIGHT, TOP, BACK, 0, 1, //
                    RIGHT, TOP, BACK, 1, 0, LEFT, BOTTOM, BACK, 0, 1, RIGHT, BOTTOM, BACK, 1, 1, //
                    //
                    LEFT, TOP, FRONT, 0, 0, RIGHT, TOP, FRONT, 1, 0, LEFT, BOTTOM, FRONT, 0, 1, //
                    RIGHT, TOP, FRONT, 1, 0, RIGHT, BOTTOM, FRONT, 0, 1, LEFT, BOTTOM, FRONT, 1, 1, //
            };
====================================================== [2] ここまで

====================================================== [3] ここから
            FloatBuffer buffer = ES20Util.wrap(cubeVertices);
            glVertexAttribPointer(attr_pos, 3, GL_FLOAT, false, 4 * 3 + 4 * 2, buffer);
            glVertexAttribPointer(attr_uv, 2, GL_FLOAT, false, 4 * 3 + 4 * 2, buffer.position(3)); // 注意:positionメソッドはbyte単位ではなく要素単位で指定する
====================================================== [3] ここまで
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
    }
}

//}

このサンプルは後々少しだけ修正するため、[1]の部分でglClearColor/glClearをメソッド化しています。ですが処理内容に変わりはありません。

[2]は、頂点の「位置情報」と「UV情報」を一つの配列に詰め込んでいます。これは"頂点構造体"という技術で、2つの配列から個別に情報を読み出すより、1つの配列からデータを一定の法則で読み出すほうがGPUにとって効率よく処理できます。

今回からは本格的な3D情報を扱うため、位置情報はXYZ座標の3要素、UV座標は今までどおり2要素、合計で5要素/頂点となります。

#@# 図を入れた方がいい

非常に効率よく処理できる反面、今回のサンプルでは36個もの頂点を構築するため、データが非常に見づらくなってしまっています。1頂点5要素×36頂点ですので、合計180個のデータが出来上がります。

情報量が多く、書くだけでも非常に大変ですね@<fn>{頂点作成苦労した}。

//footnote[頂点作成苦労した][実際にサンプルを作っていて（無駄に）苦労しましたがそれはまた別な話ですね。]

頂点構造体を使用する場合、glVertexAttribPointerコマンドに少しだけ工夫が必要です。[3]のglVertexAttribPointerコマンド部分に注目してください。

まず、attr_posに設定する際の第２引数が、XYZの3要素を扱うため"3"に変更されています。

それだけでなく、第５引数が"4 * 3 + 4 * 2"と記述されています。これは頂点構造体を使用する場合の「お約束」で、同じ配列から複数のデータを読み込む場合、「1頂点のバイト数」を伝えなければなりません。今回は4byte(float)×3要素(位置情報)+4byte(float)×2要素(UV情報)なので、合計20バイトが1頂点の大きさとなります。

以上の状態で描画を行うと、一見して立方体が正常に描画できているかのように見えます。しかしよく見ると、ポリゴンの前後関係が正しくありません。「後ろにある」はずのポリゴンが何故か前に見えてしまっているせいで、これが本当に立方体なのかどうかもわかりづらいでしょう。

=== 課題：深度を正しく扱う

OpenGL ES 2.0では、「奥行き」を管理するための機能がデフォルトでOFFになっています。なぜなら、使わなければそれだけ処理速度の向上に役立つからです。例えば2D描画だけしか使用しない場合は奥行きの機能は要りませんね。

次のサンプルコードで深度を正しく扱う方法を学びましょう（@<list>{Chapter03_02}）。

//listnum[Chapter03_02][Chapter03_02.java]{

public class Chapter03_02 extends Chapter03_01 {
    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        super.onSurfaceCreated(gl, config);

====================================================== [1] ここから
        // 深度バッファを有効化する
        glEnable(GL_DEPTH_TEST);
====================================================== [1] ここまで
    }

    @Override
    protected void clearSurface() {
        glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
====================================================== [2] ここから
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
====================================================== [2] ここまで
    }
}

//}

OpenGL ESの「奥行き機能」を"深度バッファ(Depth Buffer)"と呼びます。深度バッファを有効化するためには、2箇所に修正が必要ですが、難しくはありません。


[1]では、onSurfaceCreatedの初期化部分に、"glEnable"コマンドの呼び出しを追加します。このコマンドは、OpenGL ES 2.0の一部機能を「有効化」するためのものです。ここでは引数に"GL_DEPTH_TEST"を追加して、深度バッファを有効化しています。

ちなみに、逆に機能を無効化する場合は"glDisable"というコマンドが用意されています。

[2]では、glClearコマンドの引数に対して"GL_DEPTH_BUFFER_BIT"を「追加して」います。glClearコマンドの引数は1つしかありませんが、このコマンドは引数を"|"でビット演算行うことで、「フラグを複数同時に伝える」ということができます。

Androidに限らず、公開されているAPIにはこのように「ON/OFFの状態を"|"演算子で組み合わせて送る」という引数が、たびたび登場します。1bitにつき1つの「ON/OFF」を管理できますので、int型(32bit)では最大32個のON/OFF組み合わせを保持できます。頭の片隅に覚えておくとよいでしょう。

GL_DEPTH_BUFFER_BITは「深度バッファを再度使用できる状態にする」ことを示します。

この2つの操作を行うことで、図のように「正しく前後判定を行って」描画ができるのです。


=== 課題：インデックスバッファで描画する

ですが速度の向上にはまだ不十分です。なぜなら、この頂点データには大量の「不要データ」が含まれているからです。例えば、3DCGとは関係なく立方体の「頂点」を数えた場合、たった8個しかありません。

前述の課題で作成していた大量のデータの中には、重複したデータが存在してしまっています。

#@# 図を入れよう

OpenGL ESには、重複したデータを効率的に扱うための仕組み"インデックスバッファ"が存在します。これは頂点データ(頂点バッファ)の他に"インデックスバッファ"というもう1つのデータを作り出すことで、効率的なアクセスが出来るように取り計らってくれます。

「インデックスバッファが増えるのにデータが減るのか？」と疑問に思うかもしれませんが、見ればすぐに分かります。

GPUは"頂点バッファ"と"インデックスバッファ"を与えられると、まずインデックスバッファに対してアクセスを行います。インデックスバッファはその名のとおり、index=頂点の番号が記録されています。するとGPUは、インデックスバッファに記録された番号の頂点を読み込みにいきます。

重複した頂点を表現できるため、インデックスバッファの個数 > 頂点の個数でも問題ないという特徴があります。

また、インデックスが同じであれば、同じ頂点を読み込みに行きます。そのため、インデックスバッファは重複した頂点を表現する@<fn>{インデックスバッファのキャッシュ}ことを得意としているのです（@<list>{Chapter03_03}）。

//footnote[インデックスバッファのキャッシュ][一度処理した頂点はキャッシュが保持されるため、重複した頂点に何度も頂点シェーダー計算を行うことも避けられます。便利ですね]

//listnum[Chapter03_03][Chapter03_03.java]{
public class Chapter03_03 extends Chapter03_02 {

    /**
     * 毎描画時の処理
     */
    @Override
    public void onDrawFrame(GL10 gl) {
        中略...

        {
            // キューブを構築する
            final float LEFT = -1.0f;
            final float RIGHT = 1.0f;
            final float FRONT = -1.0f;
            final float BACK = 1.0f;
            final float TOP = 1.0f;
            final float BOTTOM = -1.0f;

====================================================== [1] ここから
            final float[] cubeVertices = {
                    LEFT, TOP, FRONT, 0, 1, // 左上手前
                    LEFT, TOP, BACK, 0, 0, // 左上奥
                    RIGHT, TOP, FRONT, 1, 1, // 右上手前
                    RIGHT, TOP, BACK, 1, 0, // 右上奥
                    LEFT, BOTTOM, FRONT, 1, 1, // 左下手前
                    LEFT, BOTTOM, BACK, 1, 0, // 左下奥
                    RIGHT, BOTTOM, FRONT, 0, 1, // 右下手前
                    RIGHT, BOTTOM, BACK, 0, 0, // 右下奥
            };
====================================================== [1] ここまで

====================================================== [2] ここから
            final short cubeIndices[] = {
                    //
                    0, 1, 2, //
                    2, 1, 3, //

                    2, 3, 6, //
                    6, 3, 7, //

                    6, 7, 4, //
                    4, 7, 5, //

                    4, 5, 0, //
                    0, 5, 1, //

                    1, 5, 3, //
                    3, 5, 7, //

                    0, 2, 4, //
                    4, 2, 6, //
            };
====================================================== [2] ここまで

            FloatBuffer buffer = ES20Util.wrap(cubeVertices);
            glVertexAttribPointer(attr_pos, 3, GL_FLOAT, false, 4 * 3 + 4 * 2, buffer);
            glVertexAttribPointer(attr_uv, 2, GL_FLOAT, false, 4 * 3 + 4 * 2, buffer.position(3)); // 注意:positionメソッドはbyte単位ではなく要素単位で指定する

====================================================== [3] ここから
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, ES20Util.wrap(cubeIndices));
====================================================== [3] ここまで
        }
    }
}

//}

[1]の頂点バッファを見ると、データ量が一気に減って非常にシンプルになっていることがわかります。これは、重複している頂点情報を除いたためです。前述のとおり、立方体は頂点が8つしか存在しないため、実は非常にシンプルな頂点として表現できるのです。

その代わり、「インデックス何番の頂点を示すか」を表すためのインデックスバッファはちゃんと36個存在しなければなりません。[2]では12ポリゴン（2ポリゴン×正六面体）分のインデックスバッファを用意しています。

最後に、[3]では"glDrawElements"コマンドを使用して描画を行います。描画コマンド名が違うことに注目してください。インデックスバッファを使用する場合はこのglDrawElementsコマンドで描画を行います。

第１引数は今までどおりGL_TRIANGLESです。

第２引数は使用するインデックスバッファの個数を指定します。今回はインデックスバッファのデータが36個ありますので、36を指定します。

第３引数はインデックスバッファの型を指定します。このサンプルでのインデックスバッファは2byte整数で現していますので、GL_UNSIGNED_SHORTを指定します。

第４引数はインデックスバッファを渡します。頂点バッファの時と同じように、Bufferオブジェクトにラップする必要がありますが、流れは同じです。

これでインデックスバッファによる描画が行えるようになりました。頂点データが減って、全体的にコードがスッキリしたのでは無いでしょうか。ですが、まだ足りません。まだ満足な描画速度を得るには足りないのです。

=== 課題：バッファオブジェクトで描画する

さらなる高速化・効率化のためには、この頂点をVRAMに格納することが重要です。今まで使用していたBufferオブジェクトはもともとCPUが効率よくデータを読み書きするためのクラスです。ですが、それはあくまでCPUにとって都合がよいというだけで、GPUにとって都合がいいとは限りません。

それだけでなく、CPU用のメモリ（RAM）はGPUから物理的に切り離されている場合があるため、効率のよいアクセスが行えない場合があるのです。そこで、頂点バッファやインデックスバッファを予めVRAMにアップロードし、GPUにとって都合がいいメモリ配置を行うことができます。

OpenGL ESでは、そのように「事前にVRAMへアップロードするオブジェクト」のことを"バッファオブジェクト"と呼びます（@<list>{Chapter03_04}）。このオブジェクトはテクスチャと同じく、int型のIDで管理されます。

//listnum[Chapter03_04][Chapter03_04.java]{
public class Chapter03_04 extends Chapter03_02 {

====================================================== [1] ここから
    /**
     * 頂点バッファオブジェクト
     */
    int vertexBufferObject;

    /**
     * インデックスバッファオブジェクト
     */
    int indexBufferObject;
====================================================== [1] ここまで

    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        super.onSurfaceCreated(gl, config);

====================================================== [2] ここから
        // オブジェクトを確保する
        {
            int[] objects = {0, 0};

            glGenBuffers(2, objects, 0);
            vertexBufferObject = objects[0];
            indexBufferObject = objects[1];

            assert vertexBufferObject != 0;
            assert indexBufferObject != 0;
        }
====================================================== [2] ここまで

====================================================== [3] ここから
        // 頂点バッファを生成する
        {
            // キューブを構築する
            final float LEFT = -1.0f;
            final float RIGHT = 1.0f;
            final float FRONT = -1.0f;
            final float BACK = 1.0f;
            final float TOP = 1.0f;
            final float BOTTOM = -1.0f;

            final float[] cubeVertices = {
                    LEFT, TOP, FRONT, 0, 1, // 左上手前
                    LEFT, TOP, BACK, 0, 0, // 左上奥
                    RIGHT, TOP, FRONT, 1, 1, // 右上手前
                    RIGHT, TOP, BACK, 1, 0, // 右上奥
                    LEFT, BOTTOM, FRONT, 1, 1, // 左下手前
                    LEFT, BOTTOM, BACK, 1, 0, // 左下奥
                    RIGHT, BOTTOM, FRONT, 0, 1, // 右下手前
                    RIGHT, BOTTOM, BACK, 0, 0, // 右下奥
            };

            glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
            glBufferData(GL_ARRAY_BUFFER, cubeVertices.length * 4, ES20Util.wrap(cubeVertices), GL_STATIC_DRAW);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }
====================================================== [3] ここまで

====================================================== [4] ここから
        // インデックスバッファを生成する
        {
            final short cubeIndices[] = {
                    //
                    0, 1, 2, //
                    2, 1, 3, //

                    2, 3, 6, //
                    6, 3, 7, //

                    6, 7, 4, //
                    4, 7, 5, //

                    4, 5, 0, //
                    0, 5, 1, //

                    1, 5, 3, //
                    3, 5, 7, //

                    0, 2, 4, //
                    4, 2, 6, //
            };

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, cubeIndices.length * 2, ES20Util.wrap(cubeIndices), GL_STATIC_DRAW);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        }
====================================================== [4] ここまで
    }

    @Override
    public void onDrawFrame(GL10 gl) {
        中略...

====================================================== [5] ここから
        {
            glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);

            glVertexAttribPointer(attr_pos, 3, GL_FLOAT, false, 4 * 3 + 4 * 2, 0);
            glVertexAttribPointer(attr_uv, 2, GL_FLOAT, false, 4 * 3 + 4 * 2, 4 * 3); // 注意:positionメソッドはbyte単位ではなく要素単位で指定する
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);
        }
====================================================== [5] ここまで
    }
}

//}

[1]ではバッファオブジェクトを保持するためのメンバ変数を追加しています。実際に確保を行っているのは[2]です。バッファオブジェクトの生成は"頂点バッファ"や"インデックスバッファ"の区別なく、glGenBuffersコマンドにより行えます。引数の構成はテクスチャの生成時と同じです。テクスチャの生成は１コマンドにつき１オブジェクトしか行っていませんでしたが、今回は２つ一括で確保しています。

[3]ではfloat[]型であるcubeVerticesを頂点バッファへとアップロードしています。大まかな流れはテクスチャと同じです。

 1. 頂点バッファをOpenGL ESにバインドする
 1. 必要なデータをアップロードする
 1. 頂点バッファをアンバインドする

まず、頂点バッファのバインドです。バッファオブジェクトをバインドするためには、glBindBufferコマンドを使用します。

第１引数には、バインド対象のオブジェクトの種類を指定します。頂点バッファオブジェクトであれば"GL_ARRAY_BUFFER"を指定し、インデックスバッファオブジェクトであれば"GL_ELEMENT_ARRAY_BUFFER"を指定します。OpenGL ES 2.0では、このどちらかしかありません。

第２引数にはバインドするバッファオブジェクトを指定します。

次に行うのは頂点データのアップロードです。アップロードはglBufferDataコマンドで行います。

第１引数には"GL_ARRAY_BUFFER"を指定します。

第２引数には頂点データのバイト数を指定します。今回はfloat型配列ですので、cubeVertices変数のlength×4バイトを指定すればよいということになります。

第３引数はアップロードするデータ自体を指定します。

第４引数はメモリの使われ方のヒントを与えます。与えられる種類はいくつかあります。多くの場合、この引数には"GL_STATIC_DRAW"を指定してあげれば問題ありません。その他に"GL_STREAM_DRAW"、"GL_DYNAMIC_DRAW"という種類があります。

最後に、データのアンバインドです。アンバインド処理はglBindBufferコマンドの第２引数に"0"を指定するだけで済みます。

[4]はインデックスバッファのアップロードですが、各コマンドの第1引数が"GL_ELEMENT_ARRAY_BUFFER"に変化している以外に違いはありませんので、問題なく行えるでしょう。

これでVRAMへのアップロードは完了しました。

[5]の部分で描画処理を行います。Java言語のOpenGL ESはいくつかのコマンドがオーバーロードされており、それを使用することで今までどおり「直接バッファを使用して描画」と「バッファオブジェクトを使用して描画」を切り替えられます。

描画直前に"glBindBuffer"を2回、頂点バッファオブジェクトとインデックスバッファオブジェクトでそれぞれ呼び出しています。これは「今からこのバッファオブジェクトで描画を行うよ」という宣言をGPUに行うことになります。

次に、glVertexAttribPointerコマンドの呼び出しです。このコマンドでは、最後の引数がBufferオブジェクトではなくint型に変化しています。このint型引数"offset"は、「バッファオブジェクトの何バイト目以降を使用するか」をGPUに教えるためのものです。

例えば、cubeVerticesは頂点構造体により、「位置情報」「UV情報」の2つのデータが一括でアップロードされています。この時、「位置情報はバッファの何バイト目以降に入っているか」「UV情報はバッファの何バイト目以降に入っているか」を教えなければ、正しくアクセスすることが出来ません。

そこで、最後の引数により「バッファオブジェクトの何バイト目にデータがあるか」を教えるのです。

今回は「位置情報」が0バイト目（先頭）に、「UV情報」が12byte目（位置情報の直後）に入っているため、それぞれ"0"と"12"を指定しています。

最終的な描画コマンドであるglDrawElementsコマンドも、同じく最後の引数は整数となっています。インデックスバッファは数値が押し込まれているだけの単純な情報ですので、"0"で問題ありません。

=== まとめ

かなりの駆け足になりましたが、以上でグラフィックの章（OpenGL ESの章）の解説は終了です。書籍では紙面の関係でOpenGL ESのほんの入口しか触れていません。もしもっと高度な描画やさらなる詳細な情報を求める場合、市販の書籍を読むとよいでしょう@<fn>{おすすめの本}。

//footnote[おすすめの本][「山下武志 OpenGL ES」をWebで検索！！]

===[column]
Android "L"ではOpenGL ES 3.1+Android Extension Packが発表されました。OpenGL ES 3.1ではCompute Shader等のGPGPUが登場し、描画以外の場所でもOpenGL ES（GPU）が活躍できる箇所が増えようとしています。

Android Extension PackではOpenGL ES 3.1が定める標準機能に加えて、ジオメトリシェーダー等の現代的なシェーダーにも対応しました。これはデスクトップPCの世界では一般的になりつつあり、移植性を高めることに役立つとの発表があります。

一方でApple社は2014年6月のイベントでOpenGL ESに変わるAPI"Metal"を発表しました。これはiOSデバイスに搭載されたGPUに最適化されたAPIで、OpenGL ESより少ないオーバーヘッドで実行できることが特徴としています。Appleの発表ではOpenGL ESよりもよりハードウェアに近い内容の操作が行え、かつオブジェクト指向のインターフェースを提供しています。ですがこのイベントではOpenGL ES 3.1への対応はなされませんでした。

OpenGL ES 2.0は現在「現役」といえるAndroid端末はほぼすべて対応しています。それに対し、OpenGL ES 3.0は一部のGPUを搭載したAndroid 4.3以上が必要で、さらにAndroid Extension Packを実装した端末も存在します。

Android "L"ではジオメトリシェーダー等により、より高度なグラフィックを描画することが出来ます。ですがその端末が普及するのは（少なくとも日本では）数年の時間が必要になります。多くのアプリや環境がES 2.0以下にしか対応していない端末を「切り捨て」するのはさらに未来になるでしょう。残念ながら、Android 2.3ですら、日本では捨てることを許さない人々がいるのですから。

2013年代まではOpenGL ES 2.0という共通インターフェースにより開発者は少ない労力で2つのプラットフォームに対応出来ました。しかし同時期（2014年6月）に発表された2社の方針は、2014年以降のグラフィック処理が「OpenGL ES 3.0で共通処理を行う」か「それぞれのプラットフォームに最適化するか」の判断を迫っています。

UnityやUnreal Engine等の「エンジンに特化した製品」は、間違いなく後者を選択するでしょう。しかし、「独自レンダリングエンジン・ゲームエンジンによるマルチプラットフォーム」による開発を行ってきた開発者がどの方向へ舵取りを行うか、今後の展開が非常に楽しみです。
