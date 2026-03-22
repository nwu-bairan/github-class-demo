// --- 1. 引脚定义 ---
const int ledPin = 2;      // 主 LED
const int LED_green = 26;  // 绿灯
const int LED_red = 27;    // 红灯
const int LED_yellow = 25; // 黄灯

// --- 2. 时间参数设置 (单位: 毫秒) ---
const unsigned long shortOnTime = 200;  // 短闪（S）亮灯时间
const unsigned long longOnTime = 600;   // 长闪（O）亮灯时间
const unsigned long offTime = 200;      // 字符内部的熄灭时间
const unsigned long pauseTime = 1500;   // 一轮 SOS 结束后的长停顿

// --- 3. 变量定义 ---
unsigned long previousMillis = 0; // 记录上一次状态改变的时间
int currentStep = 0;              // 记录当前处于 SOS 序列的哪一步 (0-18)

void setup() {
  // 初始化所有引脚为输出模式
  pinMode(ledPin, OUTPUT);
  pinMode(LED_green, OUTPUT);
  pinMode(LED_red, OUTPUT);
  pinMode(LED_yellow, OUTPUT);
  
  // 初始状态全部熄灭
  turnAllLEDs(LOW);
}

void loop() {
  unsigned long currentMillis = millis(); // 获取当前时间

  // 检查是否到了该改变状态的时间
  // getDuration(currentStep)告诉需要等待多久
  if (currentMillis - previousMillis >= getDuration(currentStep)) {
    
    previousMillis = currentMillis; // 更新时间

    switch (currentStep) {
      
      // === 第一部分：3次短闪 (S) ===
      // 步骤 0, 2, 4 是亮灯
      case 0: case 2: case 4: 
        turnAllLEDs(HIGH); // 亮
        currentStep++; 
        break;
        
      // 步骤 1, 3, 5 是灭灯
      case 1: case 3: case 5: 
        turnAllLEDs(LOW);  // 灭
        currentStep++; 
        break;

      // === 第二部分：3次长闪 (O) ===
      // 步骤 6, 8, 10 是亮灯
      case 6: case 8: case 10: 
        turnAllLEDs(HIGH); // 亮
        currentStep++; 
        break;
        
      // 步骤 7, 9, 11 是灭灯
      case 7: case 9: case 11: 
        turnAllLEDs(LOW);  // 灭
        currentStep++; 
        break;

      // === 第三部分：3次短闪 (S) ===
      // 步骤 12, 14, 16 是亮灯
      case 12: case 14: case 16: 
        turnAllLEDs(HIGH); // 亮
        currentStep++; 
        break;
        
      // 步骤 13, 15, 17 是灭灯
      case 13: case 15: case 17: 
        turnAllLEDs(LOW);  // 灭
        currentStep++; 
        break;

      // === 第四部分：长停顿 ===
      // 步骤 18 是停顿结束，重置回 0
      case 18: 
        currentStep = 0; 
        break;
    }
  }
}

// --- 辅助函数：根据当前步骤返回持续时间 ---
unsigned long getDuration(int step) {
  // 如果是亮灯步骤 (偶数步)
  if (step % 2 == 0) {
    if (step == 18) return pauseTime; // 最后的停顿
    
    // 中间的长闪 (步骤 6, 8, 10)
    if (step == 6 || step == 8 || step == 10) return longOnTime; 
    
    // 其余的短闪
    return shortOnTime; 
  } 
  // 如果是灭灯步骤 (奇数步)，统一熄灭时间
  else {
    return offTime; 
  }
}

// --- 辅助函数：一次性控制所有 LED 的开关 ---
void turnAllLEDs(int state) {
  digitalWrite(ledPin, state);
  digitalWrite(LED_green, state);
  digitalWrite(LED_red, state);
  digitalWrite(LED_yellow, state);
}