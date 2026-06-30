#include <WiFi.h>
#include <WebServer.h>

// ========= WiFi热点配置（你的手机热点） =========
const char* ssid     = "zcc的ipone";
const char* password = "88888888";

// ========= 硬件引脚定义 =========
#define TOUCH_PIN 4
#define ALARM_LED 2
int threshold = 600;
int touchValue;

// ========= 系统全局状态变量（题目要求） =========
bool isArm = false;       // 系统布防状态：true=布防  false=撤防
bool isAlarmTrigger = false; // 是否触发报警锁定

// ========= 防抖边沿检测变量（复用实验4触摸逻辑） =========
int lastTouchState = 0;
unsigned long debounceTime = 50;
unsigned long lastDebounceTime = 0;

WebServer server(80);

// ========= 网页主页：布防/撤防两个按钮 =========
void handleRoot() {
  String pageHtml = R"rawliteral(
<!DOCTYPE html>
<html lang="zh-CN">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>物联网安防报警器</title>
<style>
body{text-align:center;font-family:Arial;margin-top:80px;}
.btn{font-size:22px;padding:15px 40px;margin:20px 15px;cursor:pointer;border:none;border-radius:8px;}
#armBtn{background-color:#c82423;color:white;}
#disarmBtn{background-color:#2382de;color:white;}
#stateTip{font-size:24px;margin-top:30px;font-weight:bold;}
</style>
</head>
<body>
<h1>ESP32 安防报警控制系统</h1>
<button class="btn" id="armBtn">布防 Arm</button>
<button class="btn" id="disarmBtn">撤防 Disarm</button>
<div id="stateTip">当前状态：未布防</div>

<script>
const armBtn = document.getElementById("armBtn");
const disarmBtn = document.getElementById("disarmBtn");
const stateTip = document.getElementById("stateTip");

// 布防按钮请求
armBtn.onclick = function(){
  fetch("/arm");
  stateTip.innerText = "当前状态：已布防，等待入侵触发";
}
// 撤防按钮请求
disarmBtn.onclick = function(){
  fetch("/disarm");
  stateTip.innerText = "当前状态：已撤防，系统休眠";
}
</script>
</body>
</html>
)rawliteral";
  server.send(200, "text/html; charset=UTF-8", pageHtml);
}

// 布防接口 /arm
void handleArm() {
  isArm = true;
  server.send(200, "text/plain", "系统已布防");
}

// 撤防接口 /disarm
void handleDisarm() {
  isArm = false;
  isAlarmTrigger = false; // 撤防同时清除报警锁定
  digitalWrite(ALARM_LED, LOW);
  server.send(200, "text/plain", "系统已撤防，报警复位");
}

void setup() {
  Serial.begin(115200);
  pinMode(ALARM_LED, OUTPUT);
  digitalWrite(ALARM_LED, LOW);

  // 连接手机热点
  WiFi.begin(ssid, password);
  Serial.print("正在连接热点：");
  Serial.println(ssid);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi连接成功");
  Serial.print("访问网页地址：http://");
  Serial.println(WiFi.localIP());

  // 注册网页路由
  server.on("/", handleRoot);
  server.on("/arm", handleArm);
  server.on("/disarm", handleDisarm);
  server.begin();
}

void loop() {
  server.handleClient();
  unsigned long now = millis();
  touchValue = touchRead(TOUCH_PIN);

  // 触摸状态判断
  int currTouchState = (touchValue < threshold) ? 1 : 0;

  // 防抖+上升沿检测（触碰瞬间触发）
  if ((now - lastDebounceTime) > debounceTime) {
    if (currTouchState == 1 && lastTouchState == 0) {
      // 仅【布防状态】才触发报警锁定
      if (isArm == true) {
        isAlarmTrigger = true;
      }
      lastDebounceTime = now;
    }
    lastTouchState = currTouchState;
  }

  // 报警锁定逻辑：触发后LED高频闪烁，直到网页撤防
  if (isAlarmTrigger == true) {
    digitalWrite(ALARM_LED, HIGH);
    delay(80);
    digitalWrite(ALARM_LED, LOW);
    delay(80);
  } else {
    digitalWrite(ALARM_LED, LOW);
  }
}
