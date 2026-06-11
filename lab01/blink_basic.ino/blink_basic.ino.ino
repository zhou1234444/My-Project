#define LED_PIN 2

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  Serial.println("Fast Blink!");
  digitalWrite(LED_PIN, HIGH);
  delay(500);  // 缩短为0.5秒，闪烁变快
  digitalWrite(LED_PIN, LOW);
  delay(500);
}