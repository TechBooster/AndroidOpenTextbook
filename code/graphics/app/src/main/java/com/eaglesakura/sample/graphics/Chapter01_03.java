package com.eaglesakura.sample.graphics;

import android.opengl.GLES20;

import com.eaglesakura.sample.graphics.util.ES20Util;

import javax.microedition.khronos.opengles.GL10;

/**
 * Chapter 01-03
 * <p/>
 * 四角形を描画する
 */
public class Chapter01_03 extends Chapter01_02 {
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


        {
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
        }


        {
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
        }


        {
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
        }
    }
}
