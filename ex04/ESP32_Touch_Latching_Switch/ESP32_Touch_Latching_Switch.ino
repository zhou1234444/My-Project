// 定义触摸引脚 (T0对应GPIO4)
#define TOUCH_PIN 4
// 定义LED引脚 (ESP32 DevKit板载LED通常是GPIO2)
#define LED_PIN 2
// 中断模式设置：0为轮询模式，1为中断模式（本次用轮询）
#define EXT_ISR_MODE 0

// 阈值，需要通过串口监视器观察并调整
int threshold = 20;
// 触摸采样值
int touchValue;

// 自锁状态变量
bool ledState = false;         // LED当前状态：false灭 true亮
int lastTouchState = 0;        // 上一次触摸状态 0未触摸 1触摸
unsigned long debounceTime = 50;    // 防抖50ms
unsigned long lastDebounceTime = 0;  // 防抖时间戳

void setup() {
  Serial.begin(115200);
  delay(1000); // 等待串口稳定

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, ledState); // 初始化LED熄灭
}

void loop() {
  unsigned long now = millis();
  touchValue = touchRead(TOUCH_PIN);

  Serial.print("Touch Value: ");
  Serial.println(touchValue);

  // 判断当前是否处于触摸状态
  int currTouchState = (touchValue < threshold) ? 1 : 0;

  // 防抖判断：满足防抖间隔才处理按键事件
  if ((now - lastDebounceTime) > debounceTime) {
    // ========== 上升沿检测：上次未触摸，本次刚触摸瞬间 ==========
    if (currTouchState == 1 && lastTouchState == 0) {
      ledState = !ledState;               // 翻转LED自锁状态
      digitalWrite(LED_PIN, ledState);
      lastDebounceTime = now;             // 更新防抖计时起点
      Serial.println("有效触摸，翻转LED");
    }
    // 更新历史触摸状态
    lastTouchState = currTouchState;
  }

  delay(100);
}
