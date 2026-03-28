#include <Arduino.h>

// 引脚定义
#define LED_A_PIN 17
#define LED_B_PIN 16

// 参数设置
const int MAX_VAL = 255; 

// 步长设置
const int FADE_STEP = 5;  

// 变量
int brightness_A = 0;
int fadeAmount = FADE_STEP;

void setup() {
  Serial.begin(115200);
  // 参数：引脚, 频率, 精度
  ledcAttach(LED_A_PIN, 5000, 8);
  ledcAttach(LED_B_PIN, 5000, 8);
  
  Serial.println("启动：严格 0-255 互补逻辑...");
}

void loop() {
  // 更新 LED A 亮度
  brightness_A += fadeAmount;

  // 计算 LED B 亮度
  // 当 A=0 时，B=255；当 A=100 时，B=155...
  int brightness_B = MAX_VAL - brightness_A;

  // 边界检测与反转
  if (brightness_A >= MAX_VAL) {
    brightness_A = MAX_VAL;
    fadeAmount = -FADE_STEP; // 变暗
  } 
  else if (brightness_A <= 0) {
    brightness_A = 0;
    fadeAmount = FADE_STEP;  // 变亮
  }

  ledcWrite(LED_A_PIN, brightness_A);
  ledcWrite(LED_B_PIN, brightness_B);

  // 延时控制速度
  delay(15); 
}