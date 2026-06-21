// 定义LED引脚
const int ledPin = 2;
const int pwmChannel = 0;

// 设置PWM属性
const int freq = 5000;
const int resolution = 8;
int speedDelay = 10;   // 修改该数值改变呼吸快慢

void setup() {
  Serial.begin(115200);
  ledcSetup(pwmChannel, freq, resolution);
  ledcAttachPin(ledPin, pwmChannel);
}

void loop() {
  // 逐渐变亮
  for(int dutyCycle = 0; dutyCycle <= 255; dutyCycle++){
    ledcWrite(pwmChannel, dutyCycle);
    delay(speedDelay);
  }

  // 逐渐变暗
  for(int dutyCycle = 255; dutyCycle >= 0; dutyCycle--){
    ledcWrite(pwmChannel, dutyCycle);
    delay(speedDelay);
  }

  Serial.println("Breathing cycle completed");
}
