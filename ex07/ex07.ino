#include <WiFi.h>
#include <WebServer.h>

// WiFi 设置
const char* ssid = "Arco_br";
const char* password = "12345678";

// 硬件定义
#define LED_PIN 2  // 如果GPIO 2不行，尝试改为 4 或板载LED对应的引脚

WebServer server(80);

// HTML 代码
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>ESP32 LED 调光</title>
  <style>
    body { font-family: sans-serif; text-align: center; margin-top: 50px; background: #f0f0f0; }
    h1 { color: #333; }
    .slider-container { width: 80%; margin: 0 auto; }
    input[type=range] { width: 100%; height: 30px; -webkit-appearance: none; background: #ddd; }
    input[type=range]::-webkit-slider-thumb { -webkit-appearance: none; height: 30px; width: 30px; background: #007bff; cursor: pointer; }
  </style>
</head>
<body>
  <h1>ESP32 LED 亮度控制</h1>
  <div class="slider-container">
    <input type="range" min="0" max="255" value="0" class="slider" id="brightnessSlider">
    <p>当前数值: <span id="val">0</span></p>
  </div>

  <script>
    var slider = document.getElementById("brightnessSlider");
    slider.oninput = function() {
      var val = this.value;
      document.getElementById("val").innerHTML = val;
      // 发送请求到 ESP32
      fetch('/set?val=' + val);
    }
  </script>
</body>
</html>
)rawliteral";

// 处理函数
void handleRoot() {
  server.send(200, "text/html", index_html);
}

void handleSet() {
  if (server.hasArg("val")) {
    String valStr = server.arg("val");
    int val = valStr.toInt();
    
    // 限制范围
    val = constrain(val, 0, 255);
    
    // 关键：写入 PWM
    ledcWrite(LED_PIN, val);
    
    // 串口反馈，用于调试
    Serial.print("收到数值: ");
    Serial.print(val);
    Serial.print("\n");

    server.send(200, "text/plain", "OK");
  } else {
    server.send(500, "text/plain", "Missing val");
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);

  ledcAttach(LED_PIN, 5000, 8); 
  
  // 连接 WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // 路由
  server.on("/", HTTP_GET, handleRoot);
  server.on("/set", HTTP_GET, handleSet);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}