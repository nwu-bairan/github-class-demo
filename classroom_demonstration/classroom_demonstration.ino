#define TOUCH_PIN 4
#define LED_PIN 2
#define THRESHOLD 600 // 需要根据实际测试修改此阈值

bool ledState = false;

int count = 0;


// 中断服务函数 (ISR)
void gotTouch() {
  count ++;
  if(count%2==1){
    ledState = !ledState;
    digitalWrite(LED_PIN, ledState);
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  pinMode(LED_PIN, OUTPUT);
  
  // 绑定中断函数
  touchAttachInterrupt(TOUCH_PIN, gotTouch, THRESHOLD);
}

void loop() {
  

  delay(100);
}