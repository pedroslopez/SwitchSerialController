#include <HID-Project.h>
#include <HID-Settings.h>

const byte numChars = 32;
char receivedChars[numChars];
char tempChars[numChars];

char buttonData[numChars] = {0};
uint16_t pressedButtons = 0;
uint8_t leftStickX = 0;
uint8_t leftStickY = 0;
uint8_t rightStickX = 0;
uint8_t rightStickY = 0;

boolean newData = false;

void setup() {
  NSGamepad.begin();
  
  Serial1.begin(115200);
  Serial1.println("<Arduino is ready>");
}

void loop() {
  receiveData();
  if (newData == true) {
    strcpy(tempChars, receivedChars);
    parseData();
    sendControllerData();
    newData = false;
  }  

//  NSGamepad.loop();
}

void receiveData() {
  static boolean recvInProgress = false;
  static byte ndx = 0;
  char startMarker = '<';
  char endMarker = '>';
  char rc;

  while (Serial1.available() > 0 && newData == false) {
    rc = Serial1.read();

    if (recvInProgress) {
      if (rc != endMarker) {
        receivedChars[ndx] = rc;
        ndx++;
        if (ndx >= numChars) {
          ndx = numChars - 1;
        }
      }
      else {
        receivedChars[ndx] = '\0'; // terminate the string
        recvInProgress = false;
        ndx = 0;
        newData = true;
      }
    }

    else if (rc == startMarker) {
      recvInProgress = true;
    }
  }
}

void parseData() {
  char * strtokIndx; // this is used by strtok() as an index

  // get buttons
  strtokIndx = strtok(tempChars,",");      
  pressedButtons = atoi(strtokIndx);

  // get left stick
  strtokIndx = strtok(NULL, ",");
  leftStickX = atoi(strtokIndx);
  
  strtokIndx = strtok(NULL, ",");
  leftStickY = atoi(strtokIndx);
  
  // get right stick
  strtokIndx = strtok(NULL, ",");
  rightStickX = atoi(strtokIndx);
  
  strtokIndx = strtok(NULL, ",");
  rightStickY = atoi(strtokIndx);
}

void sendControllerData() {
  // Set sticks
  NSGamepad.leftXAxis(leftStickX);
  NSGamepad.leftYAxis(leftStickY);
  NSGamepad.rightXAxis(rightStickX);
  NSGamepad.rightYAxis(rightStickY);

  // Press/Release buttons
  NSGamepad.buttons(pressedButtons);

  // Send report
  NSGamepad.write();
  
  // Serial reply
  Serial1.println("OK");
//  Serial1.print("BTN ");
//  Serial1.print(pressedButtons, BIN);
//
//  Serial1.print(" LX ");
//  Serial1.print(leftStickX, BIN);
//
//  Serial1.print(" LY ");
//  Serial1.print(leftStickY, BIN);
//
//  Serial1.print(" RX ");
//  Serial1.print(rightStickX, BIN);
//
//  Serial1.print(" RY ");
//  Serial1.println(rightStickY, BIN);

}
