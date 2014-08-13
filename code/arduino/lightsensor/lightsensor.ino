// light sensor

#define CDS A0 // Cdsセルの入力ピン
#define LED 13 // LEDピン

void setup() {
  // デバッグシリアルの初期化
  Serial.begin(9600);
  
  // Cdsセルを入力に設定
  pinMode(CDS, INPUT);
  // LEDを出力に設定
  pinMode(LED, OUTPUT);
}

void loop() {
  // Cdsセルからの入力
  int light = analogRead(CDS);
  // デバッグ表示
  Serial.println(light);
  
  // 明るさによってLEDの点灯を変化させる
  if (light > 600) {
    // 明るい
    digitalWrite(LED, LOW);
  } else {
    // 暗い
    digitalWrite(LED, HIGH);
  }

  delay(1000);
}

