package com.eaglesakura.sample.graphics;

import android.opengl.Matrix;

import com.eaglesakura.sample.graphics.util.ES20Util;
import com.eaglesakura.sample.graphics.util.SampleUtil;
import com.eaglesakura.sample.graphics.util.Vector3;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import static android.opengl.GLES20.GL_FLOAT;
import static android.opengl.GLES20.GL_FRAGMENT_SHADER;
import static android.opengl.GLES20.GL_TRIANGLE_STRIP;
import static android.opengl.GLES20.GL_VERTEX_SHADER;
import static android.opengl.GLES20.glClear;
import static android.opengl.GLES20.glClearColor;
import static android.opengl.GLES20.glDrawArrays;
import static android.opengl.GLES20.glEnableVertexAttribArray;
import static android.opengl.GLES20.glGetAttribLocation;
import static android.opengl.GLES20.glGetUniformLocation;
import static android.opengl.GLES20.glUniform4f;
import static android.opengl.GLES20.glUniformMatrix4fv;
import static android.opengl.GLES20.glUseProgram;
import static android.opengl.GLES20.glVertexAttribPointer;
import static android.opengl.GLES20.glViewport;

/**
 * Chapter 02-05
 * <p/>
 * カメラで遠景から見たポリゴンを描画する
 * <p/>
 * TRY ポリゴンにテクスチャを貼り付けてみよう
 * <p/>
 * CHALLENGE シェーダーに記述する行列を１つだけにしてみよう(ヒント：Matrix.multiplyMM)
 */
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
