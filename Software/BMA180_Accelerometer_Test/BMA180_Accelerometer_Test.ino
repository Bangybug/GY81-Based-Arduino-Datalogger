//BMA180 triple axis accelerometer sample code//
//www.geeetech.com//
//
#include <Wire.h> 
#define BMA180_ADDRESS 0x40  //address of the accelerometer
#define BMA180_RESET 0x10   
#define BMA180_PWR 0x0D
#define BMA180_BW 0X20
#define BMA180_RANGE 0X35
#define BMA180_DATA 0x02
//
int offx = 31;  
int offy = 47;   
int offz = -23; 
// 
byte resolution = 0x08;
float scale = 2048.0;
//

void setup() { 
 Serial.begin(9600); 
 Wire.begin(); 
 Serial.println("Demo started, initializing sensors"); 
 
 AccelerometerInit(16); 
 Serial.println("Sensors have been initialized"); 
} 


void AccelerometerInit(int res){ 
  byte temp[1];
  byte temp1;

  setResolution(res);
  
  writeTo(BMA180_ADDRESS,BMA180_RESET,0xB6);
  //wake up mode
  writeTo(BMA180_ADDRESS,BMA180_PWR,0x10);
  // low pass filter,
  readFrom(BMA180_ADDRESS, BMA180_BW,1,temp);
  temp1=temp[0]&0x0F;
  writeTo(BMA180_ADDRESS, BMA180_BW, temp1);   
  // 0x08 is range +/- 4g 
  readFrom(BMA180_ADDRESS, BMA180_RANGE, 1 ,temp);  
  temp1=(temp[0]&0xF1) | resolution;
  writeTo(BMA180_ADDRESS,BMA180_RANGE,temp1);
}

void AccelerometerRead(){ 
 // read in the 3 axis data, each one is 14 bits 
 // print the data to terminal 
 int n=6;
 byte result[5];
 readFrom(BMA180_ADDRESS, BMA180_DATA, n , result);

Serial.println();
  for (int i=0; i<6; i++){
    Serial.println(result[i]);
  }
  Serial.println();
 
 int x= (( result[0] | result[1]<<8)>>2)+offx ;
 //float x1=x/4096.0;
 float x1=x/scale;
 Serial.print("x=");
 Serial.print(x1);
 Serial.print("g"); 
 //
 int y= (( result[2] | result[3]<<8 )>>2)+offy;
 //float y1=y/4096.0;
 float y1=y/scale;
 Serial.print(",y=");
 Serial.print(y1);
 Serial.print("g"); 
 //
 int z= (( result[4] | result[5]<<8 )>>2)+offz;
 //float z1=z/4096.0;
 float z1=z/scale;
 Serial.print(",z=");
 Serial.print(z1);
 Serial.println("g"); 
}

void setResolution(int gValue){
  switch (gValue){
    case 1:
      resolution = 0x00;
      scale = 4096.0 * 2;
      break;
    case 2:
      resolution = 0x04;
      scale = 4096.0;
      break; 
    case 3:
      resolution = 0x06;
      scale = 4096.0*2.0/3.0;
      break; 
    case 4:
      resolution = 0x08;
      scale = 4096.0/2.0;
      break;
    case 8:
      resolution = 0x0A;
      scale = 4096.0/4;
      break;
    case 16:
      resolution = 0x0C;
      scale = 4096.0/8;
      break;
    default:
      resolution = 0x04;
      scale = 4096.0;
      break;
  }
}

void loop() 
{ 
 AccelerometerRead(); 
 delay(300); // slow down output   
}
//
//---------------- Functions--------------------
//Writes val to address register on ACC
void writeTo(int DEVICE, byte address, byte val) {
  Wire.beginTransmission(DEVICE);   //start transmission to ACC
  Wire.write(address);               //send register address
  Wire.write(val);                   //send value to write
  Wire.endTransmission();           //end trnsmisson
}

//reads num bytes starting from address register in to buff array
 void readFrom(int DEVICE, byte address , int num ,byte buff[]){
 Wire.beginTransmission(DEVICE); //start transmission to ACC
 Wire.write(address);            //send reguster address
 Wire.endTransmission();        //end transmission
 
 Wire.beginTransmission(DEVICE); //start transmission to ACC
 Wire.requestFrom(DEVICE,num);  //request 6 bits from ACC
 
 int i=0;
 while(Wire.available()) {        //ACC may abnormal
   buff[i] =Wire.read();        //receive a byte
   i++;
 }
 Wire.endTransmission();         //end transmission
}
