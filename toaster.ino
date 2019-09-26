
#include "max6675.h"
#include "U8glib.h"
#include <EEPROM.h>

/* TODO - convert max6675 library to use normal SPI */
U8GLIB_SSD1306_128X64_2X u8g(10, 8, 7); //FT 7pin OLED - works great!
//U8GLIB_SSD1306_128X64 u8g(10, 8, 7);

/* some constants */
PROGMEM const unsigned long M_0    = 0UL;
PROGMEM const unsigned long M_200  = 200UL;
PROGMEM const unsigned long M_500  = 500UL;
PROGMEM const unsigned long M_1000 = 1000UL;
PROGMEM const unsigned long M_2500 = 2500UL;

/* screens */
PROGMEM const byte SPLASH     = 0;
PROGMEM const byte MAIN       = 1;
PROGMEM const byte STARTEDIT  = 2;
PROGMEM const byte EDITNAME   = 3;
PROGMEM const byte EDITVALUES = 4;
PROGMEM const byte RUNNING    = 5;
PROGMEM const byte CANCEL     = 6;
PROGMEM const byte SUBEDIT    = 7;

char characters[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ abcdefghijklmnopqrstuvwxyz1234567890";

PROGMEM const int eeAddress = 0;

volatile int mainItem = 0;
int seItem = 0;
int subItem = 0;
int valItem = 0;
int window = 0;
int lineItem = 0;
int strItem = 0;
int canItem = 0;
char * curChar = 0;

boolean flash = true;
boolean flashing = true;

volatile byte    screen = SPLASH;
volatile boolean redraw = true;
unsigned long lastRefresh;
unsigned long lastAccelerate;

volatile unsigned long runTime;
volatile unsigned long ticks;
volatile unsigned int curStep;
volatile int relayPin = A0;

typedef struct {
  volatile unsigned long ticksFrom;
  volatile unsigned long ticksTo;
} stepDuration;

stepDuration stepTicks[6];

typedef struct {
  volatile int  targetTemp;
  volatile int  rampTimeH;
  volatile int  rampTimeM;
  volatile int  rampTimeS;
} tempStep;

typedef struct {
  char     progName[15];
  volatile tempStep steps[6];
  volatile int      numSteps;
} programme;

int graphY[128];

programme programmes[3];

typedef struct {
  byte           pin;
  boolean        is_pressed;
  boolean        debouncing;
  unsigned long  debounceTime;
  boolean        debouncePressed;
  byte           accelerator;
} button;

button buttons[] = {
  { 2, true, true, 0, false, 1 },
  { 3, true, true, 0, false, 1 },
  { 4, true, true, 0, false, 1 }
};

PROGMEM const unsigned long DEBOUNCE = 5UL;  // button debouncer, how many ms to debounce, 5+ ms is usually plenty
PROGMEM const byte NUMBUTTONS = 3;
PROGMEM const boolean BPRESSED = false;

PROGMEM const byte BSELECT = 0;
PROGMEM const byte BUP = 1;
PROGMEM const byte BDOWN = 2;

volatile boolean isRunning = false;
volatile int targetTemp;

int thermoDO = 9;
int thermoCS = 6;
int thermoCLK = 5;

MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);

volatile float celcius;



void setup() {
  //Serial.begin(115200);
  //Serial.println("MAX6675 test");
  EEPROM.get(eeAddress, programmes);
  char empty[] = "empty         ";
  byte first;
  EEPROM.get(EEPROM.length() - 1, first);
  if ( first ) {
    //Serial.println("in first");
    //Serial.println(first);
    EEPROM.put(EEPROM.length() - 1, 0);

    for (int i = 0; i < 3; i++)
    {
      strcpy(programmes[i].progName, empty);
      programmes[i].numSteps = 0;
      for (int j = 0; j < 6; j++)
      {
        programmes[i].steps[j].targetTemp = 20;
        programmes[i].steps[j].rampTimeH = 0;
        programmes[i].steps[j].rampTimeM = 0;
        programmes[i].steps[j].rampTimeS = 0;
      }
      EEPROM.put(eeAddress, programmes);
    }
  }

  pinMode(A2, OUTPUT);
  digitalWrite(A2, LOW);

  //  for (int i=0;i<3;i++)
  //  {
  //    Serial.println(programmes[i].numSteps);
  //  }

  for (int i = 0; i < NUMBUTTONS; i++) {
    pinMode(buttons[i].pin, INPUT);
    digitalWrite(buttons[i].pin, HIGH);
  }

  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW);

  // wait for MAX chip to stabilize
  delay(500);

  // start our timer
  noInterrupts();           // disable all interrupts
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;

  OCR1A = 31250;            // compare match register 16MHz/256/2Hz
  TCCR1B |= (1 << WGM12);   // CTC mode
  TCCR1B |= (1 << CS12);    // 256 prescaler
  TIMSK1 |= (1 << OCIE1A);  // enable timer compare interrupt
  interrupts();             // enable all interrupts
}

void loop() {
  if (millis() - lastRefresh > M_500 /*&& screen == EDITVALUES */)
  {

    redraw = true;

    if (flashing && screen == EDITVALUES)
    {
      flash = !flash;
    }
    else
    {
      flash = true;
    }
  }

  drawScreen();

  handleButtons();

  //Serial.println(celcius);
}


