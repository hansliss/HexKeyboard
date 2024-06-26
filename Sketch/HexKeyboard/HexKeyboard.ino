/*
 * Hexadecimal keypad code
 *
 */

#include "KeyboardLib.h"
#include "usb_hid_keys.h"
#include <Mouse.h>
#include <SPI.h>
#include <SD.h>
#include "max7219.h"

#define DEBUG
#define ESPDEBUG
#define SCANDEBUG
#define TRISTATESTROBE
#define DOWIFI
#ifdef ESPDEBUG
#define DEBUG
#endif

struct confValues_s {
  char *name;
  char **valuep;
};

// Select the timers you're using, here ITimer4
#define USE_TIMER_1     false
#define USE_TIMER_2     false
#define USE_TIMER_3     false
#define USE_TIMER_4     true
#define USE_TIMER_5     false
#include <TimerInterrupt.h>
#include <ISR_Timer.h>

#define CSPIN 8
#define CDPIN 17

// Talk to the ESP-01's AT firmware on this port
#define ESPSERIAL Serial1
#define ESPBAUD 9600

boolean cardInserted = false;

// unsigned int rowpins[] = {0, 1, 4, 30, 29}; // Prototype
unsigned int rowpins[] = {4, 12, 6, 7, 23}; // New version
unsigned int colpins[] = {22, 21, 20, 19, 18};

#define NROWS (sizeof(rowpins) / sizeof(unsigned int))
#define NCOLS (sizeof(colpins) / sizeof(unsigned int))

#define HEXDIGIT (1<<8)
#define METAKEY (1<<9)
#define SETUP (1<<10)

const unsigned int keycode[NROWS][NCOLS] = {
  {HEXDIGIT | 'A', HEXDIGIT | 'b', HEXDIGIT | 'C', HEXDIGIT | 'd', SETUP | KEY_UP},
  {HEXDIGIT | '7', HEXDIGIT | '8', HEXDIGIT | '9', HEXDIGIT | 'E', SETUP | KEY_DOWN},
  {HEXDIGIT | '4', HEXDIGIT | '5', HEXDIGIT | '6', HEXDIGIT | 'F', -1},
  {HEXDIGIT | '1', HEXDIGIT | '2', HEXDIGIT | '3', SETUP | KEY_PROPS, SETUP | KEY_ENTER},
  {-1, HEXDIGIT | '0', -1, SETUP | KEY_LEFT, -1}
};

#define BUFLEN 64

MAX7219 max7219;

char tmpbuf[BUFLEN];


void setup() {
  TXLED0;
  RXLED0;
  pinMode(CSPIN, OUTPUT);
  digitalWrite(CSPIN, HIGH);
  pinMode(CDPIN, INPUT_PULLUP);

  initKeyboard();
  initHexpad();
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
}

void readFromSD() {
  if (SD.begin(CSPIN)) {
#ifdef DOWIFI
    checkWifi();
#endif
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

void loop() {
  static unsigned long stimeMillis = millis();
  unsigned long nowMillis = millis();
  long diff = nowMillis - stimeMillis;
  if (nowMillis < stimeMillis) {
    diff = nowMillis + (UINT32_MAX - stimeMillis);
  }

  boolean newCardInserted = (digitalRead(CDPIN) == LOW);
  if (newCardInserted && !cardInserted) {
#ifdef DEBUG
    Serial.println("Card inserted");
    readFromSD();
#endif
  } else if (!newCardInserted && cardInserted) {
#ifdef DEBUG
    Serial.println("Card removed");
#endif
  }
  cardInserted = newCardInserted;

  //checkForKeyboardChanges();

  checkHexDigitQueue();

  if (diff > random(3000) + 2000) {
//    if (random(100) > 60) {
      moveMouse();
//    }
    stimeMillis = nowMillis;
  }
  delay(1);
}

#pragma region "Hexpad code"
unsigned char hexDigitQueue[32];
int hexDigitQueueStart=0;
int hexDigitQueueNext=0;

void addToHexDigitQueue(unsigned char digit) {
  hexDigitQueue[hexDigitQueueNext] = digit;
  hexDigitQueueNext = (hexDigitQueueNext + 1) % sizeof(hexDigitQueue);
  if (hexDigitQueueNext == hexDigitQueueStart) {
    hexDigitQueueStart = (hexDigitQueueStart + 1) % sizeof(hexDigitQueue);
  }
}

unsigned char fetchFromHexDigitQueue() {
  int retval = -1;
  noInterrupts();
  if (hexDigitQueueStart != hexDigitQueueNext) {
    retval = hexDigitQueue[hexDigitQueueStart];
    hexDigitQueueStart = (hexDigitQueueStart + 1) % sizeof(hexDigitQueue);
  }
  interrupts();
  return retval;
}

boolean hasMoreHexDigits() {
  return  (hexDigitQueueStart != hexDigitQueueNext);
}

int hexpad_mode = 0;
int hexpad_worddigits = 2;
int hexpad_linelength = 16;
char *hexpad_prefix = "0x";
char *hexpad_delimiter = ", ";
char *hexpad_eol = ",\n";

unsigned char hexDigits[9];

void initHexpad() {
  memset(hexDigits, ' ', sizeof(hexDigits));
  hexDigits[sizeof(hexDigits) - 1] = '\0';
}

void checkHexDigitQueue() {
  while (hasMoreHexDigits()) {
    unsigned char newHexDigit = fetchFromHexDigitQueue();
    memmove(hexDigits, hexDigits + 1, sizeof(hexDigits) - 1);
    hexDigits[7] = newHexDigit;
    hexDigits[8] = '\0';
    max7219.DisplayText(hexDigits, 1);
#ifdef DEBUG
    // Serial.println(String("Hex digit: ") + (char)(keycode[row][col] & 0xFF));
#endif
    hexpad_addDigit(newHexDigit);
  }
}

void hexpad_addDigit(unsigned char digit) {
  static int currentDigits = 0;
  static int currentWords = 0;
  static char wordBuf[9];
  wordBuf[currentDigits] = toupper(digit);
  currentDigits++;
  wordBuf[currentDigits] = '\0';
  if (currentDigits >= hexpad_worddigits) {
    if (currentWords > 0) {
#ifdef DEBUG
      Serial.print(hexpad_delimiter);
#endif
      // output hexpad_delimiter
    }
#ifdef DEBUG
    Serial.print(hexpad_prefix);
    Serial.print(wordBuf);
#endif
    // output prefix
    // output word
    currentDigits = 0;
    currentWords++;
    if (currentWords >= hexpad_linelength) {
#ifdef DEBUG
      Serial.print(hexpad_eol);
#endif
      // output hexpad_eol
      currentWords = 0;
    }
  }
}

#pragma endregion "Hexpad code"

#pragma region "Keyboard scanning code"
#define STABILIZE_STROBE_DELAY 5
#define SCANNER_INTERVAL_MS 1

// Keep track of key state changes, in the easiest way possible
uint8_t keydown[NROWS][NCOLS];
uint8_t oldkeydown[NROWS][NCOLS];

unsigned long debounceDelay = 10;    // the debounce time; increase if the output flickers

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

  // Init timer ITimer4
  ITimer4.init();
  // Interval in unsigned long millisecs
  ITimer4.attachInterruptInterval(SCANNER_INTERVAL_MS, scanKeyboard);
}

void handleSetupKey(int keycode) {
  switch(keycode) {
    case KEY_PROPS:
#ifdef DEBUG
      Serial.println("Enter setup");
#endif
    break;
  case KEY_UP:
#ifdef DEBUG
      Serial.println("Menu Up");
#endif
    break;
  case KEY_DOWN:
#ifdef DEBUG
    Serial.println("Menu Down");
#endif
    break;
  case KEY_LEFT:
#ifdef DEBUG
    Serial.println("Menu exit");
#endif
    break;
  case KEY_ENTER:
#ifdef DEBUG
    Serial.println("Menu confirm");
#endif
    break;
  default:
#ifdef DEBUG
    Serial.print("Unknown setup key pressed: ");
    Serial.println(keycode);
#endif
    break;
  }
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
#ifdef SCANDEBUG
            Serial.println(String("Key down : (") + col + "," + row + ")");
#endif
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
  checkForKeyboardChanges();
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
            addToHexDigitQueue(keycode[row][col] & 0xFF);
          } else if (keycode[row][col] & METAKEY) {
            addMeta(keycode[row][col] & 0xFF);
            isChanged = true;
          } else if (keycode[row][col] & SETUP) {
            // TODO: Add a keystroke queue for setup keys
            // handleSetupKey(keycode[row][col] & 0xFF);
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
    // TODO: Move the HID code outside the interrupt handler
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

#pragma region "Config file code"

void trimEOL(char *str) {
  while (strlen(str) > 0 && (str[strlen(str) - 1] == '\n' || str[strlen(str) - 1] == '\r')) {
    str[strlen(str) - 1] = '\0';
  }
}

boolean readLine(File infile, char *buf, int bufsize) {
  int n = 0;
  int sz = infile.available();
  while (sz && n < bufsize - 1) {
    buf[n] = infile.read();
    sz--;
    if (buf[n] == '\n') {
      break;
    }
    n++;
  }
  buf[n] = '\0';
  trimEOL(buf);
  return (n > 0);
}

char *split(char *string, char delim) {
  char *second = string;
  while (*second && *second != delim) {
    second++;
  }
  if (*second) {
    char *tmp = second;
    while(tmp > string && *tmp == delim || isWhitespace(*tmp)) {
      *(tmp--) = '\0';
    }
    do {
      *(second++) = '\0';
    } while (*second && isWhitespace(*second));
  }
  return second;
}

boolean readConfig(char *filename, struct confValues_s *confValues, int nConfValues) {
  boolean success = false;
  File infile;
  if ((infile = SD.open(filename)) != 0) {
    for (int i=0; i<nConfValues; i++) {
      if ((*(confValues[i].valuep)) != NULL) {
        free(*(confValues[i].valuep));
        *(confValues[i].valuep) = NULL;
      }
    }
    while (readLine(infile, tmpbuf, sizeof(tmpbuf))) {
      char *value = split(tmpbuf, '=');
      if (value) {
        for (int i=0; i<nConfValues; i++) {
          if (!strcasecmp(confValues[i].name, tmpbuf)) {
            *(confValues[i].valuep) = strdup(value);
          }
        }
      }
    }
    infile.close();
  }
  return success;
}
#pragma endregion "Config file code"

#pragma region "WiFi code"

#ifdef DOWIFI
char *ssid = NULL;
char *passphrase = NULL;
char *hostname = NULL;

struct confValues_s wiFiConf[] = {
  {"ssid", &ssid},
  {"passphrase", &passphrase},
  {"hostname", &hostname}
};

const int nWiFiConf = 3;

void checkWifi() {
  readConfig("wifi.cnf", wiFiConf, nWiFiConf);
  if (ssid && passphrase) {
    connectWiFi();
  }
}

boolean expect(char *expected1, char *expected2, char *failString, int timeout) {
  int resppos = 0;
  memset(tmpbuf, 0, sizeof(tmpbuf));
  long startmillis=millis();
  while (1) {
    if (millis() - startmillis > timeout * 1000) {
      break;
    }
    while (ESPSERIAL.available() > 0) {
      char c = ESPSERIAL.read();
      if (resppos == sizeof(tmpbuf) - 1) {
        memmove(tmpbuf, tmpbuf + 1, sizeof(tmpbuf) - 1);
        resppos--;
      }
      tmpbuf[resppos++] = c;
#ifdef ESPDEBUG
      Serial.print(c);
#endif
      if (strstr(tmpbuf, expected1) != NULL || (expected2 != NULL && strstr(tmpbuf, expected2) != NULL)) {
        return true;
      } else if (failString != NULL && strstr(tmpbuf, failString) != NULL) {
#ifdef ESPDEBUG
        Serial.println("Received fail response");
#endif
        return false;
      }
    }
  }
#ifdef ESPDEBUG
  Serial.println("Timeout");
#endif
  return false;
}

boolean executeCommand(char *command, char *success, char *success2, char *failure, int timeout, int retries, int retryDelay) {
  for (int i = 0; i < retries; i++) {
    ESPSERIAL.print(command);
    ESPSERIAL.print("\r\n");
    if (expect(success, success2, failure, timeout)) {
      return true;
    } else {
      delay(retryDelay * 1000);
    }
  }
  return false;
}

boolean connectWiFi() {
  boolean retval = false;
  boolean ok = false;
  ESPSERIAL.begin(ESPBAUD);
  if (!ssid || !passphrase) {
    return false;
  }

  if (!executeCommand("AT+CWSTATE?", ssid, NULL, NULL, 2, 2, 2)) {
#ifdef DEBUG
    Serial.println("Not connected. Reconnecting.");
#endif
    expect("OK\r\n", NULL, NULL, 3);
    if (!executeCommand("AT+RST", "OK\r\n", NULL, NULL, 3, 5, 2)) {
      return false;
    }
#ifdef ESPDEBUG
    else {
      expect("ready", "GOT IP\r\n", NULL, 9);
      Serial.println("Module Reset");
    }
#endif
    if (!executeCommand("AT+CWMODE=1", "OK\r\n", NULL, NULL, 3, 5, 2)) {
      return false;
    }
#ifdef ESPDEBUG
    else {
      Serial.println("Mode set");
    }
#endif
    if (!executeCommand("AT+CWLAP", ssid, NULL, NULL, 10, 5, 2)) {
      return false;
    }
#ifdef ESPDEBUG
    else {
      Serial.println("SSID found");
    }
#endif
    expect("OK", NULL, NULL, 2);
    if (!executeCommand("AT+SYSSTORE=1", "OK\r\n", NULL, NULL, 4, 1, 2)) {
      return false;
    }
#ifdef ESPDEBUG
    else {
      Serial.println("SYSSTORE set");
    }
#endif
    if (!executeCommand("AT+CWAUTOCONN=1", "OK\r\n", NULL, NULL, 3, 5, 2)) {
      return false;
    }
#ifdef ESPDEBUG
    else {
      Serial.println("AUTOCONN set");
    }
#endif
    if (!executeCommand("AT+CWRECONNCFG=2,0", "OK\r\n", NULL, NULL, 3, 5, 2)) {
      return false;
    }
#ifdef ESPDEBUG
    else {
      Serial.println("RECONNCFG set");
    }
#endif
    if (!executeCommand("AT+CWDHCP=1,3", "OK\r\n", NULL, NULL, 3, 5, 2)) {
      return false;
    }
#ifdef ESPDEBUG
    else {
      Serial.println("DHCP config set");
    }
#endif
    if (hostname) {
      sprintf(tmpbuf, "AT+CWHOSTNAME=\"%s\"", hostname);
      if (!executeCommand(tmpbuf, "OK\r\n", NULL, NULL, 5, 5, 2)) {
        return false;
      }
#ifdef ESPDEBUG
      else {
        Serial.println("Hostname set");
      }
#endif
    }
    sprintf(tmpbuf, "AT+CWJAP=\"%s\",\"%s\"", ssid, passphrase);
    if (!executeCommand(tmpbuf, "OK\r\n", NULL, NULL, 10, 10, 5)) {
      return false;
    }
#ifdef ESPDEBUG
    else {
      Serial.println("Connected");
    }
#endif
    if (!executeCommand("AT+CIPSNTPCFG=1,0,\"ntp.se\"", "OK\r\n", NULL, NULL, 10, 1, 5)) {
      return false;
    }
#ifdef ESPDEBUG
    else {
      Serial.println("SNTP configured");
    }
#endif
#ifdef DEBUG
    Serial.println("Connected to WiFi");
#endif
  } else {
#ifdef DEBUG
    Serial.println("WiFi already connected");
#endif
  }
}
#endif
#pragma endregion "WiFi code"