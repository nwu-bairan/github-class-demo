#define TOUCH_PIN 4
#define LED_PIN 2
#define THRESHOLD 600 

bool ledState = false;
bool lastTouchState = false; // 用于记录上一次是否被触摸

// 中断服务函数
void IRAM_ATTR gotTouch() {
  // “边缘检测”
  if (lastTouchState == false) {
    
    ledState = !ledState; // 翻转状态
    digitalWrite(LED_PIN, ledState);
    
    // 打印调试信息
    Serial.println(ledState ? "ON" : "OFF");
  }
  
  // 标记当前为“已触摸”，防止在同一个按压过程中重复触发
  lastTouchState = true;
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  
  // 绑定中断
  touchAttachInterrupt(TOUCH_PIN, gotTouch, THRESHOLD);
}

void loop() {
  // 在 loop 里检测是否松手
  int val = touchRead(TOUCH_PIN);
  
  if (val > THRESHOLD) {
    // 如果数值大于阈值，说明手已经离开了
    lastTouchState = false; 
  }
  
  delay(50); // 简单的防抖延时
}