package com.eaglesakura.sample.graphics;

import com.eaglesakura.sample.graphics.util.ES20Util;

import java.nio.FloatBuffer;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import static android.opengl.GLES20.*;

/**
 * Chapter 03-03
 * <p/>
 * インデックスバッファの描画を行う
 * <p/>
 * TRY ES20Util.wrapの呼び出し回数を最小限になるように修正してみよう
 */
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
