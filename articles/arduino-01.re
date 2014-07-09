= センサー番外編

//lead{
本章ではセンサーの番外編として、実際のセンサーをArduinoという
プロトタイピングツールを使って体験し、ハードウェアについても理解を
深めてみようという番外編です。
}

== Arduinoとは

Arduino@<fn>{arduino}は今までハードルの高かったハードウェアの開発を、
非常に簡単に行えるようにしたツールです。ちょっとしたセンサーなどの
ハードウェアを制御するのに使われ、プロトタイピングツールと呼ばれています。
開発用のIDE環境が用意されており、C言語ライクな記述でプログラムを書くことが
できます。

//foootnote[ardiuino][@<href>{http://www.arduino.cc/}]

=== Arduino Uno

これは一番スタンダードなArduino Unoです。
//image[arduino-01-uno][Arduino uno]
}

Unoの仕様は

 * AVRマイコン 16MHz
 * 5v電源
 * USBでの書き込み・デバッグ
 * デジタル I/O 14本
 * アナログ Input 6本

Unoのピン配置を書き出してみます。
//image[arduino-01-pin][ピン配置]
}

"~"の付いているピンはPWM(Pulse Wave Modulation)と言い、実際には
アナログ出力っぽいことが可能です。Arduinoはこの各ピンにセンサーなど
を接続し、マイコンのプログラムを動作させることでセンサーの情報を取得
したり、外部のLEDを点灯させたり、アクチェータを動かしたりできます。

== Arduino IDEのインストール

Arduino IDEは、Windows, MacOS, Linuxに対応しています。IDEの
ダウンロードはこちら@<href>{http://arduino.cc/en/Main/Software}から行います。

== デバイスドライバのインストール

MacOSとLinuxではドライバは必要ありませんが、WindowsではUSBシリアル
ドライバが必要です。Windows用ドライバはこちら@<href>{http://www.ftdichip.com/Drivers/VCP.htm}
からダウンロードしてインストールします。ドライバをインストールしたら、
ArduinoとPCをUSBケーブルで接続して下さい。コントロールパネルが以下の
ようになれば、Arduinoがシリアルポートとして正常に認識しています。この場合は
"COM11"としてArduinoが見えるようになっています。

//image[arduino-01-driver][ドライバーのインストール]{
}

== Arduinoで"Hello World"

それではArduinoで"Hello World"を実行してみましょう。とは言ってもArduinoは
表示機能を標準では持っていませんので、どこにも表示できません。

Arduinoのようなハードウェアで言う"Hello World"は"Lチカ"と言う動作が
それに当たります。"Lチカ"とは"LEDチカチカ"の略です。

Ardino IDEを起動してみましょう。起動したら、下記のように"スケッチの例>01.Basics>Blink"
を選択すると、サンプルのコードが開きます。

//image[arduino-01-blink][Blinkを開く]

開いたコードに日本語で注釈を付けておきます。

//list[blink][Blink.ino]{
// 13番ピンはほとんどのArduinoボードでLEDに接続されています
int led = 13;

// setupルーチンはリセット後に１度だけ実行されます
void setup() {                
  // LEDピンを初期化し、出力とします
  pinMode(led, OUTPUT);     
}

// loopルーチンは、基本的に終了せず、永久にループします
void loop() {
  digitalWrite(led, HIGH);   // LEDを点灯します（HIGH は5v出力）
  delay(1000);               // 1000msのウェイト
  digitalWrite(led, LOW);    // LEDを消灯します（LOWは 0v出力）
  delay(1000);               // 1000msのウェイト
}
}

プログラムのポイントは

 * setupルーチンが実行される（一度だけ。ここで各ピンなどの初期化を行う）
 * loopルーチンはsetup終了後に実行され、永久ループとなる

プログラムの内容としては

 * setupで13番のピンをOUTPUT（出力方向）に設定
 * looop内で、digitalWriteで13番に割り当てたledにHIGHとLOWを1秒置きに出力する

それではこのコードを実行する前に、デバイスとの接続設定をしておきます。

//image[arduino-01-select-uno][Arduino Unoの選択]{
}

"ツール>マイコンボード>Arduino Uno"を選択します。

//image[arduino-01-serial][シリアルポートの選択]{
}

"ツール>シリアルポート>XXXX"を選択します。Windowsの場合は"COMxx"になります。

デバイスの設定ができたら、このコードをArduinoに書き込んで実行します。
Arduinoは書き込みが成功すると、自動的に実行を開始します。

//image[arduino-01-exe][コードの書き込み]{
}

左から2番目のボタンを押すと、コンパイルと書き込みが行われます。一番左のボタンは
コンパイルのみ行います。

//image[arduino-01-compile][コードのコンパイル中]{
}

//image[arduino-01-done][コードの書き込み完了]{
}

さて、書き込みが完了したらArduinoを見てみましょう。見るのはこの辺です。

//image[arduino-01-l][Lチカ]{
}

"L"と書かれたオレンジ色のLEDがゆっくりと点滅しているのが確認できるはずです。
そしてこの点滅の間隔は約1秒となっています。これがArduinoの"hello World"です。

== LEDを光らせる

実際のLEDデバイスを使ってみましょう。ブレッドボードとジャンパー
を使って回路を組んでいきます。ブレッドボードは内部で以下のような接続がされていて、
表面にジャンパーを差すだけで回路が作れるようになっています。端の方は横方向、
中の方は縦方向に繋がっています。これは端の方には電源やGND（グランド）を接続
することが多いためです。

//image[arduino-01-breda][ブレッドボード結線図]{
}


Cdsセルを使う：ジャンパー３本
アナログだと +2本
抵抗 10kΩ