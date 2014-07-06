package com.eaglesakura.sample.graphics;

import com.eaglesakura.sample.graphics.util.ES20Util;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import static android.opengl.GLES20.*;

/**
 * Chapter 03-04
 * <p/>
 * バッファオブジェクトを使用して描画する
 */
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
