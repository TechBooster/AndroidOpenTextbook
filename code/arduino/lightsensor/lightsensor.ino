// light sensor

#define CDS A0 // Cdsセルの入力ピン
#define LED 13 // LEDピン

void setup() {
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
  Serial.println(data);
  
  // 明るさによってLEDの点灯を変化させる
  if (data > 100) {
    digitalWrite(LED, HIGH);
  } else {
    digitalWrite(LED, LOW);
  }

  delay(1000);
}

