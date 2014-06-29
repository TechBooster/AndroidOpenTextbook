= 3D描画への基礎知識

== シェーダーでのシンプルな計算

=== 演習: ポリゴンを平行移動させる

//listnum[][Chapter02_01.java]{
public class Chapter02_01 extends Chapter01_01 {
    /**
     * プログラムオブジェクト
     */
    protected int program = 0;

    /**
     * attr_pos
     */
    protected int attr_pos;

    /**
     * 平行移動
     * unif_matrix
     */
    protected int unif_translate;

    /**
     * ポリゴン色
     */
    protected int unif_color;

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
            final String vertexShaderSource =
                    "" +
                            "uniform mediump vec4 unif_matrix;" +
                            "attribute mediump vec4 attr_pos;" +
                            "void main() {" +
                            "   gl_Position = attr_pos + unif_matrix;" +
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

            unif_translate = glGetUniformLocation(program, "unif_matrix");
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


        final float[] position = {
                // v0(left top)
                -0.75f, 0.75f,
                // v1(left bottom)
                -0.75f, -0.75f,
                // v2(right top)
                0.75f, 0.75f,
                // v3(right bottom)
                0.75f, -0.75f,};

        glVertexAttribPointer(attr_pos, 2, GL_FLOAT, false, 0, ES20Util.wrap(position));
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        // デバッグ用メッセージを表示する
        SampleUtil.setDebugText(getActivity(), String.format("translate(%.2f, %.2f)", translateX, translateY));
    }
}
	
//}

== 行列演算を行う

=== 演習:行列で平行移動を行う

//listnum[][Chapter02_02.java]{
public class Chapter02_02 extends Chapter01_01 {
    /**
     * プログラムオブジェクト
     */
    protected int program = 0;

    /**
     * attr_pos
     */
    protected int attr_pos;

    /**
     * 頂点に適用する行列
     * unif_matrix
     */
    protected int unif_matrix;

    /**
     * ポリゴン色
     */
    protected int unif_color;

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
            final String vertexShaderSource =
                            "uniform mediump mat4 unif_matrix;" +
                            "attribute mediump vec4 attr_pos;" +
                            "void main() {" +
                            "   gl_Position = unif_matrix * attr_pos;" +
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

        // 平行移動を行う
        {
            float[] matrix = ES20Util.createMatrixIdentity();
            matrix[4 * 3 + 0] = translateX;
            matrix[4 * 3 + 1] = translateY;
            glUniformMatrix4fv(unif_matrix, 1, false, matrix, 0);

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


        final float[] position = {
                // v0(left top)
                -0.75f, 0.75f,
                // v1(left bottom)
                -0.75f, -0.75f,
                // v2(right top)
                0.75f, 0.75f,
                // v3(right bottom)
                0.75f, -0.75f,};

        glVertexAttribPointer(attr_pos, 2, GL_FLOAT, false, 0, ES20Util.wrap(position));
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        // デバッグ用メッセージを表示する
        SampleUtil.setDebugText(getActivity(), String.format("translate(%.2f, %.2f)", translateX, translateY));
    }
}

//}

=== 演習: ポリゴンを行列で拡大縮小させる

//listnum[][Chapter02_03.java]{
public class Chapter02_03 extends Chapter01_01 {
    /**
     * プログラムオブジェクト
     */
    protected int program = 0;

    /**
     * attr_pos
     */
    protected int attr_pos;

    /**
     * 頂点に適用する行列
     * unif_matrix
     */
    protected int unif_matrix;

    /**
     * ポリゴン色
     */
    protected int unif_color;

    /**
     * X方向の拡大縮小
     */
    protected float scaleX = 1;

    /**
     * X方向の拡大縮小
     */
    protected float scaleY = 1;

    /**
     * Surfaceが生成されたタイミングの処理
     */
    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        {
            final String vertexShaderSource =
                    "" +
                            "uniform mediump mat4 unif_matrix;" +
                            "attribute mediump vec4 attr_pos;" +
                            "void main() {" +
                            "   gl_Position = unif_matrix * attr_pos;" +
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

        // 平行移動を行う
        {
            float[] matrix = ES20Util.createMatrixIdentity();
            matrix[4 * 0 + 0] = scaleX;
            matrix[4 * 1 + 1] = scaleY;
            glUniformMatrix4fv(unif_matrix, 1, false, matrix, 0);

            scaleX -= 0.01f;
            scaleY -= 0.005f;

            // 適当なところで元に戻す
            if (scaleX < 0) {
                scaleX = 2;
            }
            if (scaleY < 0) {
                scaleY = 2;
            }
        }


        final float[] position = {
                // v0(left top)
                -0.75f, 0.75f,
                // v1(left bottom)
                -0.75f, -0.75f,
                // v2(right top)
                0.75f, 0.75f,
                // v3(right bottom)
                0.75f, -0.75f,};

        glVertexAttribPointer(attr_pos, 2, GL_FLOAT, false, 0, ES20Util.wrap(position));
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        // デバッグ用メッセージを表示する
        SampleUtil.setDebugText(getActivity(), String.format("scale(%.2f, %.2f)", scaleX, scaleY));
    }
}	
//}

=== 演習: ポリゴンを行列で回転させる

//listnum[][Chapter02_04.java]{

public class Chapter02_04 extends Chapter01_01 {
    /**
     * プログラムオブジェクト
     */
    protected int program = 0;

    /**
     * attr_pos
     */
    protected int attr_pos;

    /**
     * 頂点に適用する行列
     * unif_matrix
     */
    protected int unif_matrix;

    /**
     * ポリゴン色
     */
    protected int unif_color;

    /**
     * 回転量
     */
    protected float rotate;

    /**
     * Surfaceが生成されたタイミングの処理
     */
    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        {
            final String vertexShaderSource =
                    "" +
                            "uniform mediump mat4 unif_matrix;" +
                            "attribute mediump vec4 attr_pos;" +
                            "void main() {" +
                            "   gl_Position = unif_matrix * attr_pos;" +
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

        // 平行移動を行う
        {
            float[] matrix = ES20Util.createMatrixIdentity();
            Matrix.setRotateM(matrix, 0, rotate, 0, 0, 1.0f);
            glUniformMatrix4fv(unif_matrix, 1, false, matrix, 0);

            rotate += 1;
        }


        final float[] position = {
                // v0(left top)
                -0.75f, 0.75f,
                // v1(left bottom)
                -0.75f, -0.75f,
                // v2(right top)
                0.75f, 0.75f,
                // v3(right bottom)
                0.75f, -0.75f,};

        glVertexAttribPointer(attr_pos, 2, GL_FLOAT, false, 0, ES20Util.wrap(position));
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        // デバッグ用メッセージを表示する
        SampleUtil.setDebugText(getActivity(), String.format("rotate(%.2f)", rotate));
    }
}

//}

==== TRY: X軸・Y軸の回転を試してみよう

== OpenGL ESにとっての「カメラ」

=== 演習: カメラを行列で指定する

//listnum[][Chapter02_05]{
public class Chapter02_05 extends Chapter01_01 {
    /**
     * プログラムオブジェクト
     */
    protected int program = 0;

    /**
     * attr_pos
     */
    protected int attr_pos;

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

    /**
     * ポリゴン色
     */
    protected int unif_color;

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
                            "void main() {" +
                            "   gl_Position = unif_projection * unif_look * unif_world * attr_pos;" +
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

            unif_world = glGetUniformLocation(program, "unif_world");
            assert unif_world >= 0;

            unif_look = glGetUniformLocation(program, "unif_look");
            assert unif_look >= 0;

            unif_projection = glGetUniformLocation(program, "unif_projection");
            assert unif_projection >= 0;
        }

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

        // 各行列の設定を行う
        setupWorld();
        setupCamera();

        final float[] position = {
                // v0(left top)
                -0.75f, 0.75f,
                // v1(left bottom)
                -0.75f, -0.75f,
                // v2(right top)
                0.75f, 0.75f,
                // v3(right bottom)
                0.75f, -0.75f,};

        glVertexAttribPointer(attr_pos, 2, GL_FLOAT, false, 0, ES20Util.wrap(position));
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    }
}
//}

=== TRY: ポリゴンにテクスチャを貼り付けてみよう

=== CHALLENGE: シェーダーに記述する行列を１つだけにしてみよう
