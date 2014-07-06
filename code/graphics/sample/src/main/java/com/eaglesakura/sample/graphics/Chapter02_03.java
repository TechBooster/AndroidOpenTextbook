package com.eaglesakura.sample.graphics;

import com.eaglesakura.sample.graphics.util.ES20Util;
import com.eaglesakura.sample.graphics.util.SampleUtil;

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
 * Chapter 02-03
 * <p/>
 * 四角形を行列で拡大縮小させる
 */
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
