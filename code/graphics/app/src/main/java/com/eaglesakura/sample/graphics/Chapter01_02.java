package com.eaglesakura.sample.graphics;

import android.app.Fragment;
import android.opengl.GLES20;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import com.eaglesakura.sample.graphics.util.ES20Util;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

/**
 * Chapter 01-02
 * <p/>
 * 画面に三角形を描画する
 * <p/>
 * TRY 端末の縦横を切り替えてみる
 * <p/>
 * TRY 三角形の色を変えてみる
 * <p/>
 * TRY 三角形の大きさや位置を変更する
 * <p/>
 * TRY Viewportを変更する
 * <p/>
 * CHALLENGE "glUniform4f"やassertの部分を変更せずに、シェーダーだけで任意の色のポリゴンを表示させてみよう
 */
public class Chapter01_02 extends Chapter01_01 {
    /**
     * プログラムオブジェクト
     */
    int program = 0;

    /**
     * attr_pos
     */
    int attr_pos;

    /**
     * ポリゴン色
     */
    int unif_color;

    /**
     * Surfaceが生成されたタイミングの処理
     */
    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        {
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

            final int vertexShader = ES20Util.compileShader(GLES20.GL_VERTEX_SHADER, vertexShaderSource);
            final int fragmentShader = ES20Util.compileShader(GLES20.GL_FRAGMENT_SHADER, fragmentShaderSource);


            // linkShaderの中でvertex/fragmentの各シェーダーがDeleteされていることに注意する
            this.program = ES20Util.linkShader(vertexShader, fragmentShader);
        }

        // attributeを取得する
        {
            attr_pos = GLES20.glGetAttribLocation(program, "attr_pos");
            assert attr_pos >= 0;

            unif_color = GLES20.glGetUniformLocation(program, "unif_color");
            assert unif_color >= 0;
        }

        GLES20.glUseProgram(program);
        ES20Util.assertGL();
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
        GLES20.glViewport(0, 0, width, height);
    }

    /**
     * 毎描画時の処理
     */
    @Override
    public void onDrawFrame(GL10 gl) {
        GLES20.glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
        GLES20.glClear(GL10.GL_COLOR_BUFFER_BIT);


        // TODO 三角形描画
        // attr_posを有効にする
        GLES20.glEnableVertexAttribArray(attr_pos);

        // ポリゴン色をアップロードする
        // 色はRGBAでアップロードする
        GLES20.glUniform4f(unif_color, 1.0f, 0.0f, 0.0f, 1.0f);


        // 画面中央へ描画する
        final float[] position = {
                // v0
                0.0f, 1.0f,
                // v1
                1.0f, -1.0f,
                // v2
                -1.0f, -1.0f};


        GLES20.glVertexAttribPointer(attr_pos, 2, GLES20.GL_FLOAT, false, 0, ES20Util.wrap(position));
        GLES20.glDrawArrays(GLES20.GL_TRIANGLES, 0, 3);

    }
}
