= OpenGL ESの3D描画

== 立方体での例

=== 課題: 立方体を描画する

//listnum[][Chapter03_01.java]{
public class Chapter03_01 extends Chapter01_01 {
    /**
     * プログラムオブジェクト
     */
    protected int program = 0;

    /**
     * 頂点座標
     */
    protected int attr_pos;

    /**
     * UV座標
     */
    protected int attr_uv;

    /**
     * 頂点に適用するワールド行列
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


    /**
     * テクスチャUniform
     */
    protected int unif_texture;

    /**
     * テクスチャオブジェクト
     */
    protected int texture;

    /**
     * カメラ位置
     * 初期位置は適当に決める
     */
    protected Vector3 cameraPos = new Vector3(10, 3, 10);

    /**
     * 回転量
     */
    protected float rotate;

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
                    "" +
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
                    "" +
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
        ES20Util.assertGL();
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
        glViewport(0, 0, width, height);
        this.screenWidth = width;
        this.screenHeight = height;
    }

    /**
     * ワールド行列設定
     */
    protected void setupWorld() {
        float[] matrix = ES20Util.createMatrixIdentity();
        Matrix.setRotateM(matrix, 0, rotate, 1.0f, 1.0f, 1.0f);
        glUniformMatrix4fv(unif_world, 1, false, matrix, 0);

        rotate += 1;
    }

    /**
     * カメラ情報のセットアップを行う
     */
    protected void setupCamera() {
        // カメラ注視
        float cameraLookX = 0;
        float cameraLookY = 0;
        float cameraLookZ = 0;

        // カメラ天地
        float cameraUpX = 0;
        float cameraUpY = 1;
        float cameraUpZ = 0;

        // 画角
        float fovY = 45.0f;
        // アスペクト比
        float aspect = screenWidth / screenHeight;

        // ニアクリップ/ファークリップ
        float nearClip = 1.0f;
        float farClip = 100.0f;

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

        {
            // projection行列を生成する
            float[] matrix = ES20Util.createMatrixIdentity();
            Matrix.perspectiveM(matrix, 0, fovY, aspect, nearClip, farClip);

            // アップロード
            glUniformMatrix4fv(unif_projection, 1, false, matrix, 0);
        }

        // デバッグ用メッセージを表示する
        SampleUtil.setDebugText(getActivity(), String.format("camera\n pos(%.2f, %.2f, %.2f)\n look(%.2f, %.2f, %.2f)\n fovy(%.1f) aspect(%.2f)",
                cameraPos.x, cameraPos.y, cameraPos.z,
                cameraLookX, cameraLookY, cameraLookZ,
                fovY, aspect));
    }

    /**
     * サーフェイスの塗りつぶし処理
     */
    protected void clearSurface() {
        glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL10.GL_COLOR_BUFFER_BIT);
    }

    /**
     * 毎描画時の処理
     */
    @Override
    public void onDrawFrame(GL10 gl) {
        // サーフェイスを単色で塗りつぶす
        clearSurface();

        // attr_posを有効にする
        glEnableVertexAttribArray(attr_pos);
        glEnableVertexAttribArray(attr_uv);

        // 各行列の設定を行う
        setupWorld();
        setupCamera();

        glBindTexture(GL_TEXTURE_2D, texture);
        glUniform1i(unif_texture, 0);

        {
            // キューブを構築する
            final float LEFT = -1.0f;
            final float RIGHT = 1.0f;
            final float FRONT = -1.0f;
            final float BACK = 1.0f;
            final float TOP = 1.0f;
            final float BOTTOM = -1.0f;

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

            FloatBuffer buffer = ES20Util.wrap(cubeVertices);
            glVertexAttribPointer(attr_pos, 3, GL_FLOAT, false, 4 * 3 + 4 * 2, buffer);
            glVertexAttribPointer(attr_uv, 2, GL_FLOAT, false, 4 * 3 + 4 * 2, buffer.position(3)); // 注意:positionメソッドはbyte単位ではなく要素単位で指定する
            glDrawArrays(GL_TRIANGLES, 0, 36);

            ES20Util.assertGL();
        }
    }
}

//}

=== 課題: 深度を正しく扱う

//listnum[][Chapter03_02.java]{

public class Chapter03_02 extends Chapter03_01 {
    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        super.onSurfaceCreated(gl, config);

        // 深度バッファを有効化する
        glEnable(GL_DEPTH_TEST);
        ES20Util.assertGL();
    }

    @Override
    protected void clearSurface() {
        glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
}

//}

=== 課題: インデックスバッファで描画する

//listnum[][Chapter03_03.java]{
public class Chapter03_03 extends Chapter03_02 {

    /**
     * 毎描画時の処理
     */
    @Override
    public void onDrawFrame(GL10 gl) {
        // サーフェイスを単色で塗りつぶす
        clearSurface();

        // attr_posを有効にする
        glEnableVertexAttribArray(attr_pos);
        glEnableVertexAttribArray(attr_uv);

        // 各行列の設定を行う
        setupWorld();
        setupCamera();

        glBindTexture(GL_TEXTURE_2D, texture);
        glUniform1i(unif_texture, 0);

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

            FloatBuffer buffer = ES20Util.wrap(cubeVertices);
            glVertexAttribPointer(attr_pos, 3, GL_FLOAT, false, 4 * 3 + 4 * 2, buffer);
            glVertexAttribPointer(attr_uv, 2, GL_FLOAT, false, 4 * 3 + 4 * 2, buffer.position(3)); // 注意:positionメソッドはbyte単位ではなく要素単位で指定する
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, ES20Util.wrap(cubeIndices));

            ES20Util.assertGL();
        }
    }
}

//}

==== TRY: ES20Util.wrapの呼び出し回数を最小限になるように修正してみよう

=== 課題: バッファオブジェクトで描画する

//listnum[][Chapter03_04.java]{
public class Chapter03_04 extends Chapter03_02 {

    /**
     * 頂点バッファオブジェクト
     */
    int vertexBufferObject;

    /**
     * インデックスバッファオブジェクト
     */
    int indexBufferObject;

    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        super.onSurfaceCreated(gl, config);

        // オブジェクトを確保する
        {
            int[] objects = {0, 0};

            glGenBuffers(2, objects, 0);
            vertexBufferObject = objects[0];
            indexBufferObject = objects[1];

            assert vertexBufferObject != 0;
            assert indexBufferObject != 0;
        }

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
    }

    /**
     * 毎描画時の処理
     */
    @Override
    public void onDrawFrame(GL10 gl) {
        // サーフェイスを単色で塗りつぶす
        clearSurface();

        // attr_posを有効にする
        glEnableVertexAttribArray(attr_pos);
        glEnableVertexAttribArray(attr_uv);

        // 各行列の設定を行う
        setupWorld();
        setupCamera();

        glBindTexture(GL_TEXTURE_2D, texture);
        glUniform1i(unif_texture, 0);

        {
            glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);

            glVertexAttribPointer(attr_pos, 3, GL_FLOAT, false, 4 * 3 + 4 * 2, 0);
            glVertexAttribPointer(attr_uv, 2, GL_FLOAT, false, 4 * 3 + 4 * 2, 4 * 3); // 注意:positionメソッドはbyte単位ではなく要素単位で指定する
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);

            ES20Util.assertGL();
        }
    }
}

//}

== GPUとOSとEGL

== AndroidにおけるOpenGL ESの今後[ver "Android L"世代]

