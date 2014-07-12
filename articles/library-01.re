= 外部ライブラリ 1日目

本章では、アプリケーション開発における外部ライブラリの役割とその使い方、作り方、配布方法を学びます。

#@# ゴールは外部ライブラリとは何かを理解し、適切に利用したり、自分で作成出来るようになる

==  ライブラリとは何か



== なぜライブラリが必要なのか

=== 巨人の肩に乗る

== Androidおけるライブラリ
 
 ** native
 *** .so
 ** java
 *** jar
 *** ライブラリプロジェクト
 *** aar
 * Androidフレームワークも一種のライブラリだ
 * バージョン問題とsupportライブラリ
 * 今までも色んなライブラリの恩恵を受けている
 ** 例えばOpenGLとか,Javaの標準クラスライブラリとか
 * ライブラリを使ってコードを書かずに処理をさせてみよう
 ** 通信系？
 ** 画像処理系?
 ** UI系
 * 様々なライブラリ


演習メモ

=== 演習1 

 ネットワーク章で作ったHttpURLConnectionの処理を@<href>{https://github.com/loopj/android-async-http,android-async-http}に置き換えてみよう

=== 演習2

 @<href>{https://github.com/Bearded-Hen/Android-Bootstrap,Android-Bootstrap}でUIを格好良くしてみよう