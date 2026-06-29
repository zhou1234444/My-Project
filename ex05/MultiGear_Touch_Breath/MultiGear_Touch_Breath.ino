// 引脚定义，沿用实验4配置
#define TOUCH_PIN 4
#define LED_PIN 2
#define EXT_ISR_MODE 0

// 适配：触摸≈350，松开≈1000，阈值设中间值600
int threshold = 600;
int touchValue;

// 触摸防抖、边沿检测变量
int lastTouchState = 0;
unsigned long debounceTime = 50;
unsigned long lastDebounceTime = 0;

// 多档位调速核心
int gear = 1;          // 当前档位 1/2/3
int ledDuty = 0;       // PWM占空比 0~255
int step;              // 每档对应的呼吸步长
bool increase = true;  // 亮度递增/递减标记

void setup() {
  Serial.begin(115200);
  delay(1000);
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  unsigned long now = millis();
  touchValue = touchRead(TOUCH_PIN);
  Serial.printf("Touch:%d Gear:%d\n", touchValue, gear);

  // 触摸判定：数值小于600代表手指触碰
  int currTouchState = (touchValue < threshold) ? 1 : 0;

  // 触摸上升沿 + 软件防抖，切换档位
  if ((now - lastDebounceTime) > debounceTime) {
    if (currTouchState == 1 && lastTouchState == 0) {
      gear = gear % 3 + 1; // 档位循环 1→2→3→1
      lastDebounceTime = now;
      Serial.printf("档位切换至：%d\n", gear);
    }
    lastTouchState = currTouchState;
  }

  // 不同档位对应不同呼吸速度步长
  switch(gear) {
    case 1: step = 2;  break; // 慢速呼吸
    case 2: step = 5;  break; // 中速呼吸
    case 3: step = 10; break; // 快速呼吸
  }

  // PWM呼吸渐变逻辑
  if(increase) {
    ledDuty += step;
    if(ledDuty >= 255) increase = false;
  } else {
    ledDuty -= step;
    if(ledDuty <= 0) increase = true;
  }
  analogWrite(LED_PIN, ledDuty);
  delay(15);
}