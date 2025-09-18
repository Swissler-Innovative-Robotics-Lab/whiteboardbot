#include "simple_helper_functions.h"
//replace with WiFi/Hotspot credentials
    #define Wifi_SSID "DESKTOP-GLO6DFB 4172"
    #define Wifi_PASSWORD "password"
    
    //used for OTA Updates, can use LAN router or PC hotspot
#define Wifi_SSID "DESKTOP-GLO6DFB 4172"
#define Wifi_PASSWORD "password"

//use by commenting out currnent device address (one being uploaded too
// and having 2 remaining addresses as Address and address2
//uint8_t broadcastAddress0[] = {0xf4,0x12,0xfa,0x97,0xe0,0x68};//black 
uint8_t broadcastAddress[] = {0xf4,0x12,0xfa,0x97,0xde,0x04};//white 
uint8_t broadcastAddress2[] = {0xf4,0x12,0xfa,0x97,0xe2,0x84};//yellow

//theoretically can be converted to this but had issues uppon testing
//uint8_t broadcastAddresses[][] = {broadcastAddress0,broadcastAddress, broadcastAddress2}
//then u can do curr_address = smth and then when these addresses are used (sendbroadcast) you can loop and then have condition if it isnt curr mac send msg 

void readMacAddress();

typedef struct struct_message {
   char a[32];

} struct_message;

typedef void (*FunctionPointer)();
typedef void (*RCFunctionPointer)(uint8_t[6], String); 


RCFunctionPointer rcPtr;
Preferences preferences;
unsigned int task = 0;

//OTA update setup
static const char *url = "http://10.203.172.183:8000/movement.ino.bin"; //state url of your firmware image
static const char *server_certificate = "";
static HttpsOTAStatus_t otastatus;

bool task_ran = false;
bool notRun = true;
    struct_message myData;

    esp_now_peer_info_t peerInfo;
    esp_now_peer_info_t peerInfo2;


    void _h(HttpEvent_t *event);//empty method for handling httpevents if needed for debug purposes
    void triggerUpdate();
    
    void sendBroadcast(String msg);
    void sendSingle(uint8_t mac[6], String msg);

    void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);
    void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len);
    
    void meshSetup(RCFunctionPointer rcPtrFunc);
    void otaStuff();