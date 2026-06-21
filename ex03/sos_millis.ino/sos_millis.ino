#define LED_PIN 2

// 时序定义 单位ms
const int SHORT_BLINK = 200;    // 短亮
const int LONG_BLINK  = 600;    // 长亮
const int GAP_BETWEEN = 200;    // 单个闪烁间隔
const int GROUP_GAP   = 600;    // 三组SOS段间隔
const int SOS_PAUSE   = 1500;   // 一轮SOS结束总停顿

unsigned long previousTime = 0;
bool ledState = LOW;
int stateIndex = 0;
unsigned long duration = 0;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(115200);
  Serial.println("SOS 摩尔斯码闪烁程序启动");
}

void loop() {
  unsigned long now = millis();
  if (now - previousTime >= duration) {
    previousTime = now;

    switch(stateIndex) {
      // S：3次短闪
      case 0: ledState=1; duration=SHORT_BLINK; break;
      case 1: ledState=0; duration=GAP_BETWEEN; break;
      case 2: ledState=1; duration=SHORT_BLINK; break;
      case 3: ledState=0; duration=GAP_BETWEEN; break;
      case 4: ledState=1; duration=SHORT_BLINK; break;
      case 5: ledState=0; duration=GROUP_GAP; break;

      // O：3次长闪
      case 6: ledState=1; duration=LONG_BLINK; break;
      case 7: ledState=0; duration=GAP_BETWEEN; break;
      case 8: ledState=1; duration=LONG_BLINK; break;
      case 9: ledState=0; duration=GAP_BETWEEN; break;
      case 10:ledState=1; duration=LONG_BLINK; break;
      case 11:ledState=0; duration=GROUP_GAP; break;

      // S：3次短闪
      case 12:ledState=1; duration=SHORT_BLINK; break;
      case 13:ledState=0; duration=GAP_BETWEEN; break;
      case 14:ledState=1; duration=SHORT_BLINK; break;
      case 15:ledState=0; duration=GAP_BETWEEN; break;
      case 16:ledState=1; duration=SHORT_BLINK; break;
      case 17:ledState=0; duration=SOS_PAUSE; break;

      // 一轮SOS完成，从头循环
      default:
        stateIndex = -1;
        Serial.println("一轮 SOS 发送完成");
        ledState = 0;
        duration = 0;
        break;
    }

    digitalWrite(LED_PIN, ledState);
    stateIndex++;
  }
}