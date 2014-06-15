= センサー概要
//lead{
 本章ではAndroidに搭載された各種センサーの使用方法および、
 Google Play Serviceの位置情報を利用する方法について学んでいきます。
//}

== センサーの種類
AndroidではAPIレベルが上がるたびに使用可能なセンサーの種類が増えています。
現在の対応はリファレンスを参照してください@<fn>{sensor_ref}。
Androidでは多くのセンサー情報が取得できますが、実際のハードウェアがその数の
分だけ搭載されているわけではありません。ハードウェア１つで複数のセンサー
情報に対して提供することが多いです。

//footnote[sensor_ref][http://developer.android.com/reference/android/hardware/Sensor.html]

=== 定義されているセンサー一覧
 * accelerometer：加速度：加速度を検出するセンサ
 * gravity：重力：重力を検出するセンサ
 * gyro：ジャイロ：角速度を検出するセンサ
 * luminance：照度：周囲の照度を検出するセンサ
 * 地磁気：地場を検知できるセンサ。コンパス
 * 回転ベクトル：おまいらの要求をまとめてみた
 * アンキャリブレート回転ベクトル：
 * 地磁気回転ベクトル：
 * pressure：気圧
 * proximity：近接
 * Temperatuew：温度
 * Step counter：歩数
 * Step detector：歩行検出
 * Significant motion：？？
 * GPS

Base sensor
Composite sensor
Batching sensor
Motion Sensors
Position Sensor
Environment Sensors

https://source.android.com/devices/sensors/composite_sensors.html
http://dev.classmethod.jp/smartphone/android/android-google-play-services-location-api-current-location/

GPSはGoogle Play Serviceを使用する体で

== 搭載されたセンサーの取得
 * 実機のセンサーの取得をするサンプルアプリ

=== センサー値を取得してみよう
 * 実測アプリ

== GPSを使う
 * Google Play Services

#@# SIMはない予定・WiMAXはある
#@# Kitkat 4.4
#@# Windows8.1
#@# ですます調
#@# 1人称は使わない方向：筆者