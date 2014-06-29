package com.eaglesakura.sample.graphics.util;

// static importでGLES20のメソッドへアクセスさせる

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;

import static android.opengl.GLES20.*;

import java.io.InputStream;
import java.nio.Buffer;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;


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
    public static Buffer wrap(float[] buffer) {
        return ByteBuffer.allocateDirect(buffer.length * 4).order(ByteOrder.nativeOrder()).asFloatBuffer().put(buffer).position(0);
    }

    /**
     * assetsディレクトリからBitmapを読み込む
     *
     * @param context
     * @param fileName
     * @return
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
}
