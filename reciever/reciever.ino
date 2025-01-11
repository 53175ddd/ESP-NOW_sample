#include <esp_now.h>
#include <WiFi.h>

#define LED_BUILTIN 4  // 回路に合わせて変更

void esp_now_callback(const esp_now_recv_info *esp_now_recieve_info, const unsigned char *recieve_data, int length) {
  char recieve_data_buffer[length];  // 受信データ格納用

  sprintf(recieve_data_buffer, "%s", recieve_data);

  Serial.print(recieve_data_buffer);
}

void setup() {
  Serial.begin(115200);  // シリアルデバッグ用

  WiFi.mode(WIFI_STA);  // Wi-Fi モードをステーションモードに設定

  delay(100);  // 一応待ってみる

  Serial.print("MAC Address: ");
  Serial.println(WiFi.macAddress());

  pinMode(LED_BUILTIN, OUTPUT);

  if(esp_now_init() == ESP_OK){  // 初期化が正しく通ったかの判別
    Serial.println("Success to initialize ESP-NOW");
  }else {  // 初期化に失敗した時の処理
    Serial.println("Error initializing ESP-NOW");

    while(true) {  // 初期化失敗したときに入る無限ループ
      digitalWrite(LED_BUILTIN, HIGH);
      delay(50);
      digitalWrite(LED_BUILTIN,  LOW);
      delay(50);
    }
  }

  esp_now_register_recv_cb(esp_now_callback);  // データ受信時に呼び出される関数を登録
}

void loop() {
}
