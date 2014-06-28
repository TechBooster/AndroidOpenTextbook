package com.eaglesakura.sample.graphics;

import android.app.Fragment;
import android.opengl.GLES20;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

/**
 * Chapter 01
 */
public class Chapter01 extends Fragment {

    GLSurfaceView glSurfaceView;

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        glSurfaceView = new GLSurfaceView(getActivity());
        glSurfaceView.setEGLContextClientVersion(4);
        glSurfaceView.setRenderer(renderer);

        return glSurfaceView;
    }

    @Override
    public void onPause() {
        super.onPause();
        glSurfaceView.onPause();
    }

    @Override
    public void onResume() {
        super.onResume();
        glSurfaceView.onResume();
    }

    GLSurfaceView.Renderer renderer = new GLSurfaceView.Renderer() {
        @Override
        public void onSurfaceCreated(GL10 gl, EGLConfig config) {

        }

        @Override
        public void onSurfaceChanged(GL10 gl, int width, int height) {

        }

        @Override
        public void onDrawFrame(GL10 gl) {
            GLES20.glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
            GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT);
        }
    };
}
