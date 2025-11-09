/*
  Smart Plant Pot - Automatic Plant Care System
  작성자: 손충락
  설명:
  - 토양 수분이 일정 이하일 경우 펌프 작동 (자동 급수)
  - 조도가 일정 이하일 경우 LED 조명 점등
  - HC-06 블루투스를 통한 원격 수동 제어 가능
*/

// 센서 및 제어 핀 설정
#define SOIL_PIN   A0   // 토양습도 센서 입력
#define LIGHT_PIN  A1   // 조도 센서 입력
#define PUMP_PIN   8    // 펌프 릴레이 출력
#define LED_PIN    9    // LED 조명 출력

// 블루투스 통신 설정
#define BAUDRATE 9600

// 임계값 (상황에 맞게 조정 가능)
int soilThreshold = 500;   // 토양 습도 기준 (0~1023)
int lightThreshold = 300;  // 조도 기준 (0~1023)

void setup() {
  Serial.begin(BAUDRATE);
  pinMode(PUMP_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  
  digitalWrite(PUMP_PIN, LOW);
  digitalWrite(LED_PIN, LOW);

  Serial.println("Smart Plant Pot Initialized");
}

void loop() {
  int soilValue = analogRead(SOIL_PIN);
  int lightValue = analogRead(LIGHT_PIN);

  Serial.print("Soil: "); Serial.print(soilValue);
  Serial.print(" | Light: "); Serial.println(lightValue);

  // 자동 급수 조건
  if (soilValue > soilThreshold) {
    digitalWrite(PUMP_PIN, HIGH);
    Serial.println("Water Pump ON");
  } else {
    digitalWrite(PUMP_PIN, LOW);
  }

  // 조도 감지에 따른 조명 제어
  if (lightValue < lightThreshold) {
    digitalWrite(LED_PIN, HIGH);
    Serial.println("LED ON (Low Light)");
  } else {
    digitalWrite(LED_PIN, LOW);
  }

  // 블루투스 수동 제어 명령 처리
  if (Serial.available()) {
    char cmd = Serial.read();
    if (cmd == 'W') { // 수동 급수
      digitalWrite(PUMP_PIN, HIGH);
      Serial.println("Manual Watering ON");
      delay(3000);
      digitalWrite(PUMP_PIN, LOW);
      Serial.println("Manual Watering OFF");
    }
    else if (cmd == 'L') { // 조명 토글
      digitalWrite(LED_PIN, !digitalRead(LED_PIN));
      Serial.println("LED Toggled via Bluetooth");
    }
  }

  delay(1000); // 1초 간격으로 반복
}
