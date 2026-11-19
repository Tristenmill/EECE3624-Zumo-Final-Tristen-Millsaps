/*
  Simple character read.  This is looking for an "H" or and "L"
  and will set the LED tied to the given pin on or off.

  "SoftwareSerial" is a library that does a bit-bang USART interface on 
  any two digital GPIOs on an arduino.  This allows you to leave the 
  primary USART for debugging.

  This routine setups a USART on pins 2 and 4 named "bluetooth".  It then
  setups up this uart at 9600 baud.  In the loop, it checks to see if a
  character has arrived, and then reads it.

  The LED13 (built in LED) is blinked to show the program is running.  This
  blink can be moved inside the bluetooth if statement for debugging, or
  the normal serial pipe can be used.
*/
#include <Wire.h>
#include <ZumoShield.h>
#include <SoftwareSerial.h>

//receiver
int led = 13; 
#define NUM_SENSORS 6
unsigned int sensor_values[NUM_SENSORS];
unsigned char incomingByte;     

#define QTR_THRESHOLD  1500 // microseconds

// these might need to be tuned for different motor types

int SPEED = 200;
int T_SPEED = 150;

ZumoMotors motors;

// HC-05
SoftwareSerial bluetooth(2,4);  // RX, TX

ZumoReflectanceSensorArray sensors(QTR_NO_EMITTER_PIN);

void setup() {
  Serial.begin(9600);
  Serial.println("Serial setup complete.");
  pinMode(led, OUTPUT);
  // Setup the HC-05 bluetooth device
  // (HC)=(Uno).  VCC=5V, GND=GND, TX=RX(pin2),
  // RX=DNC! (possible damage if 5V Uno TX drives to HC-05)
  bluetooth.begin(9600);
}

void loop() {
  //digitalWrite(led,HIGH);

  //digitalWrite(led,LOW);

  if (bluetooth.available()) {  
    incomingByte = (unsigned char) bluetooth.read();
    Serial.write(incomingByte);
  }

  if(incomingByte == 'W'){
      motors.setSpeeds(SPEED, SPEED);     
    }
    else if(incomingByte == 'D'){
      motors.setSpeeds(T_SPEED, -T_SPEED);
    }
    else if(incomingByte == 'A'){
      motors.setSpeeds(-T_SPEED, T_SPEED);
    }
    else if(incomingByte == 'S'){
      motors.setSpeeds(-SPEED, -SPEED);
    }
    else if(incomingByte == 'X'){
      motors.setSpeeds(0, 0);
    }
    else if(incomingByte == 'Z'){
      motors.setSpeeds(250, 250);
      incomingByte = 'L';
      while(incomingByte != 'Z'){
        sensors.read(sensor_values);
        if (bluetooth.available()) {  
         incomingByte = (unsigned char) bluetooth.read();
         Serial.write(incomingByte);
        }
      else if((sensor_values[0] > QTR_THRESHOLD)&&(sensor_values[5] > QTR_THRESHOLD)){
        while(incomingByte != 'W'){
        motors.setSpeeds(300, -300);
        incomingByte = (unsigned char) bluetooth.read();
        }
      }
      else if((sensor_values[0] > QTR_THRESHOLD)){
       
        motors.setSpeeds(200, -200);
      }
      else if(sensor_values[5] > QTR_THRESHOLD){
        
        motors.setSpeeds(-200, 200);

      }
      else if(incomingByte == 'W'){
        motors.setSpeeds(250, 250);
      }
    }
    incomingByte = (unsigned char) bluetooth.read();
    }

  
}