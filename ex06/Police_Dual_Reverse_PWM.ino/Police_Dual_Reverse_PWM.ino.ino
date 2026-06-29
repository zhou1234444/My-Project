// 外接两路LED引脚定义
const int ledA_Pin = 16;
const int ledB_Pin = 17;

// 和你实验三参数完全保持一致
const int freq = 5000;
const int resolution = 8;

// 手动分配两个独立PWM硬件通道（ESP32 0~15通道互不冲突）
const int chanA = 0;
const int chanB = 1;

int dutyA = 0;
int step = 1;

void setup() {
  Serial.begin(115200);
  // 配置PWM通道
  ledcSetup(chanA, freq, resolution);
  ledcSetup(chanB, freq, resolution);
  // 通道绑定外接LED引脚
  ledcAttachPin(ledA_Pin, chanA);
  ledcAttachPin(ledB_Pin, chanB);
}

void loop() {
  dutyA += step;

  // 到达最大/最小值反向
  if (dutyA >= 255 || dutyA <= 0) {
    step = -step;
  }

  // 核心反相逻辑：A变亮 = B变暗
  int dutyB = 255 - dutyA;

  ledcWrite(chanA, dutyA);
  ledcWrite(chanB, dutyB);

  delay(8);
}