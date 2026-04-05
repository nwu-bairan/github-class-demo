#include <Arduino.h>

// 硬件定义 
#define TOUCH_PIN 4
#define LED_PIN 2

// 速度档位参数
const unsigned long SPEED_TIME_1 = 5;  
const unsigned long SPEED_TIME_2 = 10;  
const unsigned long SPEED_TIME_3 = 20;  

// 呼吸灯参数
// 限制最大亮度为 970 
const int MAX_BRIGHTNESS = 970; 
const int MIN_BRIGHTNESS = 0;

// 全局变量
int speedLevel = 1;         
int lastTouchState = 0;     
int brightness = 0;         
int fadeAmount = 8;    

// millis() 计时变量 
unsigned long previousMillis = 0; 

void IRAM_ATTR gotTouch(); 

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  
  ledcAttach(LED_PIN, 5000, 10); 

  // 绑定触摸中断
  touchAttachInterrupt(TOUCH_PIN, gotTouch, 600);
  
}

void loop() {
  unsigned long currentMillis = millis();

  // 确定当前档位对应的间隔时间
  unsigned long interval = 0;
  if (speedLevel == 1) interval = SPEED_TIME_1;
  else if (speedLevel == 2) interval = SPEED_TIME_2;
  else interval = SPEED_TIME_3;

  // 判断是否该更新亮度
  if (currentMillis - previousMillis >= interval) {
    
    previousMillis = currentMillis;

    // 更新亮度
    brightness += fadeAmount;

    // 如果超过最大值，反转方向并限制在最大值
    if (brightness >= MAX_BRIGHTNESS) {
      brightness = MAX_BRIGHTNESS; 
      fadeAmount = -8; 
    }
    // 如果低于最小值，反转方向并限制在最小值
    else if (brightness <= MIN_BRIGHTNESS) {
      brightness = MIN_BRIGHTNESS;
      fadeAmount = 8;  
    }
    
    // 输出 PWM 信号
    ledcWrite(LED_PIN, brightness);
  }

  // 3. 检测松手状态
  if (touchRead(TOUCH_PIN) > 600) {
    lastTouchState = 0; 
  }
}

// 中断服务函数
void IRAM_ATTR gotTouch() {
  if (lastTouchState == 0) {
    speedLevel++;
    if (speedLevel > 3) {
      speedLevel = 1;
    }
    Serial.print("切换档位: ");
    Serial.println(speedLevel);
  }
  lastTouchState = 1;
}