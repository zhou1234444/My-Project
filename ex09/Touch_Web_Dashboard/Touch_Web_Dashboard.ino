#include <WiFi.h>
#include <WebServer.h>

// 你的WiFi配置
const char* ssid = "zcc";
const char* password = "88888888";

WebServer server(80);
const int touchPin = 4;

// 网页放入程序闪存PROGMEM，杜绝局部栈溢出崩溃（解决反复重启根源）
const char htmlPage[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="utf-8">
  <title>ESP32 传感器仪表盘</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    body { text-align: center; font-family: 'Courier New', Courier, monospace; background-color: #222; color: #0f0; padding-top: 20vh; }
    .sensor-box { border: 2px solid #0f0; padding: 40px; display: inline-block; border-radius: 10px; }
    .value { font-size: 80px; font-weight: bold; }
  </style>
</head>
<body>
  <div class="sensor-box">
    <h2>电容触摸传感器值</h2>
    <div class="value" id="sensorVal">--</div>
  </div>

  <script>
    // 每 200 毫秒执行一次 fetch 请求抓取最新数据
    setInterval(function() {
      fetch("/data")
        .then(response => response.text())
        .then(text => {
          document.getElementById("sensorVal").innerText = text;
        });
    }, 200);
  </script>
</body>
</html>
)rawliteral";

// 替换不兼容lambda匿名函数，通用回调函数，所有ESP32库编译运行无报错
void handleRootPage()
{
  String pageBuf = String(FPSTR(htmlPage));
  server.send(200, "text/html; charset=utf-8", pageBuf);
}

void handleDataApi()
{
  int val = touchRead(touchPin);
  server.send(200, "text/plain", String(val));
}

void setup() {
  // 上电第一行立刻初始化串口，只要程序运行，必然有打印
  Serial.begin(115200);
  Serial.println("==== ex09 传感器仪表盘上电启动 ====");

  // WiFi增加8秒超时，不会无限死锁卡死程序
  unsigned long wifiTimeOut = millis() + 8000;
  Serial.print("正在连接WiFi：");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED && millis() < wifiTimeOut)
  {
    delay(500);
    Serial.print(".");
  }

  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.println("\nIP Address: " + WiFi.localIP().toString());
  }
  else
  {
    Serial.println("\nWiFi连接超时，请核对WiFi名称、密码、必须为2.4GHz频段");
  }

  // 注册网页路由
  server.on("/", handleRootPage);
  server.on("/data", handleDataApi);
  server.begin();
}

void loop() {
  server.handleClient();
}