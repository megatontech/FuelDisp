#include <SoftwareSerial.h>
#include <TimeLib.h>

#define TIME_MSG_LEN 11  // time sync to PC is HEADER followed by unix time_t as ten ascii digits
#define TIME_HEADER 'T'  // Header tag for serial time sync message

//8 start low->high
const int INC_pin = 11;
//9 direction low - high +
const int UD_pin = 12;
//10 set low to use chip
const int CS_pin = 13;
const int step = 50;
int count = 0;
void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(INC_pin, OUTPUT);
  pinMode(UD_pin, OUTPUT);
  pinMode(CS_pin, OUTPUT);
}

void minval() {
  digitalWrite(UD_pin, LOW);
  int i = 0;
  for (i = 120; i > 0; i--) {
    digitalWrite(INC_pin, LOW);
    delayMicroseconds(2);
    digitalWrite(INC_pin, HIGH);
    delay(10);
  }
}
void maxval() {
  int i = 0;
  digitalWrite(UD_pin, LOW);
  for (i = 120; i > 0; i--) {
    digitalWrite(INC_pin, LOW);
    delayMicroseconds(2);
    digitalWrite(INC_pin, HIGH);
    delay(10);
  }
  digitalWrite(UD_pin, HIGH);
  for (i = 50; i > 0; i--) {
    digitalWrite(INC_pin, LOW);
    delayMicroseconds(2);
    digitalWrite(INC_pin, HIGH);
    delay(10);
  }
}
void zeroval() {
  digitalWrite(UD_pin, LOW);
  int i = 0;
  for (i = 120; i > 0; i--) {
    digitalWrite(INC_pin, LOW);
    delayMicroseconds(2);
    digitalWrite(INC_pin, HIGH);
    delay(100);
  }
}
void upval() {
  digitalWrite(UD_pin, HIGH);
  int i = 0;
  for (i = 1; i > 0; i--) {
    digitalWrite(INC_pin, LOW);
    delayMicroseconds(2);
    digitalWrite(INC_pin, HIGH);
    delay(100);
  }
}
void downval() {
  digitalWrite(UD_pin, LOW);
  int i = 0;
  for (i = 1; i > 0; i--) {
    digitalWrite(INC_pin, LOW);
    delayMicroseconds(2);
    digitalWrite(INC_pin, HIGH);
    delay(100);
  }
}
void digitalClockDisplay() {
  // digital clock display of the time
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.print(" ");
  Serial.print(day());
  Serial.print(" ");
  Serial.print(month());
  Serial.print(" ");
  Serial.print(year());
  Serial.println();
}
void loop() {
  if (Serial.available()) {
    processSyncMessage();
  }
  digitalClockDisplay();
  delay(1000);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(50);
  digitalWrite(LED_BUILTIN, LOW);
  delay(50);
  digitalWrite(CS_pin, LOW);

  int currhour = hour();
  int currminute = minute();
  bool up = false;
  bool down = false;
  bool min = false;
  bool max = false;
  bool zero = false;

  int remainhour = 18 - currhour - 1;
  int remainminute = 60 - currminute;
  int remaincycle = ((remainhour * 60) + remainminute) / 10;

  //offline
  if (remaincycle < 50 && count == 0 && ((remainhour * 60 + remainminute) < 500)) {
    //clear all
    zeroval();
    //step to remain val
    int j = 0;
    for (j = 50 - remaincycle; j > 0; j--) {
      upval();
      count++;
    }

  } else {
    //normal step starts at 09.25
    if (((remainhour * 60 + remainminute) < 515) && (count <= 50) && (remaincycle >= 0) && (50 - count) >= remaincycle) {
      up = true;
      count++;
    }
  }
  if (count > 50) {
    //overtime work
    zero = true;
    up = false;
  }
  if (currhour < 9) {
    //keep zero before 9am
    zero = true;
    up = false;
  }
  if (remainhour <= 0 && remainminute <= 15) {
    //zero when time used up
    zero = true;
    up = false;
  }
  if (remaincycle <= 0) {
    //zero when time used up
    zero = true;
    up = false;
  }

  //+10
  if (up) {
    upval();
  }
  //-10
  if (down) {
    downval();
  }
  if (min) {
    minval();
  }
  //set to 270
  if (max) {
    maxval();
  }
  //morning time set val to 38.5
  if (zero) {
    zeroval();
  }
}

void printDigits(int digits) {
  // utility function for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if (digits < 10)
    Serial.print('0');
  Serial.print(digits);
}
void processSyncMessage() {
  unsigned long pctime;
  const unsigned long DEFAULT_TIME = 1678266000;
  pctime = Serial.parseInt();
  if (pctime >= DEFAULT_TIME) {  // check the integer is a valid time (greater than Jan 1 2013)
    setTime(pctime);             // Sync Arduino clock to the time received on the serial port
  }
}
