#include <SPI.h>
#include <SD.h>
#include <SoftwareSerial.h>

#define LIDAR_RESOLUTION 240
#define FRAME_LENGTH 120

#define DIR_OUT1 6
#define DIR_OUT2 7

#define SD_CS 4

#define MOT1_A 7
#define MOT1_B 8
#define MOT2_A 6
#define MOT2_B 5


SoftwareSerial SoftSerial(2, 3); // Rx, Tx
String control = "";
bool data_recording = false;

//### LIDAR
int lidar_frame[FRAME_LENGTH];
int distances[LIDAR_RESOLUTION];
int signals[LIDAR_RESOLUTION];

void setup() {
  pinMode(DIR_OUT1, OUTPUT);
  pinMode(DIR_OUT2, OUTPUT);
  pinMode(MOT1_A, OUTPUT);
  pinMode(MOT1_B, OUTPUT);
  pinMode(MOT2_A, OUTPUT);
  pinMode(MOT2_B, OUTPUT);

  //Serial monitor
  Serial.begin(230400);
  Serial1.begin(230400);

  if (!SD.begin(SD_CS)) {
    Serial.println("Card failed, or not present");
    while (1);
  }
  Serial.println("card initialized.");

  //setting all distance in the array to 0
  for (int a = 0; a < LIDAR_RESOLUTION; a++) {
    distances[a] = 0;
    signals[a] = 50;
  }

  if (!SoftSerial.begin(9600, SERIAL_8E2)) {
        Serial.println("Failed to init soft serial");
        while (1);
    }
}

long last_send = 0;
void loop() {
    if (Serial1.read() == 170) {
      GetLidarData();
      if(lidar_frame[2] == 1 && lidar_frame[3] == 97){
      int circleSegment = ((lidar_frame[10]*256+lidar_frame[11])/2250);
      int points = (lidar_frame[6] - 5) / 3;
      if(points > 30 && circleSegment < 16){
        for(int p = 0; p < 15; p++){
            int d = (lidar_frame[13 + p*6]*256 + lidar_frame[14 + p*6]) * 0.25;
            int signal = (lidar_frame[12+p*6]);
            if(d < 8000 && signal >= 100){           
              signals[circleSegment*15+p] = signal;
              distances[circleSegment*15+p] = d;
            }
          }
        }
      }
    }
   
    while(SoftSerial.available()) {
      int data = SoftSerial.read();
        if(data == 70){
          control = "F";
        }else if(data == 83){
          control = "S";
        }else if(data == 76){
          control = "L";
        }else if(data == 82){
          control = "R";
        }else if(data == 71){
          control = "G";
        }else if(data == 73){
          control = "I";
        }else if(data == 66){
          control = "B";
        }else if(data == 72){
          control = "H";
        }else if(data == 74){
          control = "J";
        }else if(data == 87){
          data_recording = true;
        }else if(data == 119){
          data_recording = false;
        }
        drive(control);
    }
      if(data_recording && control != "" && control != "B" && control != "S"){
        if((millis() - last_send) > 200){
          String toPython = "";
          for (int a = 0; a < LIDAR_RESOLUTION; a++) {
            toPython.concat(distances[a]);
            toPython.concat(",");
            //toPython.concat(signals[a]);
            //toPython.concat(",");
          }
          toPython.concat(control);
          File dataFile = SD.open("datalog.txt", FILE_WRITE);

          if (dataFile) {
            dataFile.println(toPython);
            dataFile.close();
            Serial.println(toPython);
          }else {
            Serial.println("error opening datalog.txt");
          }
          last_send = millis();
        }
      }
}


void GetLidarData() {
  for (int a = 0; a < FRAME_LENGTH; a++) {
    while (!Serial1.available());
    lidar_frame[a] = Serial1.read();
  }
}

void setMotor(int motor, int value){
if(motor == 1){
  if(value >= 0){
    analogWrite(MOT1_A, value);
    analogWrite(MOT1_B, 0);
  }else{
    analogWrite(MOT1_A, 0);
    analogWrite(MOT1_B, -value);
  }
}else if(motor == 2){
  if(value >= 0){
    analogWrite(MOT2_A, value);
    analogWrite(MOT2_B, 0);
  }else{
    analogWrite(MOT2_A, 0);
    analogWrite(MOT2_B, -value);
  }
}
}

void drive(String ctrl){
  Serial.print("Control in drive: ");
  Serial.println(ctrl);
  if(ctrl == "F"){
    setMotor(1, 255);
    setMotor(2, 255);
  }else if(ctrl == "B"){
    setMotor(1, -110);
    setMotor(2, -110);
  }else if(ctrl == "L"){
    setMotor(1, 90);
    setMotor(2, -90);
  }else if(ctrl == "R"){
    setMotor(1, -90);
    setMotor(2, 90);
  }else if(ctrl == "S"){
    setMotor(1, 0);
    setMotor(2, 0);
  }else if(ctrl == "G"){
    setMotor(1, 255);
    setMotor(2, 100);
  }else if(ctrl == "I"){
    setMotor(1, 100);
    setMotor(2, 255);
  }
}