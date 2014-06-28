package com.eaglesakura.sample.graphics.util;

import android.opengl.GLES20;

/**
 * ES2.0以上で使用できるUtil系メソッドの集合
 */
public class ES20Util {

    public static void assertGL() {
        assert GLES20.glGetError() == GLES20.GL_NO_ERROR;
    }

    /**
     * 頂点シェーダー/フラグメントシェーダーのコンパイルを行う
     */
    public static int compileShader(int GL_XXXX_SHADER, String source) {
        final int shader = GLES20.glCreateShader(GL_XXXX_SHADER);

        GLES20.glShaderSource(shader, source);
        GLES20.glCompileShader(shader);

        // コンパイルエラーをチェックする
        {
            int[] compileSuccess = new int[]{0};
            GLES20.glGetShaderiv(shader, GLES20.GL_COMPILE_STATUS, compileSuccess, 0);
            if (compileSuccess[0] == GLES20.GL_FALSE) {
                throw new RuntimeException(GLES20.glGetShaderInfoLog(shader));
            }
        }

        return shader;
    }

    /**
     * 頂点シェーダとフラグメントシェーダをリンクさせる
     */
    public static int linkShader(int vertexShader, int fragmentShader) {
        final int program = GLES20.glCreateProgram();
        GLES20.glAttachShader(program, vertexShader);
        GLES20.glAttachShader(program, fragmentShader);

        assertGL();

        GLES20.glLinkProgram(program);


        // リンクエラーをチェックする
        {
            int[] linkSuccess = new int[]{0};
            GLES20.glGetProgramiv(program, GLES20.GL_LINK_STATUS, linkSuccess, 0);
            if (linkSuccess[0] == GLES20.GL_FALSE) {
                throw new RuntimeException(GLES20.glGetProgramInfoLog(program));
            }
        }

        // delete
        GLES20.glDeleteShader(vertexShader);
        GLES20.glDeleteShader(fragmentShader);
        assertGL();

        return program;
    }
}
