// 定义LED引脚
const int ledPin = 2;  
#define LED_green 26
#define LED_red 27
#define LED_yellow 25


// 设置PWM属性
const int freq = 5000;          // 频率 5000Hz
const int resolution = 8;       // 分辨率 8位 (0-255)

unsigned long previousMillis = 0; // 记录上一次执行的时间
const long interval = 10;         // 对应原来的 delay(10)

int dutyCycle = 0;    // 当前亮度
int direction = 1;    // 方向：1 表示变亮，-1 表示变暗

void setup() {
  Serial.begin(115200);

  // 【新版用法】直接将引脚、频率和分辨率绑定
  // 它会自动返回一个关联的通道（如果需要的话）
  ledcAttach(ledPin, freq, resolution);
  ledcAttach(LED_green, freq, resolution);
  ledcAttach(LED_red, freq, resolution);
  ledcAttach(LED_yellow, freq, resolution);
}

void loop() {
  // 获取当前时间
  unsigned long currentMillis = millis();

  // 检查时间是否到了 (相当于检查 delay 是否结束)
  if (currentMillis - previousMillis >= interval) {
    
    previousMillis = currentMillis;

    // 每次进来只走一步
    dutyCycle += direction;

    // 判断是否到头了，用来掉头
    if (dutyCycle >= 255) {
      direction = -1; // 变亮到头了，开始变暗
    } else if (dutyCycle <= 0) {
      direction = 1;  // 变暗到头了，开始变亮
    }

    // 写入 LED 
    ledcWrite(ledPin, dutyCycle); 
    ledcWrite(LED_green, 256 - dutyCycle); 
    ledcWrite(LED_red, 256 - dutyCycle); 
    ledcWrite(LED_yellow, 256 - dutyCycle); 
  }

  
  
}