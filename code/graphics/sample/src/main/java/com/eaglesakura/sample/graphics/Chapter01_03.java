package com.eaglesakura.sample.graphics;

import android.opengl.GLES20;

import static android.opengl.GLES20.*;
import com.eaglesakura.sample.graphics.util.ES20Util;

import javax.microedition.khronos.opengles.GL10;

/**
 * Chapter 01-03
 * <p/>
 * 四角形を描画する
 * <p/>
 * TRY: 右側の三角形、左側の三角形だけをそれぞれ描画してみよう
 * <p/>
 * TRY: 五角形や六角形を描画してみよう
 */
public class Chapter01_03 extends Chapter01_02 {
    @Override
    public void onDrawFrame(GL10 gl) {
        glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL10.GL_COLOR_BUFFER_BIT);

        // attr_posを有効にする
        glEnableVertexAttribArray(attr_pos);

        // ポリゴン色をアップロードする
        // 色はRGBAでアップロードする
        glUniform4f(unif_color, 1.0f, 0.0f, 0.0f, 1.0f);


        {
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
            glDrawArrays(GLES20.GL_TRIANGLES, 0, 6);
        }
    }
}
