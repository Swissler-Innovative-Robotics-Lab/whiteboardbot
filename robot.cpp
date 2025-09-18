#pragma once
/*/////////////////////////////////////////////////////////////
// Author: Petras Swissler
// Description: Basically a container for robot variables and
                other logic.
*//////////////////////////////////////////////////////////////
// Include Libraries
#include <Arduino.h>
#include "robot.h"
// Methods -------------------------------------------
// Instantiates class
robot::robot(void) {}
robot::~robot(void) {}

// Logic methods
void robot::runLogic(void)
{
    resetOutputsToNull();

    // This is the main logic function
    switch(behavior)
    {
        case BEHAVIOR_IDLE:
            // Do Nothing
            break;

        case BEHAVIOR_HEARTBEAT:
            behaviorReport();
            break;

        case BEHAVIOR_EXAMPLE:
            behaviorExample();
            break;

        case COLOR_SENSE_TEST:
            whats_the_color();
            break;

        case WHERE_TO_GO:
            move_test();
            break;

        case BEHAVIOR_1:
        behavior_spatial_programming();
            break;

        default:
            // Do Nothing
            break;
    }
}

////////////////////////////////////////////////////////////////
// Behavior methods -------------------------------------------
///////////////////////////////////////////////////////////////
void robot::resetOutputsToNull(void)
{
    drive_forward = 0;
    drive_spin = 0;

    rgb_r = 0;
    rgb_g = 0;
    rgb_b = 0;
    color_sensor_white_led = 0;
}

void robot::behaviorReport(void)
{
    // Print all sensor information then delay for some duration
    drawHoriz(20, '#');
    Serial.print(F("Time, dt:           "));
    Serial.print(this_time);
    Serial.print(F(", "));
    Serial.print(dt);
    Serial.println();

    Serial.print(F("Accelerometer xyz:  "));
    Serial.print(ax);
    Serial.print(F(", "));
    Serial.print(ay);
    Serial.print(F(", "));
    Serial.print(az);
    Serial.println();

    Serial.print(F("Theta:  "));
    Serial.print(_theta);
    Serial.println();

    Serial.print(F("Gyroscope xyz:      "));
    Serial.print(gx);
    Serial.print(F(", "));
    Serial.print(gy);
    Serial.print(F(", "));
    Serial.print(gz);
    Serial.println();

    Serial.print(F("Magnetometer xyz:   "));
    Serial.print(mx);
    Serial.print(F(", "));
    Serial.print(my);
    Serial.print(F(", "));
    Serial.print(mz);
    Serial.println();

    Serial.print(F("Temperature:        "));
    Serial.print(temperature);
    Serial.println();

    Serial.print(F("Battery V, pct:     "));
    Serial.print(battery_voltage);
    Serial.print(F(", "));
    Serial.print(battery_percent);
    Serial.println();

    Serial.print(F("Color rgb sense:    "));
    Serial.print(sense_red);
    Serial.print(F(", "));
    Serial.print(sense_green);
    Serial.print(F(", "));
    Serial.print(sense_blue);
    Serial.print(F(", "));
    Serial.print(_brightness);
    Serial.println();

    Serial.print(F("Hue:                "));
    Serial.print(_hue);
    Serial.println();

    Serial.print(F("Color index:        "));
    Serial.print(_color);
    Serial.println();
}

void robot::whats_the_color(void)
{
    Serial.print(F("Hue:    "));
    Serial.print(_hue);
    Serial.println();
    Serial.print(F("Color Index:   "));
    Serial.print(_color);
    Serial.println();
}

void robot::move_test(void)
{
    correction(de_theta, _theta, drive_forward, drive_spin);
    
    Serial.print(F("Current Theta:  "));
    Serial.print(_theta);
    Serial.println();

    Serial.print(F("Desired Theta:  "));
    Serial.print(de_theta);
    Serial.println();

    Serial.print(F("Forward:        "));
    Serial.print(drive_forward);
    Serial.println();

    Serial.print(F("Spin:           "));
    Serial.print(drive_spin);
    Serial.println();
}

void robot::behaviorExample(void)
{
    /*inputColor(sense_red, sense_green, sense_blue, sense_clear);*/
    switch(action)
    {
        case ACTION_INIT:
            // No initialization actions, transition to action_forward
            action = ACTION_EXAMPLE_FORWARD;
            break;
        // ------------------------------------------------
        case ACTION_EXAMPLE_FORWARD:
            // Drive directly forward
            drive_forward = 1;
            drive_spin = 0;
            // Condition: if the level of red light falls below 200 (of 255), transition to spin action
            if(_color == 2 )
            {
                action = ACTION_EXAMPLE_SPIN;
            }
            break;
        // ------------------------------------------------
        case ACTION_EXAMPLE_SPIN:

            drive_forward = 0;
            drive_spin = 1;
            // Condition: if level of red light goes above threshold, go forward again
            if(_color == 6)
            {
                action = ACTION_EXAMPLE_FORWARD;
            }
            break;
        // ------------------------------------------------
        default:
            // nothing
            drive_forward = 0;
            drive_spin = 0;
            break;
    }
}

void robot::behavior_spatial_programming(void)
{
    static uint8_t previous_color = _color;
    static uint32_t c_c = millis();
	switch(action)
	{

        case ACTION_INIT:
            // No initialization actions
            action = GREEN_ACTION;
            break;
        // ------------------------------------------------
		case GREEN_ACTION:

            Serial.println();
	        Serial.print("Pass ==================================================== ");
            Serial.println();

            Serial.print(F("Time:            "));
            Serial.print(millis());
            Serial.println();

			move_x = -1;
			move_y = 0;


            Serial.println();
	        Serial.print("Pass ==================================================== ");
            Serial.println();

            de_theta = orientation(move_y, move_x);

            Serial.print(F("Desired Theta:   "));
            Serial.print(de_theta);
            Serial.println();

            correction(de_theta, _theta, drive_forward, drive_spin);

            if(color_count(_color, previous_color, c_c) && _color == 6)
            {
            Serial.print("I see blue");
            Serial.println();

                action = BLUE_ACTION;
            }
			break;
        // ------------------------------------------------
		case BLUE_ACTION:

            Serial.println();
	        Serial.print("Pass ==================================================== ");
            Serial.println();

            Serial.print(F("Time:            "));
            Serial.print(millis());
            Serial.println();

			move_x = 0;
			move_y = 1;

            Serial.println();
	        Serial.print("Pass ==================================================== ");
            Serial.println();

            de_theta = orientation(move_y, move_x);

            Serial.print(F("Desired Theta:   "));
            Serial.print(de_theta);
            Serial.println();

            correction(de_theta, _theta, drive_forward, drive_spin);

            if(color_count(_color, previous_color, c_c) && _color == 4)
            {
            Serial.print("I see green");
            Serial.println();
            
            
                action = GREEN_ACTION;
            }
			break;
        // ------------------------------------------------
		default:
        
            drive_forward = 1;
            drive_spin = 0;

            break;
	}
	
}

///////////////////////////////////////////////////////////////
// Input methods ---------------------------------------
///////////////////////////////////////////////////////////////
void robot::inputIMU(float _ax, float _ay, float _az, float _gx, float _gy, float _gz)
{
    ax = _ax;
    ay = _ay;
    az = _az;
    // Todo: find unit vector

    gx = _gx;
    gy = _gy;
    gz = _gz;
    // Todo: make the calibration happen. probably not needed

    _theta = orientation(_ax, _ay);

    Serial.print(F("Current Theta:   "));
    Serial.print(_theta);
    Serial.println();

    //movement(de_theta, _theta, drive_forward, drive_spin);
}

void robot::inputMag(float _mx, float _my, float _mz)
{
    mx = _mx;
    my = _my;
    mz = _mz;
    // Todo: Need to account for how these magnetometer readings actually point to north
}

void robot::inputTemperature(float _temperature)
{
    temperature = _temperature;
}

void robot::inputBattery(float _voltage, float _percent)
{
    battery_voltage = _voltage;
    battery_percent = _percent;
}

void robot::setInputTime(uint32_t _time)
{
    last_time = this_time;
    this_time = _time;
    dt = (this_time - last_time) / 1000;
}

void robot::inputColor(float _red, float _green, float _blue, float _clear)
{   
    /*sense_red   = _red;
    sense_green = _green;
    sense_blue  = _blue;
    sense_clear = _clear;*/

    // Here is where to put the color interpretation code
    _hue = calchue(_red, _green, _blue);
    _brightness = intensity(_red, _green, _blue);
    _color = determineColor(_hue, _brightness, THRESHOLD_WHITE, THRESHOLD_BLACK);
    
    // Serial.print(F("Hue:             "));
    // Serial.print(_hue);
    // Serial.println();
    
    // Serial.print(F("Brightness:      "));
    // Serial.print(_brightness);
    // Serial.println();

    // Serial.print(F("Color Index:     "));
    // Serial.print(_color);
    // Serial.println();
}

void addDeviceToMesh(uint8_t[6] mac){//To be implemented in the datarecv funct
    sendBroadcast("ADDTOMESH: "+ (char*)mac)
}
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

    // Send message via ESP-NOW to the second broadcast address
    esp_err_t result2 = esp_now_send(broadcastAddress2, (uint8_t *)&myData, sizeof(myData));
    if (result2 == ESP_OK) {
        Serial.println("Sent with success to broadcastAddress2");
    } else {
        Serial.println("Error sending the data to broadcastAddress2");
    }
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
void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len){
    pixels.clear();
    pixels.setPixelColor(0, pixels.Color(50, 205, 0)); // green
    pixels.show();
    memcpy(&myData, incomingData, sizeof(myData));
    Serial.print("Bytes received: ");
    Serial.println(len);
    Serial.print("Char: ");
    Serial.println(myData.a);

    if(strstr(myData.a,"update")!=NULL)
        triggerUpdate();
  /*else if(msg.indexOf("NEWIP: ")!=-1 && msg.indexOf(" ENDIP")!=0)// Send a message like "NEWIP: 199.168.11 ENDIP"
    IP = msg.substring(msg.indexOf("NEW IP: ") + strlen("NEW IP: "), msg.indexOf(" ENDIP"));*/
    else{
        uint8_t tempMac[6];
        memcpy(tempMac, mac, sizeof(broadcastAddress));
        rcPtr(tempMac, myData.a);
  }
}

void meshSetup(RCFunctionPointer rcPtrFunc){
    Serial.begin(115200);

  rcPtr = rcPtrFunc;
  
  pixels.begin();
  pixels.clear();
  preferences.begin("my-app", false);
  //preferences.putUInt("task",0);
  task = preferences.getUInt("task",0);//defaults to 0 if task variable not found
  if (task==1){
    
    pixels.setPixelColor(0, pixels.Color(255,0,255)); // display magenta to show in update mode
    pixels.show();
    HttpsOTA.onHttpEvent(_h);
    WiFi.begin(Wifi_SSID, Wifi_PASSWORD);
    
  }else{
    
    pixels.setPixelColor(0, pixels.Color(0,0,255,0,0));//display red to show in mesh mode
    pixels.show();
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
    void otaStuff(){
  otastatus = HttpsOTA.status();
  if(otastatus == HTTPS_OTA_SUCCESS) {
    if(!task_ran){ 
      //tell mesh network update available
      Serial.println("Firmware written successfully. To reboot device, call API ESP.restart() or PUSH restart button on device");
      pixels.clear();
      pixels.setPixelColor(0, pixels.Color(0, 255, 0));//Green to show successful update
      pixels.show();
      preferences.putUInt("task",0);
      preferences.end();
    }
  } else if(otastatus == HTTPS_OTA_FAIL) { 
      Serial.println("Firmware Upgrade Fail");
      pixels.clear();
      pixels.setPixelColor(0, pixels.Color(255, 0, 0));//Red to show unsuccessful update
      pixels.show();
  }
  if(notRun){
    HttpsOTA.begin(url, server_certificate); 
    notRun = false;
    Serial.print("Waiting on OTA Update");
  }
}
}

