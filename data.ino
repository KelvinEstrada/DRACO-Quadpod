#include <Wire.h>
#include <Arduino.h>
#include <FNQR.h>
#define SLAVE_ADDRESS 0x04
#include <Servo.h>

Servo myServo;

FNQR robot;
int flag_int_to_send_to_PI = 0;
int flag_int_received_from_PI = 0;
char data_to_send_PI[] = "hello PI";
String data_recieved_from_pi = "";
boolean receivedW = false;
boolean receivedD = false;
boolean receivedS = false;
boolean receivedA = false;
boolean receivedE = false;
boolean receivedC = false; //Claw command
const int clawServo = 28;

void setup() {
  // put your setup code here, to run once:
     Wire.begin(SLAVE_ADDRESS);
     Wire.onRequest(sendData);

     Serial.begin(9600);
     flag_int_to_send_to_PI = 1;
     robot.Start();
     clawServo.attach(28);
}

void loop() {
  // put your main code here, to run repeatedly:
  robot.Update();
  Wire.onReceive(receiveData);
   if(receivedW){
    robot.CrawlForward();
    receivedW = false;
   }
   else if(receivedD){
    robot.TurnRight();
    receivedD = false;
   }
   else if(receivedS){
    robot.CrawlBackward();
    receivedS = false;
   }
   else if(receivedA){
      robot.TurnLeft();
      receivedA = false;
   }
   else if(receivedE){
      robot.MoveBody(0, 30, 0);
      robot.MoveBody(-30, 0, 0);
      robot.MoveBody(0, -30, 0);
      robot.MoveBody(30, 0, 0);
      robot.MoveBody(0, 0, 0);
      robot.MoveBody(0, 0, 45);
      robot.MoveBody(0, 0, 0);
      delay(1000);
      robot.RotateBody(15, 0, 0);
      robot.RotateBody(0, 15, 0);
      robot.RotateBody(-15, 0, 0);
      robot.RotateBody(0, -15, 0);
      robot.RotateBody(0, 0, 0);
      robot.RotateBody(0, 0, 15);
      robot.RotateBody(0, 0, -15);
      robot.RotateBody(0, 0, 0);
      receivedE = false;
    }
    else if(receivedA){
      grab();
      receivedC = false;
    }
}

void receiveData(int byteCount) {

    while(Wire.available()) {
        flag_int_received_from_PI = Wire.read();

        if(flag_int_received_from_PI == 87) { //ascii code for W
            Serial.println("Crawling forward");
            receivedW = true;
            flag_int_to_send_to_PI = 3;
        }
        else if(flag_int_received_from_PI == 68) { //ascii code for D
            Serial.println("Turningb right");
            receivedD = true;
        }
        else if(flag_int_received_from_PI == 83){ //ascii code for S
          receivedS = true;
        }
        else if(flag_int_received_from_PI == 69){ //ascii code for E
          receivedE = true;
        }
        else if(flag_int_received_from_PI == 65){ //ascii code for A
          receivedA = true;
          }
        else if(flag_int_received_from_PI == 67){ //ascii code for C
          receivedC = true;
          }
        }
    }

void grab(){
  for(int i = 0; i < 46; i+=2){
      clawServo.write(i);
      delay(500);
    }
}

void sendData() {

    if(flag_int_to_send_to_PI == 1) {
        Serial.println("Conversation begin : sending 1 to PI");

        Wire.write(flag_int_to_send_to_PI);
    }

    if(flag_int_to_send_to_PI == 3) {
        Wire.write(data_to_send_PI);
    }

    flag_int_to_send_to_PI = 0;
}
