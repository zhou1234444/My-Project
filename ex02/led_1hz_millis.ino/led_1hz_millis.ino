// ESP32板载LED引脚定义
#define LED_PIN 2

// 记录上一次状态翻转的时间
unsigned long previousTime = 0;
// 翻转间隔 500ms，整体周期1000ms = 1Hz
const unsigned long interval = 500;
// LED当前状态标记
bool ledState = LOW;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(115200);
  Serial.println("程序启动，LED以1Hz频率闪烁");
}

void loop() {
  // 获取当前系统运行总毫秒数
  unsigned long currentTime = millis();

  // 判断是否达到定时间隔
  if (currentTime - previousTime >= interval)
  {
    previousTime = currentTime;
    // 翻转LED电平
    ledState = !ledState;
    digitalWrite(LED_PIN, ledState);
    
    // 串口打印当前时间与LED状态，其余逻辑完全不变
    if (ledState == HIGH)
    {
      Serial.printf("time: %lu ms, LED ON\n", currentTime);
    }
    else
    {
      Serial.printf("time: %lu ms, LED OFF\n", currentTime);
    }
  }
}