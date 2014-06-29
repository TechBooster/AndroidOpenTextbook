package com.eaglesakura.sample.graphics;

import android.app.Fragment;
import android.opengl.GLES20;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

/**
 * Chapter 01-01
 * <p/>
 * 画面をクリアする
 * <p/>
 * TRY:画面を好きな色で描画する
 * <p/>
 * TRY:画面をランダムな色で描画する
 */
public class Chapter01_01 extends Fragment implements GLSurfaceView.Renderer {

    protected GLSurfaceView glSurfaceView;

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        glSurfaceView = new GLSurfaceView(getActivity());
        glSurfaceView.setEGLContextClientVersion(2);
        glSurfaceView.setRenderer(this);

        // SurfaceViewを使う場合のおまじない
        // ※ これはWindowSystemの都合で、詳細は割愛
        glSurfaceView.setZOrderOnTop(false);

        return glSurfaceView;
    }

    @Override
    public void onPause() {
        super.onPause();
        glSurfaceView.onPause();

        Log.i(getClass().getSimpleName(), "onPause");
    }

    @Override
    public void onResume() {
        super.onResume();
        glSurfaceView.onResume();

        Log.i(getClass().getSimpleName(), "onResume");
    }

    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {

    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {

    }

    @Override
    public void onDrawFrame(GL10 gl) {
        GLES20.glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
//            GLES20.glClearColor((float) Math.random(), (float) Math.random(), (float) Math.random(), 1.0f); // ランダムな色でクリアする
        GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT);
    }
}
