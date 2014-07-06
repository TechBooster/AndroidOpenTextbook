// blink LED

#define LED 13  // LEDが接続されたピン番号

// 起動時に１度だけ実行される
void setup() {
  // シリアルでバッグを有効に
  Serial.begin(9600);
  // LEDピンを出力に設定
  pinMode(LED, OUTPUT);
}

// setup後に呼ばれる
void loop() {
  // LEDピンにHIGHを出力
  digitalWrite(LED, HIGH);
  // ウェイト1000ms = 1s
  delay(1000);
  // LEDピンにLOWを出力
  digitalWrite(LED, LOW);
  // ウェイト1000ms = 1s
  delay(1000);
}

