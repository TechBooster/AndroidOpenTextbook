package com.eaglesakura.sample.graphics.util;

// static importでGLES20のメソッドへアクセスさせる

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.opengl.GLUtils;

import java.io.InputStream;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;

import static android.opengl.GLES20.GL_CLAMP_TO_EDGE;
import static android.opengl.GLES20.GL_COMPILE_STATUS;
import static android.opengl.GLES20.GL_FALSE;
import static android.opengl.GLES20.GL_FRAGMENT_SHADER;
import static android.opengl.GLES20.GL_LINK_STATUS;
import static android.opengl.GLES20.GL_NEAREST;
import static android.opengl.GLES20.GL_NO_ERROR;
import static android.opengl.GLES20.GL_TEXTURE_2D;
import static android.opengl.GLES20.GL_TEXTURE_MAG_FILTER;
import static android.opengl.GLES20.GL_TEXTURE_MIN_FILTER;
import static android.opengl.GLES20.GL_TEXTURE_WRAP_S;
import static android.opengl.GLES20.GL_TEXTURE_WRAP_T;
import static android.opengl.GLES20.GL_VERTEX_SHADER;
import static android.opengl.GLES20.glAttachShader;
import static android.opengl.GLES20.glBindTexture;
import static android.opengl.GLES20.glCompileShader;
import static android.opengl.GLES20.glCreateProgram;
import static android.opengl.GLES20.glCreateShader;
import static android.opengl.GLES20.glDeleteShader;
import static android.opengl.GLES20.glGenTextures;
import static android.opengl.GLES20.glGetError;
import static android.opengl.GLES20.glGetProgramInfoLog;
import static android.opengl.GLES20.glGetProgramiv;
import static android.opengl.GLES20.glGetShaderInfoLog;
import static android.opengl.GLES20.glGetShaderiv;
import static android.opengl.GLES20.glLinkProgram;
import static android.opengl.GLES20.glShaderSource;
import static android.opengl.GLES20.glTexParameteri;


/**
 * ES2.0以上で使用できるUtil系メソッドの集合
 */
public class ES20Util {

    public static void assertGL() {
        assert glGetError() == GL_NO_ERROR;
    }

    /**
     * 頂点シェーダー/フラグメントシェーダーのコンパイルを行う
     */
    public static int compileShader(int GL_XXXX_SHADER, String source) {
        final int shader = glCreateShader(GL_XXXX_SHADER);

        glShaderSource(shader, source);
        glCompileShader(shader);

        // コンパイルエラーをチェックする
        {
            int[] compileSuccess = new int[]{0};
            glGetShaderiv(shader, GL_COMPILE_STATUS, compileSuccess, 0);
            if (compileSuccess[0] == GL_FALSE) {
                throw new RuntimeException(glGetShaderInfoLog(shader));
            }
        }

        return shader;
    }

    /**
     * 頂点シェーダとフラグメントシェーダをリンクさせる
     */
    public static int linkShader(int vertexShader, int fragmentShader) {
        final int program = glCreateProgram();
        glAttachShader(program, vertexShader);
        glAttachShader(program, fragmentShader);

        assertGL();

        glLinkProgram(program);


        // リンクエラーをチェックする
        {
            int[] linkSuccess = new int[]{0};
            glGetProgramiv(program, GL_LINK_STATUS, linkSuccess, 0);
            if (linkSuccess[0] == GL_FALSE) {
                throw new RuntimeException(glGetProgramInfoLog(program));
            }
        }

        // delete
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        assertGL();

        return program;
    }

    /**
     * コンパイルとリンクを行う
     *
     * @param vertexShaderSource
     * @param fragmentShaderSource
     * @return
     */
    public static int compileAndLinkShader(String vertexShaderSource, String fragmentShaderSource) {
        int vShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
        int fShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

        return linkShader(vShader, fShader);
    }

    /**
     * 配列バッファをOpenGL ESで使用できるBufferオブジェクトにラップする
     */
    public static FloatBuffer wrap(float[] buffer) {
        return (FloatBuffer) ByteBuffer.allocateDirect(buffer.length * 4).order(ByteOrder.nativeOrder()).asFloatBuffer().put(buffer).position(0);
    }

    /**
     * assetsディレクトリからBitmapを読み込む
     */
    public static Bitmap decodeBitmapFromAssets(Context context, String fileName) {
        InputStream is = null;
        try {
            is = context.getAssets().open(fileName);

            return BitmapFactory.decodeStream(is);
        } catch (Exception e) {
            return null;
        } finally {
            if (is != null) {
                try {
                    is.close();
                } catch (Exception ee) {

                }
            }
        }
    }

    /**
     * assetsディレクトリからテクスチャを読み込む
     */
    public static int loadTextureFromAssets(Context context, String fileName) {
        Bitmap bitmap = decodeBitmapFromAssets(context, "sample512x512.png");

        int[] textureId = {0};
        glGenTextures(1, textureId, 0);
        int texture = textureId[0];
        assert texture != 0;

        glBindTexture(GL_TEXTURE_2D, texture);
        GLUtils.texImage2D(GL_TEXTURE_2D, 0, bitmap, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        ES20Util.assertGL();

        return texture;
    }

    /**
     * 単位行列を生成する
     *
     * @return
     */
    public static float[] createMatrixIdentity() {
        float[] result = new float[]{
                1, 0, 0, 0,
                0, 1, 0, 0,
                0, 0, 1, 0,
                0, 0, 0, 1,
        };
        return result;
    }
}
