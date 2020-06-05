#include <HID-Project.h>
#include <HID-Settings.h>

void setup() {
  NSGamepad.begin();
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);
  NSGamepad.press(NSButton_A);
  NSGamepad.write();

  delay(1000);

  digitalWrite(LED_BUILTIN, LOW);
  NSGamepad.release(NSButton_A);
  NSGamepad.write();
  
  delay(1000);
}
