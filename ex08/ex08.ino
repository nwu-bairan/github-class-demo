#include <WiFi.h>
#include <WebServer.h>

// --- 1. WiFi 设置 ---
const char* ssid = "Arco_br";
const char* password = "12345678";

// --- 2. 硬件定义 ---
#define LED_PIN 2        // 板载 LED
#define TOUCH_PIN 4      // 触摸引脚 (T0/GPIO4)

// --- 3. 报警阈值设置 ---
#define TOUCH_THRESHOLD 600  // 触摸阈值：读数低于 600 视为触发

// --- 4. 服务器对象 ---
WebServer server(80);

// --- 5. 全局变量：系统状态 ---
// 0 = 撤防, 1 = 布防, 2 = 报警
int systemState = 0; 

// --- 6. HTML 网页代码 ---
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>ESP32 安防主机</title>
  <style>
    body { font-family: 'Arial', sans-serif; text-align: center; margin-top: 50px; background-color: #2c3e50; color: white; }
    h1 { margin-bottom: 40px; }
    .status-box { margin: 20px auto; padding: 10px; width: 50%; border-radius: 10px; background: #34495e; }
    button { padding: 15px 30px; font-size: 18px; margin: 10px; border: none; border-radius: 5px; cursor: pointer; color: white; }
    .btn-arm { background-color: #e74c3c; } /* 红色 - 布防 */
    .btn-disarm { background-color: #2ecc71; } /* 绿色 - 撤防 */
    button:active { transform: scale(0.98); }
  </style>
</head>
<body>
  <h1>🛡️ 安防报警系统</h1>
  
  <div class="status-box">
    <h2>当前状态: <span id="statusText">撤防</span></h2>
  </div>

  <button class="btn-arm" onclick="setState(1)">布防 (Arm)</button>
  <button class="btn-disarm" onclick="setState(0)">撤防 (Disarm)</button>

  <script>
    // 定时获取状态
    setInterval(() => {
      fetch('/status')
        .then(res => res.text())
        .then(state => {
          const textElem = document.getElementById('statusText');
          if(state === '0') textElem.innerHTML = "撤防 (安全)";
          else if(state === '1') textElem.innerHTML = "布防 (警戒中...)";
          else if(state === '2') textElem.innerHTML = "⚠️ 报警! (已锁定)";
        });
    }, 500); // 每0.5秒检查一次

    // 发送控制指令
    function setState(val) {
      fetch('/action?state=' + val);
    }
  </script>
</body>
</html>
)rawliteral";

// --- 7. 处理函数 ---

// 访问主页
void handleRoot() {
  server.send(200, "text/html", index_html);
}

// 处理按钮动作 (布防/撤防)
void handleAction() {
  if (server.hasArg("state")) {
    int newState = server.arg("state").toInt();
    
    if (newState == 1) {
      systemState = 1; // 设置为布防
      Serial.println("🔒 系统已布防 (Armed)");
    } else {
      systemState = 0; // 设置为撤防
      digitalWrite(LED_PIN, LOW); // 确保撤防时灯熄灭
      Serial.println("🔓 系统已撤防 (Disarmed)"); // <--- 这里是你要求的串口反馈
    }
    server.send(200, "text/plain", "OK");
  } else {
    server.send(500, "text/plain", "Missing state");
  }
}

// 网页获取当前状态
void handleStatus() {
  server.send(200, "text/plain", String(systemState));
}

// --- 8. Setup ---
void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

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

  // 路由设置
  server.on("/", HTTP_GET, handleRoot);
  server.on("/action", HTTP_GET, handleAction); // 处理布防/撤防
  server.on("/status", HTTP_GET, handleStatus); // 网页查询状态用
  server.begin();
  Serial.println("HTTP Server Started");
}

// --- 9. Loop ---
void loop() {
  server.handleClient();

  // 核心逻辑判断
  if (systemState == 1) { 
    // 状态：布防中
    // 读取触摸值
    int touchValue = touchRead(TOUCH_PIN);
    
    // 调试用：如果想看数值可以打开下面注释，但会刷屏
    // Serial.println(touchValue); 
    
    // 如果数值小于阈值（被触摸）
    if (touchValue < TOUCH_THRESHOLD) {
      systemState = 2; // 进入报警状态
      Serial.println("🚨 检测到入侵！进入报警状态！");
    }
  } 
  else if (systemState == 2) {
    // 状态：报警锁定
    // LED 高频闪烁
    digitalWrite(LED_PIN, HIGH);
    delay(100);
    digitalWrite(LED_PIN, LOW);
    delay(100);
    // 注意：这里不需要手动改回状态0，必须等待用户网页点击撤防
  }
  else {
    // 状态：撤防
    digitalWrite(LED_PIN, LOW); // 确保灯灭
  }
}