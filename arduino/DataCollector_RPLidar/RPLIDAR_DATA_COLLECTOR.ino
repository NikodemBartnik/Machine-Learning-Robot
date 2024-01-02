#include <RPLidar.h>
#include <SPI.h>
#include <SD.h>
#include <SoftwareSerial.h>

#define RPLIDAR_MOTOR 9
RPLidar lidar;

#define LIDAR_RESOLUTION 240
#define SD_CS 4

#define DIR_OUT1 6
#define DIR_OUT2 7

#define MOT1_A 7
#define MOT1_B 8
#define MOT2_A 6
#define MOT2_B 5

float distances[LIDAR_RESOLUTION];

SoftwareSerial SoftSerial(2, 3);  // Rx, Tx
String control = "";
bool data_recording = false;

void setup() {

  Serial.begin(115200);
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

  if (!SD.begin(SD_CS)) {
    Serial.println("Card failed, or not present");
    while (1)
      ;
  }
  Serial.println("card initialized.");
  if (!SoftSerial.begin(9600, SERIAL_8E2)) {
    Serial.println("Failed to init soft serial");
    //while (1)
      ;
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
      delay(50);
    }
  }
}

long last_send = 0;
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
      Serial.println(distances[0]);
    }
  while (SoftSerial.available()) {
    int data = SoftSerial.read();
    if (data == 70) {
      control = "F";
    } else if (data == 83) {
      control = "S";
    } else if (data == 76) {
      control = "L";
    } else if (data == 82) {
      control = "R";
    } else if (data == 71) {
      control = "G";
    } else if (data == 73) {
      control = "I";
    } else if (data == 66) {
      control = "B";
    } else if (data == 72) {
      control = "H";
    } else if (data == 74) {
      control = "J";
    } else if (data == 87) {
      data_recording = true;
    } else if (data == 119) {
      data_recording = false;
    }
    drive(control);
  }
  if (data_recording && control != "" && control != "B" && control != "S") {
    if ((millis() - last_send) > 200) {
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
      } else {
        Serial.println("error opening datalog.txt");
      }
      last_send = millis();
    }
  }
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


void setMotor(int motor, int value) {
  if (motor == 1) {
    if (value >= 0) {
      analogWrite(MOT1_A, value);
      analogWrite(MOT1_B, 0);
    } else {
      analogWrite(MOT1_A, 0);
      analogWrite(MOT1_B, -value);
    }
  } else if (motor == 2) {
    if (value >= 0) {
      analogWrite(MOT2_A, value);
      analogWrite(MOT2_B, 0);
    } else {
      analogWrite(MOT2_A, 0);
      analogWrite(MOT2_B, -value);
    }
  }
}

void drive(String ctrl) {
  //Serial.print("Control in drive: ");
  //Serial.println(ctrl);
  if (ctrl == "F") {
    setMotor(1, 160);
    setMotor(2, 160);
  } else if (ctrl == "B") {
    setMotor(1, -100);
    setMotor(2, -100);
  } else if (ctrl == "L") {
    setMotor(1, 80);
    setMotor(2, -80);
  } else if (ctrl == "R") {
    setMotor(1, -80);
    setMotor(2, 80);
  } else if (ctrl == "S") {
    setMotor(1, 0);
    setMotor(2, 0);
  } else if (ctrl == "G") {
    setMotor(1, 160);
    setMotor(2, 70);
  } else if (ctrl == "I") {
    setMotor(1, 70);
    setMotor(2, 160);
  }
}