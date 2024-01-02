#include <RPLidar.h>
#include "randomForest10.h"
#define RPLIDAR_MOTOR 9
RPLidar lidar;

#define LIDAR_RESOLUTION 240

#define DIR_OUT1 6
#define DIR_OUT2 7

#define MOT1_A 7
#define MOT1_B 8
#define MOT2_A 6
#define MOT2_B 5

Eloquent::ML::Port::RandomForest clf;
int lidarDataSelection[80] = {136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 176, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223};

float distances[LIDAR_RESOLUTION];

void setup() {
  Serial.begin(9600);
  Serial1.begin(115200);  // For RPLidar
  lidar.begin(Serial1);
  pinMode(RPLIDAR_MOTOR, OUTPUT);  // Set pin modes

  pinMode(DIR_OUT1, OUTPUT);
  pinMode(DIR_OUT2, OUTPUT);
  pinMode(MOT1_A, OUTPUT);
  pinMode(MOT1_B, OUTPUT);
  pinMode(MOT2_A, OUTPUT);
  pinMode(MOT2_B, OUTPUT);

  //setting all distance in the array to 0
  for (int a = 0; a < LIDAR_RESOLUTION; a++) {
    distances[a] = 0;
  }
    if (IS_OK(lidar.waitPoint())) {

  } else {
    analogWrite(RPLIDAR_MOTOR, 0);  // Stop the RPLIDAR motor
    // Try to detect RPLIDAR
    Serial.println("Checking lidar");
    rplidar_response_device_info_t info;
    if (IS_OK(lidar.getDeviceInfo(info, 100))) {
      // Detected
      Serial.println("LIDAR OK");
      lidar.startScan();
      analogWrite(RPLIDAR_MOTOR, 255);
      delay(1000);
    }
  }
}

long int last_print = 0;
int ctrl = 0;
void loop() {
  lidar.waitPoint();
    float distance = lidar.getCurrentPoint().distance;
    float angle = lidar.getCurrentPoint().angle;
    float quality = lidar.getCurrentPoint().quality;

    if (distance < 2000 && quality > 0) {
      int index = mapAndRoundAngle(angle);
      if (distance == 0) {
        distance = distances[index - 1];
      }
      distances[index] = distance;
    }
    if(millis()-last_print > 100){
      //printData();
      last_print = millis();
    }
    float selectedData[80];
    for (int i = 0; i < 80; i++) {
        selectedData[i] = distances[lidarDataSelection[i]];
    }
    ctrl = clf.predict(selectedData);
    drive(ctrl);
}
void printData() {
  for(int i = 0; i<LIDAR_RESOLUTION; i++){
    Serial.print(distances[i]);
    Serial.print(",");
  }
  Serial.println(ctrl);
}

int mapAndRoundAngle(float angle) {
  // Subtract 90 degrees to offset the angles clockwise
  angle -= 90;

  // Ensure the angle stays within the valid range (0-360)
  if (angle < 0) {
    angle += 360;
  } else if (angle >= 360) {
    angle -= 360;
  }
  // Map the angle from 0-360 degrees to 0-239 (LIDAR_RESOLUTION)
  int index = int(map(angle, 0, 360, 0, LIDAR_RESOLUTION));

  return index;
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
    setMotor(1, 250);
    setMotor(2, 250);
  }else if(ctrl == 1){
    setMotor(1, 250);
    setMotor(2, 110);
  }else if(ctrl == 2){
    setMotor(1, 110);
    setMotor(2, 250);
  }
}