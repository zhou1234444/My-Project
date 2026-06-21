// 定义LED引脚
const int ledPin = 2;  

// 设置PWM属性
const int freq = 5000;          
const int resolution = 8;       

void setup() {
  Serial.begin(115200);
  ledcAttach(ledPin, freq, resolution);
}

void loop() {
  for(int dutyCycle = 0; dutyCycle <= 255; dutyCycle++){   
    ledcWrite(ledPin, dutyCycle);   
    delay(30);
  }

  for(int dutyCycle = 255; dutyCycle >= 0; dutyCycle--){
    ledcWrite(ledPin, dutyCycle);   
    delay(30);
  }
  
  Serial.println("Breathing cycle completed");
}