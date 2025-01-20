#include <TFT_eSPI.h>
#include <math.h>

// Meter colour schemes
#define RED2RED 0
#define GREEN2GREEN 1
#define BLUE2BLUE 2
#define BLUE2RED 3
#define GREEN2RED 4
#define RED2GREEN 5

#define TFT_GREY 0x2104 // Dark grey 16-bit colour

TFT_eSPI tft = TFT_eSPI(); // Invoke custom library with default width and height

const int SHUNT_PIN = 35;    // ADC pin for shunt
const float SHUNT_RESISTANCE = 0.00075; // Shunt resistance in ohms, 75mv!
const float ADC_MAX_VOLTAGE = 3.3;     // Max voltage for ESP32 ADC, the pin that measures the current, if you draw such a huge amount of current that it excedes 3.3V may god be with you!
const int ADC_RESOLUTION = 4095;       // ESP32 ADC resolution

const float HARD_CODED_VOLTAGE = 12.0; // Hardcoded DC voltage
const float MAX_POWER = 600.0;        // Maximum power for gauge scaling
const float RED_ZONE_THRESHOLD = 500.0; // Threshold for red zone activation

uint32_t runTime = -99999;       // time for next update
float current = 0.0;
float power = 0.0;
int reading = 0; // Value to be displayed
int d = 0; // Variable used for the sinewave test waveform
bool range_error = 0;
int8_t ramp = 1;

void setup() {
  tft.begin();
  tft.setRotation(1); // Landscape mode
  tft.fillScreen(TFT_BLACK);
}

void loop() {
  if (millis() - runTime >= 0L) {
    runTime = millis();

    // Read current from shunt
    int shuntRaw = analogRead(SHUNT_PIN);

    // Convert ADC reading to current
    float shuntVoltage = (shuntRaw / float(ADC_RESOLUTION)) * ADC_MAX_VOLTAGE; // Voltage across shunt
    current = shuntVoltage / SHUNT_RESISTANCE; // Calculate current

    // Calculate power
    power = HARD_CODED_VOLTAGE * current;

    // Map power to reading for the ring meter
    reading = constrain((int)power, 0, MAX_POWER);

    // Draw the ring meter
    int xpos = 480 / 2 - 160, ypos = 0, radius = 170;
    ringMeter(reading, 0, MAX_POWER, xpos, ypos, radius, "Watt", GREEN2RED);

    delay(100); // Small delay for refresh
  }
}

// #########################################################################
//  Draw the meter on the screen, returns x coord of righthand side
// #########################################################################
int ringMeter(int value, int vmin, int vmax, int x, int y, int r, const char *units, byte scheme) {
  x += r; y += r;   // Calculate coords of centre of ring

  int w = r / 3;    // Width of outer ring is 1/4 of radius
  int angle = 150;  // Half the sweep angle of meter (300 degrees)

  int v = map(value, vmin, vmax, -angle, angle); // Map the value to an angle v
  byte seg = 3; // Segments are 3 degrees wide = 100 segments for 300 degrees
  byte inc = 6; // Draw segments every 3 degrees, increase to 6 for segmented ring

  int colour = TFT_BLUE; // Default text colour
 
  for (int i = -angle + inc / 2; i < angle - inc / 2; i += inc) {
    float sx = cos((i - 90) * 0.0174532925);
    float sy = sin((i - 90) * 0.0174532925);
    uint16_t x0 = sx * (r - w) + x;
    uint16_t y0 = sy * (r - w) + y;
    uint16_t x1 = sx * r + x;
    uint16_t y1 = sy * r + y;

    float sx2 = cos((i + seg - 90) * 0.0174532925);
    float sy2 = sin((i + seg - 90) * 0.0174532925);
    int x2 = sx2 * (r - w) + x;
    int y2 = sy2 * (r - w) + y;
    int x3 = sx2 * r + x;
    int y3 = sy2 * r + y;

    /* 
    
    Make some asserts for the power 
    Mainly used for the Powersupply errors !

    The Powersupply I am Using is a 600W 12VDC 50A one
    if you have any different one please change these values!

    */
    
    if (current > 600) {
      printf("MAXPOWER REACHED! CANNOT DISPLAY MORE! \n");
    } else if (current > 480) {
      printf("BE CAREFUL! CURRENT EXCEEDED 80 PERCENT of PSU! \n");
    } else if (current < 0) {
      printf("Something is wrong with your measurments! Negative Value recived?!")
    } else if (current > 1000) {
      printf("Something is wrong with your values! Power exceeded 1000W, maximum is to be 600W, Something may be wrong with the PowerSupply?")
    }


    if (i < v) {
      // switch case cause yk

      switch (scheme) {
        case 0: colour = TFT_RED; break;
        case 1: colour = TFT_GREEN; break;
        case 2: colour = TFT_BLUE; break;
        case 3: colour = rainbow(map(i, -angle, angle, 0, 127)); break;
        case 4: colour = rainbow(map(i, -angle, angle, 70, 127)); break;
        case 5: colour = rainbow(map(i, -angle, angle, 127, 63)); break;
        default: colour = TFT_BLUE; break;
      }
      tft.fillTriangle(x0, y0, x1, y1, x2, y2, colour);
      tft.fillTriangle(x1, y1, x2, y2, x3, y3, colour);
    } else {
      tft.fillTriangle(x0, y0, x1, y1, x2, y2, TFT_GREY);
      tft.fillTriangle(x1, y1, x2, y2, x3, y3, TFT_GREY);
    }
  }

  char buf[10];
  byte len = 3; if (value > 999) len = 5;
  dtostrf(value, len, 0, buf);
  buf[len] = ' '; buf[len + 1] = 0;
  tft.setTextSize(1);

  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextDatum(MC_DATUM);

  // Adjust the padding to ensure numbers clear on the left side
  if (r > 84) {
    tft.setTextPadding(65 * 3); // Increased from 55 to 60
    tft.drawString(buf, x, y, 8);
  } else {
    tft.setTextPadding(4 * 14); // Increased from 3 to 4
    tft.drawString(buf, x, y, 4);
  }
  tft.setTextSize(1);
  tft.setTextPadding(0);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  if (r > 84) tft.drawString(units, x, y + 60, 4);
  else tft.drawString(units, x, y + 15, 2);

  return x + r;
}

/* #########################################################################
   Return a 16-bit rainbow colour
   ######################################################################### */
unsigned int rainbow(byte value) {
  byte red = 0;
  byte green = 0;
  byte blue = 0;

  byte quadrant = value / 32;

  if (quadrant == 0) {
    blue = 31;
    green = 2 * (value % 32);
    red = 0;
  }
  if (quadrant == 1) {
    blue = 31 - (value % 32);
    green = 63;
    red = 0;
  }
  if (quadrant == 2) {
    blue = 0;
    green = 63;
    red = value % 32;
  }
  if (quadrant == 3) {
    blue = 0;
    green = 63 - 2 * (value % 32);
    red = 31;
  }
  return (red << 11) + (green << 5) + blue;
}
