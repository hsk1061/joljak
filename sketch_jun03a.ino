#include "HX711.h"
#define calibration_factor -7050.0
#define DOUT 3
#define CLK 2
HX711 scale(DOUT, CLK);
#include <SoftwareSerial.h>

#define BT_RXD 5
#define BT_TXD 4
SoftwareSerial bluetooth(BT_RXD, BT_TXD);

unsigned long elapsedTime = 0; // 경과 시간
unsigned long lastTime = 0; // 마지막 체크 시간
bool timerActive = false; // 타이머 활성화 여부

void setup() {
  bluetooth.begin(9600);
  Serial.begin(9600);
  Serial.println("HX711 scale TEST");
  scale.set_scale(calibration_factor);
  scale.tare();
  Serial.println("Readings:");
}

void loop() {
  float weight = scale.get_units(10); // 10회 평균 측정 
  Serial.println(weight);

  // 무게가 특정 값 이상일 경우 타이머 시작
  if (weight > 5) { // 특정_무게는 임계값으로 설정
    if (!timerActive) {
      lastTime = millis(); // 타이머 시작 시간
      timerActive = true; // 타이머 활성화
    } else {
      // 타이머가 활성화된 상태에서 경과 시간 업데이트
      elapsedTime += millis() - lastTime; // 이전 체크 시간과 현재 시간 차이를 더함
      lastTime = millis(); // 현재 시간을 마지막 체크 시간으로 저장
    }
  } else {
    // 무게가 임계값 이하일 경우
    // 마지막 체크 시간을 업데이트하지 않고 경과 시간을 유지
    timerActive = false; // 타이머 비활성화
  }

  // 시간 변환
  unsigned long totalSeconds = elapsedTime / 1000; // 밀리초를 초로 변환
  unsigned long hours = totalSeconds / 3600; // 시 계산
  unsigned long minutes = (totalSeconds % 3600) / 60; // 분 계산
  unsigned long seconds = totalSeconds % 60; // 초 계산

  // 경과 시간 출력
  Serial.print("공부 시간: ");
  Serial.print(hours);
  Serial.print("h ");
  Serial.print(minutes);
  Serial.print("m ");
  Serial.print(seconds);
  Serial.println("s");

  // 경과 시간을 Bluetooth로 전송
  bluetooth.print("공부 시간: ");
  bluetooth.print(hours);
  bluetooth.print("h ");
  bluetooth.print(minutes);
  bluetooth.print("m ");
  bluetooth.print(seconds);
  bluetooth.println("s"); // 초 단위로 전송

  delay(1000); // 1초 간격
}
