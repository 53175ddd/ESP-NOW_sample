#include <esp_now.h>
#include <WiFi.h>

#define LED_BUILTIN 4  // 回路に合わせて変更

uint8_t target_address[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};  // 相手の MAC アドレス

esp_now_peer_info_t peer_info;  // 接続先（ピア）の情報を保持するための構造体

void esp_now_callback_tx(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "ESP-NOW data send success" : "ESP-NOW data send failed");
}

void esp_now_callback_rx(const esp_now_recv_info *esp_now_recieve_info, const uint8_t *recieve_data, int length) {
  char buffer[length];
  memcpy(&buffer, recieve_data, length);
  Serial.println(buffer);
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

  memcpy(peer_info.peer_addr, target_address, 6);
  peer_info.channel = 0;      // Wi-Fi のチャンネル選択  
  peer_info.encrypt = false;  // 暗号化の有無

  if(esp_now_add_peer(&peer_info) == ESP_OK) {
    Serial.println("Successed to add peer");
  }else {
    Serial.println("Faild to add peer");
  }

  esp_now_register_send_cb(esp_now_callback_tx);  // データ送信時に呼び出される関数を登録
  esp_now_register_recv_cb(esp_now_callback_rx);  // データ受信時に呼び出される関数を登録
}

void loop() {
  while(!Serial.available());

  delay(100);

  String arg = Serial.readString();

  char buffer[250];
  sprintf(buffer, "%s", arg.c_str());

  esp_now_send(target_address, (uint8_t*)&buffer, sizeof(buffer));
}
