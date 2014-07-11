
// 3axis sensor

#define X_AXIS A0 // x軸
#define Y_AXIS A1 // y軸
#define Z_AXIS A2 // z軸

// 1G = 1v
// 0G = 2.5v
// offset = 1023/2

int axis[3]; // 加速度センサー値
char string[64]; // 出力文字列

void setup() {
  Serial.begin(9600);
  
  // 加速度センサーのピンを入力に設定
  pinMode(X_AXIS, INPUT);
  pinMode(Y_AXIS, INPUT);
  pinMode(Z_AXIS, INPUT);
}

void loop() {
  // 入力値を取得し、オフセットを引いておく
  axis[0] = analogRead(X_AXIS) - 511;
  axis[1] = analogRead(Y_AXIS) - 511;
  axis[2] = analogRead(Z_AXIS) - 511;

  sprintf(string, "x= %d, y= %d, z= %d", axis[0], axis[1], axis[2]);
  Serial.println(string);

  delay(500);
}

