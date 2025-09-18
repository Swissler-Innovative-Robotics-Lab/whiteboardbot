/*////////////////////////////////////////////////////
 Name:		simple_helper_functions.h
 Created:	3/25/2019
 Author:	Petras Swissler

 Purpose:	Define simple helper functions

 Limitations: Assumes 8 bits per byte
*/////////////////////////////////////////////////////
// Boilerplate ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include "HttpsOTAUpdate.h"
#include <Preferences.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include <esp_now.h>

#ifndef _SIMPLE_HELPER_FUNCTIONS_h
#define _SIMPLE_HELPER_FUNCTIONS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

// Include Simple Definitions ~~~~~~~~~~~~~~~~~~~~~~~~
#ifdef __DEF_COMPUTER_G7__
	#include "C:\Users\Petra\OneDrive\NorthWestern\Lab\FA3D_2\Code\RobotCode\shared_definitions\def_friendly_names.h"
#endif // __DEF_COMPUTER_G7__
#ifdef __DEF_COMPUTER_GIGABYTE__
	#include "D:\petras\Onedrive\OneDrive\Onedrive\NorthWestern\Lab\FA3D_2\Code\RobotCode\shared_definitions\def_friendly_names.h"
#endif
// This library is intended to stand alone
// Define several useful terms ~~~~~~~~~~~~~~~~~~~~~~~
#define MS_PER_S			1000
#define BYTES_PER_FLOAT		sizeof(float)
#define BITS_PER_BYTE		8

// Color-sensor thresholds
#define THRESHOLD_WHITE 5700
#define THRESHOLD_BLACK  100

// Hue cutoffs 
#define RED_MIN_0    0.00f
#define YELLOW_MIN   0.125f
#define GREEN_MIN    0.25f
#define CYAN_MIN     0.35f
#define BLUE_MIN     0.53f
#define MAGENTA_MIN  0.70f
#define RED_MIN_1    0.85f

// Color codes
#define COLOR_BLACK    0
#define COLOR_WHITE    1
#define COLOR_RED      2
#define COLOR_YELLOW   3
#define COLOR_GREEN    4
#define COLOR_CYAN     5
#define COLOR_BLUE     6
#define COLOR_MAGENTA  7

// Variable for correction()
// float dz = 5.0;
// float ma = 180.0;

// Low-Level Functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// None: All functions intended to stand alone
// Main Functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Simple Math Functions
void	normalizedNumber0to1(float* x, float* x_max, float* x_min);
void normalizedNumberNeg1to1(float* x, float* x_max, float* x_min);
void	adjustMaxMin(float* measurement, float* max_meas, float* min_meas);
float	bound(float input, float minimum, float maximum);
float	getFloatSign(float);
int		getIntSign(int);
float	getMidof3(float, float, float);
float	getSlope(float x1, float y1, float x2, float y2);
float	getYIntercept(float x1, float y1, float slope);
int		isqrt(int);
uint8_t	getAbsentByte(uint8_t A, uint8_t B, uint8_t option1, uint8_t option2, uint8_t option3);
float	wrapToPi(float);
int		getNthPrime(int);

// Vector and Matrix Math Functions
void	vectorAdd_f(float* target_vec3, float vx, float vy, float vz, float scale);
float getVectorMagnitude(float x, float y, float z);
float vectorDotProduct(float* x0, float* y0, float* z0, float* x1, float* y1, float* z1);
void normalizeVector(float* x, float* y, float* z);
float getAngleBetweenVectors(float* x0, float* y0, float* z0, float* x1, float* y1, float* z1);

// Timing Functions
bool	checkTime(unsigned long startTime, unsigned long nowTime, float targetDuration_seconds);

// Measurement Functions
float	getAvgReading(int, uint8_t);
int		getAvgReading_int(int pin, uint8_t numMeas);
int		getMaxofN(int pin, uint8_t numMeas);
int		getAvgReading_int_notHiOrLo(int pin, uint8_t numMeas);

// Filtering functions
float	susdFilter(float meas, float last, float stepsize);
float	expFilter(float newVal, float oldVal, float alpha);

// Value and type manipulation
void	floatToFourChars(float, uint8_t*);
float	fourCharsToFloat(uint8_t*);
void	int16ToTwoChars(uint16_t inInt, uint8_t* outChar);
uint16_t twoCharsToInt16(uint8_t* inChar);
uint8_t s2u_int8(int8_t inbyte);
int8_t	u2s_int8(uint8_t inbyte);
uint8_t floatToEncodedByte(float inValue, float conversionFactor);
float	encodedByteToFloat(uint8_t inValue, float conversionFactor);

// Serial Functions
char waitForKeypress(void);
void printArray(uint8_t* msgArray, uint8_t numBytes, bool use_send);

// Memory Management Functions
int getFreeMemory(void);
void clearArray(uint8_t* msgArray, uint8_t len);

// Color stuff
float calchue(float R, float G, float B);
float intensity(float R, float G, float B);
uint8_t determineColor(float hue, float brightness, uint16_t thresh_white, uint16_t thresh_black);

// Movement
float orientation(float x, float y);
void correction(float d_theta, float c_theta, float &drive_forward, float &drive_spin);

// Delay
bool color_count(uint8_t measured_color, uint8_t &color_for_logic, uint32_t &count_of_color);

// Boilerplate ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#endif


//networking helpers


//used for OTA Updates, can use LAN router or PC hotspot
#define Wifi_SSID "DESKTOP-GLO6DFB 4172"
#define Wifi_PASSWORD "password"

//use by commenting out currnent device address (one being uploaded too
// and having 2 remaining addresses as Address and address2
uint8_t broadcastAddress0[] = {0xf4,0x12,0xfa,0x97,0xe0,0x68};//black 
uint8_t broadcastAddress[] = {0x70,0x04,0x1d,0xcd,0xda,0x68};//white 
uint8_t broadcastAddress2[] = {0xf4,0x12,0xfa,0x97,0xdd,0xc0};//yellow

//theoretically can be converted to this but had issues uppon testing
uint8_t broadcastAddresses[][] = {broadcastAddress0,broadcastAddress, broadcastAddress2}
//then u can do curr_address = smth and then when these addresses are used (sendbroadcast) you can loop and then have condition if it isnt curr mac send msg 

void readMacAddress()

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