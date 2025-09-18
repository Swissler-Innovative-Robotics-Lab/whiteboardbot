#include "mesh.h"

#ifdef INADDR_NONE
#undef INADDR_NONE
#endif

void triggerUpdate(){
    preferences.putUInt("task",1);
    preferences.end();

    delay(5000);
    ESP.restart();
}

void sendBroadcast(String msg){
    // Set values to send
    Serial.println("SENDING BROADCAST");
    strcpy(myData.a, msg.c_str());

    // Send message via ESP-NOW to the first broadcast address
    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *)&myData, sizeof(myData));
    if (result == ESP_OK) {
        Serial.println("Sent with success to broadcastAddress");
    } else {
        Serial.println("Error sending the data to broadcastAddress");
    }
    /*
    // Send message via ESP-NOW to the second broadcast address
    esp_err_t result2 = esp_now_send(broadcastAddress2, (uint8_t *)&myData, sizeof(myData));
    if (result2 == ESP_OK) {
        Serial.println("Sent with success to broadcastAddress2");
    } else {
        Serial.println("Error sending the data to broadcastAddress2");
    }*/
}
void sendSingle(uint8_t mac[6], String msg){
      // Set values to send
  Serial.println("SENT TO SINGLE BELOW");
  strcpy(myData.a, msg.c_str());

  // Send message via ESP-NOW to the first broadcast address
  esp_err_t result = esp_now_send(mac, (uint8_t *)&myData, sizeof(myData));
  if (result == ESP_OK) {
    Serial.println("Sent with success to broadcastAddress");
  } else {
    Serial.println("Error sending the data to broadcastAddress");
  }
}

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status){
    Serial.print("\r\nLast Packet Send Status:\t");
    Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}
//here is where you implement per behavior communications, reccomend if statements with functions that handle comms
void OnDataRecv(const esp_now_recv_info_t *recv_info, const uint8_t *incomingData, int len) {
    // Get MAC address like this:
    const uint8_t* mac = recv_info->src_addr;
    memcpy(&myData, incomingData, sizeof(myData));
    Serial.print("Bytes received: ");
    Serial.println(len);
    Serial.print("Char: ");
    Serial.println(myData.a);

    if (strstr(myData.a, "LIGHT UP") != NULL) {  
        // Your behavior
    }
    if (strstr(myData.a, "update") != NULL) {
        triggerUpdate();
    } else {
        uint8_t tempMac[6];
        memcpy(tempMac, mac, 6); // Use mac from recv_info
        rcPtr(tempMac, myData.a);
    }
}


void meshSetup(RCFunctionPointer rcPtrFunc){
    Serial.begin(115200);

  rcPtr = rcPtrFunc;
  preferences.begin("my-app", false);
  //preferences.putUInt("task",0);
  task = preferences.getUInt("task",0);//defaults to 0 if task variable not found
  if (task==1){
    HttpsOTA.onHttpEvent(_h);
    WiFi.begin(Wifi_SSID, Wifi_PASSWORD);
    
  }else{
    
    // Set device as a Wi-Fi Station
    WiFi.mode(WIFI_STA);

    // Init ESP-NOW
    if (esp_now_init() != ESP_OK) {
      Serial.println("Error initializing ESP-NOW");
      return;
    }

    // Once ESPNow is successfully Init, we will register for Send CB to
    // get the status of Transmitted packet
    esp_now_register_send_cb(OnDataSent);
    esp_now_register_recv_cb(OnDataRecv);

    // Register peer
    memcpy(peerInfo.peer_addr, broadcastAddress, 6);
    peerInfo.channel = 0;  
    peerInfo.encrypt = false;

    // Add peer        
    if (esp_now_add_peer(&peerInfo) != ESP_OK) {
      Serial.println("Failed to add peer");
      return;
    }

    // Register second peer
    memcpy(peerInfo2.peer_addr, broadcastAddress2, 6);
    peerInfo2.channel = 0;  
    peerInfo2.encrypt = false;

    // Add second peer        
    if (esp_now_add_peer(&peerInfo2) != ESP_OK) {
      Serial.println("Failed to add peer 2");
      return;
    }
    Serial.println("Mesh is Ready to use");
  }
}
void otaStuff(){
  otastatus = HttpsOTA.status();
  if(otastatus == HTTPS_OTA_SUCCESS) {
    if(!task_ran){ 
      //tell mesh network update available
      Serial.println("Firmware written successfully. To reboot device, call API ESP.restart() or PUSH restart button on device");
      preferences.putUInt("task",0);
      preferences.end();
    }
  } else if(otastatus == HTTPS_OTA_FAIL) { 
      Serial.println("Firmware Upgrade Fail");
  }
  if(notRun){
    HttpsOTA.begin(url, server_certificate); 
    notRun = false;
    Serial.print("Waiting on OTA Update");
  }

}