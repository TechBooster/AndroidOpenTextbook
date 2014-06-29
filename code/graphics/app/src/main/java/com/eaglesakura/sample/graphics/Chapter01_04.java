package com.eaglesakura.sample.graphics;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.opengl.GLES20;
import android.opengl.GLUtils;

import static android.opengl.GLES20.*;

import com.eaglesakura.sample.graphics.util.ES20Util;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

/**
 * テクスチャサンプル
 * <p/>
 * TRY テクスチャの「ネガ」を描画してみよう
 * <p/>
 * TRY UV座標を操作してテクスチャの一部を描画してみよう
 */
public class Chapter01_04 extends Chapter01_01 {
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
     * テクスチャUniform
     */
    protected int unif_texture;

    /**
     * テクスチャオブジェクト
     */
    protected int texture;

    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        {
            final String vertexShaderSource =
                    "" +
                            "attribute mediump vec4 attr_pos;" +
                            "attribute mediump vec2 attr_uv;" +
                            "varying mediump vec2 vary_uv;" +
                            "void main() {" +
                            "   gl_Position = attr_pos;" +
                            "   vary_uv = attr_uv;" +
                            "}";

            final String fragmentShaderSource =
                    "" +
                            "uniform sampler2D unif_texture;" +
                            "varying mediump vec2 vary_uv;" +
                            "void main() {" +
                            "   gl_FragColor = texture2D(unif_texture, vary_uv);" +
                            "}";

            program = ES20Util.compileAndLinkShader(vertexShaderSource, fragmentShaderSource);
        }

        // locationを取り出す
        {
            attr_pos = glGetAttribLocation(program, "attr_pos");
            assert attr_pos >= 0;
            attr_uv = glGetAttribLocation(program, "attr_uv");
            assert attr_uv >= 0;
            unif_texture = glGetUniformLocation(program, "unif_texture");
            assert unif_texture >= 0;
        }

        // テクスチャを読み込む
        {
            Bitmap bitmap = ES20Util.decodeBitmapFromAssets(getActivity(), "sample512x512.png");

            int[] textureId = {0};
            glGenTextures(1, textureId, 0);
            this.texture = textureId[0];
            assert this.texture != 0;

            glBindTexture(GL_TEXTURE_2D, texture);
            GLUtils.texImage2D(GL_TEXTURE_2D, 0, bitmap, 0);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

            ES20Util.assertGL();
        }

        glUseProgram(program);
        ES20Util.assertGL();
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
        glViewport(0, 0, width, height);
    }

    @Override
    public void onDrawFrame(GL10 gl) {
        glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // attr_posを有効にする
        glEnableVertexAttribArray(attr_pos);
        glEnableVertexAttribArray(attr_uv);

        // unif_textureへテクスチャを設定する
        glUniform1i(unif_texture, 0);

        {
            final float[] position = {
                    // v0(left top)
                    -0.75f, 0.75f,
                    // v1(left bottom)
                    -0.75f, -0.75f,
                    // v2(right top)
                    0.75f, 0.75f,
                    // v3(right bottom)
                    0.75f, -0.75f,};

            final float[] uv = {
                    // v0(left top)
                    0, 0,
                    // v1(left bottom)
                    0, 1,
                    // v2(right top)
                    1, 0,
                    // v3(right bottom)
                    1, 1,};

            glVertexAttribPointer(attr_pos, 2, GL_FLOAT, false, 0, ES20Util.wrap(position));
            glVertexAttribPointer(attr_uv, 2, GL_FLOAT, false, 0, ES20Util.wrap(uv));
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        }
    }
}
