#include <WiFi.h>
#include <WebServer.h>

//  WiFi 设置 
const char* ssid = "Arco_br";
const char* password = "12345678";

// 硬件定义 
#define TOUCH_PIN 4      // 触摸引脚 (T0/GPIO4)

// -服务器对象 
WebServer server(80);

// HTML 网页代码 
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>ESP32 实时监控</title>
  <style>
    body { 
      font-family: 'Arial', sans-serif; 
      text-align: center; 
      margin-top: 50px; 
      background-color: #111; 
      color: #00ffcc; 
    }
    h1 { text-transform: uppercase; letter-spacing: 2px; font-size: 24px; margin-bottom: 40px;}
    
    /* 仪表盘容器 */
    .dashboard {
      display: inline-block;
      text-align: center;
      border: 4px solid #00ffcc;
      border-radius: 20px;
      padding: 20px 40px;
      box-shadow: 0 0 20px #00ffcc;
      background: #1a1a1a;
      width: 300px;
    }

    /* 标签样式：位于数字上方 */
    .label {
      font-size: 18px;
      color: #88ffdd;
      margin-bottom: 10px;
      text-transform: uppercase;
      font-weight: bold;
    }

    /* 数字样式 */
    #sensorValue {
      font-size: 80px;
      font-weight: bold;
      font-family: 'Courier New', monospace; /* 等宽字体，数字跳动时不会左右晃动 */
      margin: 0;
      line-height: 1;
    }
    
    /* 单位或状态 */
    .unit {
      font-size: 14px;
      color: #555;
      margin-top: 10px;
    }
  </style>
</head>
<body>
  <h1>触摸传感器实时监控</h1>
  
  <div class="dashboard">
    <!-- 标签：数值 -->
    <div class="label">当前数值</div>
    
    <!-- 数字显示区域 -->
    <div id="sensorValue">--</div>
    
    <div class="unit">电容读数 (Capacitance)</div>
  </div>

  <script>
    // AJAX 函数：从服务器获取数据
    function getSensorData() {
      fetch('/data')
        .then(response => response.text())
        .then(data => {
          document.getElementById("sensorValue").innerHTML = data;
        })
        .catch(err => console.log("Error: " + err));
    }

    // 每隔 100ms (0.1秒) 刷新一次数据
    setInterval(getSensorData, 100);
  </script>
</body>
</html>
)rawliteral";

// 处理根路径请求 
void handleRoot() {
  server.send(200, "text/html", index_html);
}

// 处理数据请求
void handleData() {
  int touchValue = touchRead(TOUCH_PIN);
  server.send(200, "text/plain", String(touchValue));
}

void setup() {
  Serial.begin(115200);
  
  // 连接 WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected! IP address: ");
  Serial.println(WiFi.localIP());

  // 设置路由
  server.on("/", HTTP_GET, handleRoot);
  server.on("/data", HTTP_GET, handleData); // 新增的数据接口
  
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}