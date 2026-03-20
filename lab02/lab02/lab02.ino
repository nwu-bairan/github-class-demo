// 定义引脚
const int PIN_RED = 27;
const int PIN_YELLOW = 25;
const int PIN_GREEN = 26;
const int PIN_ONBOARD = 2; // 板载LED通常是13

// 定义闪烁间隔 (1Hz = 1000ms)
const unsigned long INTERVAL = 500; 

// 记录上次闪烁的时间
unsigned long previousMillis = 0; 

// 记录LED当前的状态 (LOW 或 HIGH)
int ledState = LOW; 

void setup() {
  // 初始化引脚模式
  pinMode(PIN_RED, OUTPUT);
  pinMode(PIN_YELLOW, OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(PIN_ONBOARD, OUTPUT);
  
  // 初始状态全部熄灭
  digitalWrite(PIN_RED, LOW);
  digitalWrite(PIN_YELLOW, LOW);
  digitalWrite(PIN_GREEN, LOW);
  digitalWrite(PIN_ONBOARD, LOW);
}

void loop() {
  // 获取当前时间
  unsigned long currentMillis = millis();

  // 检查是否过了 1 秒
  if (currentMillis - previousMillis >= INTERVAL) {
    // 保存当前时间
    previousMillis = currentMillis;

    // 翻转状态变量
    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }

    // 同时更新所有四个 LED 的状态
    digitalWrite(PIN_RED, ledState);
    digitalWrite(PIN_YELLOW, ledState);
    digitalWrite(PIN_GREEN, ledState);
    digitalWrite(PIN_ONBOARD, ledState);
  }

}