#include <Wire.h>
#include <Adafruit_AM2320.h>

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

//#define USE_LCD_20_4

#ifndef USE_LCD_20_4
LiquidCrystal_I2C lcd(0x27, 8, 2); // set the LCD address to 0x27 for a 8 chars and 2 line display
//LiquidCrystal_I2C lcd(0x3F, 8, 2); // set the LCD address to 0x27 for a 8 chars and 2 line display
#endif

#ifdef USE_LCD_20_4
LiquidCrystal_I2C lcd(0x3F, 20, 4); // set the LCD address to 0x3F for a 20 chars and 4 line display
#endif

// CONSTANTS
#define SENSOR_DURATION 30

Adafruit_AM2320 am2320;
char str_temp[20];

double a = 17.271;
double b = 237.7;

int duration_counter = 0;

void setup() {
  Serial.begin(9600);
  lcd.init();                      // initialize the lcd
  lcd.clear();
#ifndef USE_LCD_20_4
  lcd.backlight();
#endif

  delay(1000);
  Serial.println("AM2320 Toets! + LCD");
#ifndef USE_LCD_20_4
  lcd.setCursor(0, 0);
  lcd.print(" AM2320 ");
  lcd.setCursor(0, 1);
  lcd.print("plus LCD");
  delay(2000);
  lcd.setCursor(0, 0);
  lcd.print("Doel    ");
  lcd.setCursor(0, 1);
  //           12345678901234567890123456789012345678901
  lcd.print("Meet lug se temperatuur en vogtigheid");
  delay(1000);
  for (int positionCounter = 0; positionCounter < (51 - 16 - 6); positionCounter++) {
    // scroll one position left:
    lcd.scrollDisplayLeft();
    lcd.setCursor(positionCounter+1, 0);
    lcd.print("Doel");
    // wait a bit:
    delay(250);
  }
  delay(2000);
  lcd.clear();
  lcd.home();
  lcd.setCursor(0, 0);
  lcd.print("Weergawe");
  lcd.setCursor(0, 1);
  lcd.print("2.1     ");
  delay(2000);
  lcd.setCursor(0, 0);
  lcd.print("Outeur  ");
  lcd.setCursor(0, 1);
  lcd.print("Rudolph ");
  delay(1000);
  lcd.setCursor(0, 1);
  lcd.print("Lombaard");
  delay(1000);
  lcd.setCursor(0, 0);
  lcd.print("Datum   ");
  lcd.setCursor(0, 1);
  lcd.print("01Mar18 ");
  delay(2000);
#else
  lcd.setCursor(0, 0);
  lcd.print("AM2320 Toets! + LCD");
  lcd.setCursor(0, 1);
  lcd.print("Weergawe: 2.1");
  lcd.setCursor(0, 2);
  lcd.print("Outeur  : R.Lombaard");
  lcd.setCursor(0, 3);
  lcd.print("Datum: 01 Maart 2018");
  delay(3000);
  //  lcd.setCursor(0, 1);
  //  lcd.print("Toets+LCD");
#endif
  if (! am2320.begin()) {
    Serial.println("Sensor not found, check wiring & pullups!");
    lcd.setCursor(0, 0);
    lcd.print("AM2320");
    lcd.setCursor(0, 1);
    lcd.print("!found");
    while (1);
  }
  lcd.clear();
  duration_counter = SENSOR_DURATION;
}

//Parts of this code is from
//http://forum.arduino.cc/index.php?topic=107569.0

//Dex point and humidex calculator
//Humidex = (air temperature) + h
// h = (0.5555)*(e - 10.0);
// e = 6.11 * exp(5417.7530 * ((1/273.16) - (1/dewpoint)))

// http://www.physlink.com/reference/weather.cfm

// heat index (HI) or humiture
// http://en.wikipedia.org/wiki/Heat_index
// http://www.nws.noaa.gov/os/heat/index.shtml#heatindex
//HI = -42.379 + 2.04901523T + 10.14333127R - 0.22475541TR - 6.83783x10
//    -3T2 - 5.481717x10 - 2R2 + 1.22874x10 -3T2R + 8.5282x10
// -4TR2 - 1.99x10-6T2R2
// where T = ambient dry bulb temperature (°F)
// R = relative humidity (integer percentage).
// http://www.4wx.com/wxcalc/formulas/heatIndex.php


//Heat Index = -42.379 + (2.04901523 x T) + (10.14333127 x R) - (0.22475541 x T x R) - (6.83783x10-3 x T2) - (5.481717x10-2 x R2) + (1.22874x10-3 x T2 x R) + (8.5282x10-4 x T x R2) - (1.99x10-6 x T2 x R2)
// Where,
//T = Temperature in ?F
// R = Relative Humidity in %
// Heat index is utilized only in warm weather  >=70° F
// http://www.gorhamschaffler.com/humidity_formulas.htm

double calculatDEWpoint(double temp, double RH)
{
  // for dewpoint see: http://en.wikipedia.org/wiki/Dew_point

  double gammavalue = calculateDewPointGammaValue(temp, RH);

  double dewpoint = (b * gammavalue) / (a - gammavalue);
  //  Serial.print("Dewpoint = ");
  //  Serial.println(dewpoint);
  return dewpoint;

}

double calculateDewPointGammaValue (double temp, double RH)
{
  double gamma = ((a * temp) / (b + temp)) + log (RH / 100);
  return gamma;
}


double calculateHumidexValue (double temp, double DewPoint)
{

  double h = temp + 0.5555 * ( 6.11 *  exp ( calculateHumidexEValue(DewPoint)) - 10);
  //  Serial.print("Humidex = ");
  //  Serial.println(h);
  return h;
}

double calculateHumidexEValue (double DewPoint)
{

  double e = 5417.7530 * ((1 / 273.16) - (1 / (273.16 + DewPoint)));
  //  Serial.print("evalue = ");
  //  Serial.println(e);
  return e;
}

float get_relative_temp(float t, float h) {
  double dewpoint = calculatDEWpoint(t, h);
  return calculateHumidexValue(t, dewpoint);
}

void loop() {
  //  Serial.print("Hum: "); Serial.println(am2320.readHumidity());
  //  Serial.print("Temp: "); Serial.println(am2320.readTemperature());

  duration_counter++;
  if (duration_counter > SENSOR_DURATION) {
    duration_counter = 0;
    float t;
    float h;
    am2320.readTemperatureAndHumidity(t, h);

    Serial.print("Temp: "); Serial.println(t);
    Serial.print("Hum : "); Serial.println(h);

#ifndef USE_LCD_20_4
    lcd.setCursor(0, 0);
    lcd.print("Tmp:"); dtostrf(t, 2, 1, str_temp); lcd.print(str_temp);
    lcd.setCursor(0, 1);
    lcd.print("Hum:"); dtostrf(h, 2, 1, str_temp); lcd.print(str_temp);
#else
    lcd.setCursor(0, 0);
    //  lcd.print("MEASUREMENT: AIR");
    lcd.print("MEETING    : LUG");
    lcd.setCursor(0, 1);
    dtostrf(t, 4, 1, str_temp);
    lcd.print("Temperatuur: "); lcd.print(str_temp); lcd.print(char(0xDF)); lcd.print("C");
    lcd.setCursor(0, 2);
    dtostrf(h, 4, 1, str_temp);
    lcd.print("Vogtigheid : "); lcd.print(str_temp); lcd.print("%Rh");
    //  sprintf(str_temp, "%.1f\n", t);  //NOT IMPLEMENTED in Arduino
    lcd.setCursor(0, 3);
    lcd.print("Voel soos  : ");
    dtostrf(get_relative_temp(t, h), 4, 1, str_temp);
    lcd.print(str_temp); lcd.print(char(0xDF)); lcd.print("C");
#endif
  }
#ifdef USE_LCD_20_4
  lcd.setCursor(17, 0);
  //  dtostrf(SENSOR_DURATION - duration_counter, 2, 0, str_temp);
  sprintf(str_temp, "%03d", (SENSOR_DURATION - duration_counter));
  //  lcd.print("   ");
  //  lcd.setCursor(17,0);
  lcd.print(str_temp);
#endif

  delay(1000);
}
