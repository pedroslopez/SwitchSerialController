#include <HID-Project.h>
#include <HID-Settings.h>

#define STICK_MIN      0
#define STICK_CENTER 128
#define STICK_MAX    255

const byte numChars = 32;
char receivedChars[numChars];
char tempChars[numChars];

char buttonData[numChars] = {0};
uint8_t leftStickX = 0;
uint8_t leftStickY = 0;
uint8_t rightStickX = 0;
uint8_t rightStickY = 0;

boolean newData = false;

void setup() {
  NSGamepad.begin();
  
  Serial1.begin(9600);
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

  NSGamepad.loop();
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
  strcpy(buttonData, strtokIndx); 

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
  Serial1.print("Buttons ");
  Serial1.println(buttonData);
  Serial1.print("Left Stick ");
  Serial1.println(leftStickX);
  Serial1.println(leftStickY);
  Serial1.print("Right Stick ");
  Serial1.println(rightStickX);
  Serial1.println(rightStickY);

  // Set sticks
  NSGamepad.leftXAxis(leftStickX);
  NSGamepad.leftYAxis(leftStickY);
  NSGamepad.rightXAxis(rightStickX);
  NSGamepad.rightYAxis(rightStickY);

  // Press/Release buttons
  for(int i=0; buttonData[i] != 0; i++) {
    switch(buttonData[i]) {
      case 'a':
        NSGamepad.press(NSButton_A);
        break;
      case 'A':
        NSGamepad.release(NSButton_A);
        break;
      case 'b':
        NSGamepad.press(NSButton_B);
        break;
      case 'B':
        NSGamepad.release(NSButton_B);
        break;
      case 'y':
        NSGamepad.press(NSButton_Y);
        break;
      case 'Y':
        NSGamepad.release(NSButton_Y);
        break;
      case 'x':
        NSGamepad.press(NSButton_X);
        break;
      case 'X':
        NSGamepad.release(NSButton_X);
        break;
      case 'o':
        NSGamepad.press(NSButton_LeftTrigger);
        break;
      case 'O':
        NSGamepad.release(NSButton_LeftTrigger);
        break;
      case 'p':
        NSGamepad.press(NSButton_RightTrigger);
        break;
      case 'P':
        NSGamepad.release(NSButton_RightTrigger);
        break;
      case 'j':
        NSGamepad.press(NSButton_LeftThrottle);
        break;
      case 'J':
        NSGamepad.release(NSButton_LeftThrottle);
        break;
      case 'k':
        NSGamepad.press(NSButton_RightThrottle);
        break;
      case 'K':
        NSGamepad.release(NSButton_RightThrottle);
        break;
      case '-':
        NSGamepad.press(NSButton_Minus);
        break;
      case '_':
        NSGamepad.release(NSButton_Minus);
        break;
      case '+':
        NSGamepad.press(NSButton_Plus);
        break;
      case '=':
        NSGamepad.release(NSButton_Plus);
        break;
      case 'l':
        NSGamepad.press(NSButton_LeftStick);
        break;
      case 'L':
        NSGamepad.release(NSButton_LeftStick);
        break;
      case 'r':
        NSGamepad.press(NSButton_RightStick);
        break;
      case 'R':
        NSGamepad.release(NSButton_RightStick);
        break;
      case 'h':
        NSGamepad.press(NSButton_Home);
        break;
      case 'H':
        NSGamepad.release(NSButton_Home);
        break;
      case 'c':
        NSGamepad.press(NSButton_Capture);
        break;
      case 'C':
        NSGamepad.release(NSButton_Capture);
        break;
    }
  } 
}
