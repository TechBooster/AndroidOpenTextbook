package com.eaglesakura.sample.graphics;

import com.eaglesakura.sample.graphics.util.ES20Util;
import com.eaglesakura.sample.graphics.util.SampleUtil;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import static android.opengl.GLES20.*;

/**
 * Chapter 02-01
 * <p/>
 * 四角形を平行移動させる
 */
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

        // デバッグ用メッセージを表示する
        SampleUtil.setDebugText(getActivity(), String.format("translate(%.2f, %.2f)", translateX, translateY));
    }
}
