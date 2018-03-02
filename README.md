# LCDtemperatureDisplayer
Arduino based project, reads temperature and humidity from a AM2320 sensor, and displays the temperature and humidity on a character LCD

## Required Hardware
The following hardware are required:
  - Arduino Leonardo
  - Character LCD Display
  - I2C LCD Interface
  - AM2320 Temperature and Humidity sensor
  
## Required Software
The following software are required:
  - Arduino IDE Version 1.8.5
  - Various 3rd Party Libraries (can be installed from the library manager)
    - LiquidCrystal
    - LiquidCrystal_I2C
    - Adafruit AM2320
  
## Howto connect everything up


## Howto Get It Running
**TODO**

## Howto configure the code
The code makes provision to use two different kinds of LCD which as per the interconnection diagram **TODO** have there own I2C interfaces each with there own "unique" I2C address.

uncomment or comment the USE_20_4_LCD line to either for when the 20x4 LCD or the 8x2 are connected respectively.

## Last thoughts
**HAVE FUN!!**
