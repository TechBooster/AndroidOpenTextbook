package com.example.usemylibrary;

import com.example.mylibrary.Calculator;

import android.app.Activity;
import android.os.Bundle;
import android.widget.TextView;


public class MainActivity extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        
        Calculator calculator = new Calculator();
        int result = calculator.plus(10, 15);
        
        TextView textView = (TextView)findViewById(R.id.text);
        textView.setText("result=" + result);
    }
}
