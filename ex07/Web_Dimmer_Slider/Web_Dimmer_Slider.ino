#include <WiFi.h>
#include <WebServer.h>

const char* ssid     = "zcc的ipone";
const char* password = "88888888";

const int ledPin = 2;
const int freq = 5000;
const int resolution = 8;

WebServer server(80);
int ledBright = 0;

void handleRoot() {
  // 精简稳健HTML，避免JS语法错误导致页面空白
  String html = "<!DOCTYPE html><html lang='zh-CN'>"
  "<head>"
  "<meta charset='UTF-8'>"
  "<meta name='viewport' content='width=device-width, initial-scale=1.0'>"
  "<title>ESP32 网页无极调光</title>"
  "<style>"
  "body{font-family:Arial;text-align:center;margin-top:60px;}"
  ".slider-box{width:85%;margin:30px auto;}"
  "#brightSlider{width:100%;height:25px;}"
  "#valShow{font-size:24px;font-weight:bold;margin:15px 0;}"
  "</style>"
  "</head>"
  "<body>"
  "<h1>Web 滑动条无极调光器</h1>"
  "<div class='slider-box'>"
  "<input type='range' id='brightSlider' min='0' max='255' value='0'>"
  "<div>当前亮度：<span id='valShow'>0</span></div>"
  "</div>"
  "<script>"
  "let slider = document.getElementById('brightSlider');"
  "let showVal = document.getElementById('valShow');"
  "slider.addEventListener('input',function(){"
  "let v = this.value;"
  "showVal.innerText = v;"
  "fetch('/set?bright='+v);"
  "});"
  "</script>"
  "</body></html>";
  server.send(200, "text/html; charset=UTF-8", html);
}

void handleSetBright() {
  if(server.hasArg("bright")){
    ledBright = server.arg("bright").toInt();
    ledBright = constrain(ledBright, 0, 255);
    ledcWrite(ledPin, ledBright);
  }
  server.send(200, "text/plain", "OK:"+String(ledBright));
}

void setup() {
  Serial.begin(115200);
  ledcAttach(ledPin, freq, resolution);
  ledcWrite(ledPin, 0);

  Serial.print("正在连接热点：");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi连接成功！");
  Serial.print("访问地址：http://");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/set", handleSetBright);
  server.begin();
}

void loop() {
  server.handleClient();
}