/*
 * Simulate a mouse moving randomly one pixel every once in a while.
 * Also read texts from three text files on the SD card (BUTTON0.TXT, BUTTON1.TXT, BUTTON2.TXT)
 * and send the texts as keyboard input when the corresponding button is pressed. Or move the mouse
 * in a circle. Or whatever. Just check loop() to see what it does.
 *
 * NOTE: The text sent will be encoded into HID bytes according to an American keyboard layout, so some
 * translation may be needed. Look at the keyboard layouts for a hint of what needs to be translated. It's
 * mostly positional, so whatever's on an American keyboard in the same position will be the correct
 * character.
 *
 * One example is that if you are using a Swedish keyboard layout and want to send a question mark,
 * use underscore.
 *
 */

#include "KeyboardLib.h"
#include "usb_hid_keys.h"
#include <Mouse.h>
#include <SPI.h>
#include <SD.h>
#include "max7219.h"

#define DEBUG
#define DOWRITE
#undef TRISTATESTROBE

// Select the timers you're using, here ITimer4
#define USE_TIMER_1     false
#define USE_TIMER_2     false
#define USE_TIMER_3     false
#define USE_TIMER_4     true
#define USE_TIMER_5     false
#include <TimerInterrupt.h>
#include <ISR_Timer.h>

#define CSPIN PB4
#define CDPIN PB0

unsigned int rowpins[] = {0, 1, 4, 30, 29};
unsigned int colpins[] = {22, 21, 20, 19, 18};

#define NROWS (sizeof(rowpins) / sizeof(unsigned int))
#define NCOLS (sizeof(colpins) / sizeof(unsigned int))

#define HEXDIGIT (1<<8)
#define METAKEY (1<<9)

const unsigned int keycode[NROWS][NCOLS] = {
  {HEXDIGIT | 'C', HEXDIGIT | 'd', HEXDIGIT | 'E', HEXDIGIT | 'F', -1},
  {HEXDIGIT | '8', HEXDIGIT | '9', HEXDIGIT | 'A', HEXDIGIT | 'b', -1},
  {HEXDIGIT | '4', HEXDIGIT | '5', HEXDIGIT | '6', HEXDIGIT | '7', -1},
  {HEXDIGIT | '0', HEXDIGIT | '1', HEXDIGIT | '2', HEXDIGIT | '3', -1},
  {METAKEY | KEY_MOD_LALT, -1, -1, -1, KEY_ENTER}
};

#define BUFLEN 150
#define TXTLEN 32

MAX7219 max7219;

char xlate_string[TXTLEN];

char tmpbuf[BUFLEN];

void trimEOL(char *str) {
  while (strlen(str) > 0 && str[strlen(str) - 1] == '\n') {
    str[strlen(str) - 1] = '\0';
  }
}

#ifdef DOWRITE
int writeSDFile(char *filename, char *buf) {
  File outfile;
  if (SD.exists(filename)) {
    SD.remove(filename);
  }
  if ((outfile = SD.open(filename, FILE_WRITE)) != 0) {
    int res = outfile.print(buf);
#ifdef DEBUG
    Serial.println(String("Wrote ") + buf + " to " + filename + ", " + res + " bytes");
#endif
    outfile.close();
  } else {
#ifdef DEBUG
    Serial.println(String("Failed to open ") + filename + " for writing");
#endif
    return 0;
  }
  return 1;
}
#endif

int readSDFile(char *filename, char *buf, int bufsize) {
  File infile;
  if ((infile = SD.open(filename)) != 0) {
#ifdef DEBUG
    Serial.print("Reading from ");
    Serial.println(filename);
#endif
    int sz = infile.available();
    infile.read(buf, bufsize - 1);
    if (sz < bufsize - 1) {
      buf[sz] = '\0';
    } else {
      buf[bufsize-1] = '\0';
    }
#ifdef DEBUG
    Serial.print("Read: ");
    Serial.println(buf);
#endif
//    trimEOL(buf);
    infile.close();
  } else {
#ifdef DEBUG
    Serial.print("Failed to read file: ");
    Serial.println(filename);
#endif
    buf[0] = '\0';
    return 0;
  }
  return 1;
}

void doxlate(char *str, char *xlate) {
  int i, j;
  for (i=0; i<strlen(xlate); i+=2) {
    for (j=0; j<strlen(str); j++) {
      if (str[j] == xlate[i]) {
        str[j] = xlate[i + 1];
      }
    }
  }
}

void readTxt(char *filename, char *buf, int buflen, char *xlate) {
  if (readSDFile(filename, tmpbuf, sizeof(tmpbuf))) {
      doxlate(tmpbuf, xlate);
      strncpy(buf, tmpbuf, buflen);
      buf[buflen-1] = '\0';
  }
}

void setup() {
  TXLED0;
  //RXLED0;
  pinMode(CSPIN, OUTPUT);
  pinMode(CDPIN, INPUT_PULLUP);

  initKeyboard();
  initHID();

  max7219.Begin();
  max7219.DisplayText("REAdY", 1);
  Mouse.begin();
#ifdef DEBUG
  Serial.begin(115200);
  while (!Serial){
    ;
  }
#endif
  File infile;
  if (SD.begin(CSPIN)) {
    if (readSDFile("xlate.txt", xlate_string, sizeof(xlate_string))) {
      xlate_string[strlen(xlate_string) & 0xFE] = '\0';
    } else {
      xlate_string[0] = '\0';
    }
#ifdef DEBUG
  } else {
    Serial.println("SD initialization failed");
#endif
  }
}

void moveMouse() {
  Mouse.move(random(2) - 1, random(2) - 1);
  delay(200);
}

void moveMouseCircle() {
  int radius=30 + random(250);
  int del=3 + random(7);
  static float pi = 3.14159265;
  float angle;
  // take control of the mouse:
  Mouse.begin();
  //Serial.begin(9600);
  //while (!Serial){};
  int x = radius;
  int y = 0;
  int oldx = x;
  int oldy = y;
  for (angle = 0; angle < 2 * pi; angle += pi/128) {
    x = (int)round(radius * cos(angle));
    y = (int)round(radius * sin(angle));
    //Serial.print(x - oldx);
    //Serial.print(";");
    //Serial.println(y - oldy);
    Mouse.move(x - oldx, y - oldy, 0);
    delay(del);
    oldx = x;
    oldy = y;
  }
  Mouse.end();
}

void moveMouseSpiral() {
  float radius = 30 + random(250);
  float rdelta = (float)random(250)/(float)2500;
  int del=3 + random(7);
  static float pi = 3.14159265;
  float angle;
  // take control of the mouse:
  Mouse.begin();
  //Serial.begin(9600);
  //while (!Serial){};
  int x = radius;
  int y = 0;
  int oldx = x;
  int oldy = y;
  while (radius > 0.1) {
    for (angle = 0; angle < 2 * pi; angle += pi/128) {
      x = (int)round(radius * cos(angle));
      y = (int)round(radius * sin(angle));
      //Serial.print(x - oldx);
      //Serial.print(";");
      //Serial.println(y - oldy);
      Mouse.move(x - oldx, y - oldy, 0);
      // delay(del);
      oldx = x;
      oldy = y;
      radius -= rdelta;
      if (radius <= 0.1) {
        break;
      }
    }
  }
  Mouse.end();
}

void loop() {
  static unsigned long stimeMillis = millis();
  unsigned long nowMillis = millis();
  long diff = nowMillis - stimeMillis;
  if (nowMillis < stimeMillis) {
    diff = nowMillis + (UINT32_MAX - stimeMillis);
  }

  checkForKeyboardChanges();

  if (diff > random(3000) + 2000) {
//    if (random(100) > 60) {
      moveMouse();
//    }
    stimeMillis = nowMillis;
  }
  delay(5);
}

#pragma region "Keyboard scanning code"
#define STABILIZE_STROBE_DELAY 10
#define SCANNER_INTERVAL_MS 1

// Keep track of key state changes, in the easiest way possible
uint8_t keydown[NROWS][NCOLS];
uint8_t oldkeydown[NROWS][NCOLS];

unsigned char hexDigits[9];

unsigned long debounceDelay = 20;    // the debounce time; increase if the output flickers

void initKeyboard() {
  for (int i=0; i<NCOLS; i++) {
    pinMode(colpins[i], INPUT_PULLUP);
  }
  for (int i=0; i<NROWS; i++) {
#ifdef TRISTATESTROBE
    pinMode(rowpins[i], INPUT);
#else
    pinMode(rowpins[i], OUTPUT);
    digitalWrite(rowpins[i], HIGH);
#endif
  }

  memset(keydown, 0, sizeof(keydown));
  memset(oldkeydown, 0, sizeof(oldkeydown));
  memset(hexDigits, ' ', sizeof(hexDigits));
  hexDigits[sizeof(hexDigits) - 1] = '\0';

  // Init timer ITimer4
  ITimer4.init();
  // Interval in unsigned long millisecs
  ITimer4.attachInterruptInterval(SCANNER_INTERVAL_MS, scanKeyboard);
}

void scanKeyboard() {
  // We keep a local debouncing state array here
  static uint8_t debounce[NROWS][NCOLS];

  noInterrupts();
  for (int row = 0; row < NROWS; row++) {
#ifdef TRISTATESTROBE
    pinMode(rowpins[row], OUTPUT);
#endif
    digitalWrite(rowpins[row], LOW);
    // Wait a bit to let it settle
    delayMicroseconds(STABILIZE_STROBE_DELAY);

    for (int col = 0; col < NCOLS; col++) {
      if (digitalRead(colpins[col]) == LOW) {
        switch (debounce[row][col]) {
          // We debounce by just waiting one extra cycle (SCANNER_INTERVAL) and making
          // sure the key is still pressed
          case 0:
            debounce[row][col]++;
            break;
          case 1:
            debounce[row][col]++;
            keydown[row][col] = 1;
            break;
          case 2:
            break;
        }
      } else {
        debounce[row][col] = 0;
        keydown[row][col] = 0;
      }
    }
#ifdef TRISTATESTROBE
    pinMode(rowpins[row], INPUT);
#else
    digitalWrite(rowpins[row], HIGH);
#endif
  }
  interrupts();
}

// Scan through the key state array, compare to the saved state from last run, and
// generate events. Some special cases are handled internally
void checkForKeyboardChanges() {
  static boolean isChanged = false;
  resetKeys();
  for (unsigned int col = 0; col < NCOLS; col++) {
    for (unsigned int row = 0; row < NROWS; row++) {
      if (keydown[row][col] && !oldkeydown[row][col]) {
        // Key Down event
        if (keycode[row][col] != -1) {
          if (keycode[row][col] & HEXDIGIT) {
            memmove(hexDigits, hexDigits + 1, sizeof(hexDigits) - 1);
            hexDigits[7] = keycode[row][col] & 0xFF;
            hexDigits[8] = '\0';
            max7219.DisplayText(hexDigits, 1);
#ifdef DEBUG
            Serial.println(String("Hex digit: ") + (char)(keycode[row][col] & 0xFF));
#endif
          } else if (keycode[row][col] & METAKEY) {
            addMeta(keycode[row][col] & 0xFF);
            isChanged = true;
          } else {
            addKeyToBuffer(keycode[row][col]);
            isChanged = true;
          }
        }
      } else if (!keydown[row][col] && oldkeydown[row][col]) {
        if (keycode[row][col] != -1 && !(keycode[row][col] & HEXDIGIT)) {
          isChanged = true;
        }
      }
      // Update the saved state
      oldkeydown[row][col] = keydown[row][col];
    }
  }
  if (isChanged) {
    sendKeys();
  }
}
#pragma endregion "Keyboard scanning code"

#pragma region "HID code"

const byte keyLimit = 6;
byte keyPlace = 0;
uint8_t keyBuf[keyLimit];
uint8_t nullBuf[keyLimit];
uint8_t meta = 0;

void addMeta(uint8_t metacode) {
  meta |= metacode;
}

void addKeyToBuffer(uint8_t key) {
  if (keyPlace < keyLimit) {
    keyBuf[keyPlace++] = key;
  }
}

void sendKeys() {
  sendBuffer(meta, keyBuf);
}

void sendBuffer(uint8_t meta, uint8_t keyBuf[]) {
  sendKeyBuffer(meta, keyBuf);
  resetKeys();
}

void resetKeys() {
  memset(keyBuf, 0, sizeof(keyBuf));
  memset(nullBuf, 0, sizeof(nullBuf));
  keyPlace = 0;
  meta = 0;
}

#pragma endregion "HID code"
