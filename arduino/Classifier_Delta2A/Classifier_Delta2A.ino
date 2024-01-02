#include <SPI.h>
#include <SD.h>
#include "randomForest3.h"

//Max lidar resolution is 752, to make it easier to send and display in pygame program reduces it to 188 (752/4 = 188)
#define LIDAR_RESOLUTION 240
#define FRAME_LENGTH 120

#define DIR_OUT1 6
#define DIR_OUT2 7

#define SD_CS 4

#define MOT1_A 7
#define MOT1_B 8
#define MOT2_A 6
#define MOT2_B 5


Eloquent::ML::Port::RandomForest clf;
int lidarDataSelection[15] = {147, 148, 149, 150, 151, 152, 206, 207, 208, 209, 210, 211, 212, 213, 214};
//int lidarDataSelection[10] = {144, 145, 146, 147, 148, 149, 150, 151, 208, 209};

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


  //setting all distance in the array to 0
  for (int a = 0; a < LIDAR_RESOLUTION; a++) {
    distances[a] = 0;
    signals[a] = 50;
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
  float selectedData[15] = {distances[lidarDataSelection[0]], distances[lidarDataSelection[1]], distances[lidarDataSelection[2]], distances[lidarDataSelection[3]], distances[lidarDataSelection[4]], distances[lidarDataSelection[5]], distances[lidarDataSelection[6]], distances[lidarDataSelection[7]], distances[lidarDataSelection[8]], distances[lidarDataSelection[9]], distances[lidarDataSelection[10]], distances[lidarDataSelection[11]], distances[lidarDataSelection[12]], distances[lidarDataSelection[13]], distances[lidarDataSelection[14]]};
  int ctrl = clf.predict(selectedData);
  //Serial.println(distances[0]);
  drive(ctrl);
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

void drive(int ctrl){
  if(ctrl == 0){
    setMotor(1, 170);
    setMotor(2, 150);
  }else if(ctrl == 1){
    setMotor(1, 170);
    setMotor(2, 100);
  }else if(ctrl == 2){
    setMotor(1, 110);
    setMotor(2, 150);
  }
}