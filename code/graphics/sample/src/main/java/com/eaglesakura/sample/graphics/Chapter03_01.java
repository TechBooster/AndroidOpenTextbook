package com.eaglesakura.sample.graphics;

import android.opengl.Matrix;

import com.eaglesakura.sample.graphics.util.ES20Util;
import com.eaglesakura.sample.graphics.util.SampleUtil;
import com.eaglesakura.sample.graphics.util.Vector3;

import java.nio.ByteBuffer;
import java.nio.FloatBuffer;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import static android.opengl.GLES20.*;

/**
 * Chapter 03-01
 * <p/>
 * 立方体を描画する
 */
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
