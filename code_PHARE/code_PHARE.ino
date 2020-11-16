
#include <Wire.h>  // Include Wire if you're using I2C
#include <SPI.h>  // Include SPI if you're using SPI
#include <SFE_MicroOLED.h>  // Include the SFE_MicroOLED library
#include <CDS5500.h>#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

#include <PololuLedStrip.h>
PololuLedStrip<12> ledStrip;
#define LED_COUNT 12
rgb_color colors[LED_COUNT];

CDS5500 SERVO;
int val = 0; // variable de type int pour stocker la valeur de la mesure
int depart = 0;


//////////////////////////
// MicroOLED Definition //
//////////////////////////
#define PIN_RESET 9  // Connect RST to pin 9
#define PIN_DC    8  // Connect DC to pin 8
#define PIN_CS    10 // Connect CS to pin 10
#define DC_JUMPER 0

#define PRINT_SERIAL

//////////////////////////////////
// MicroOLED Object Declaration //
//////////////////////////////////
//MicroOLED oled(PIN_RESET, PIN_DC, PIN_CS); // SPI declaration
MicroOLED oled(PIN_RESET, DC_JUMPER);    // I2C declaration



void setup() {
  Serial.begin(117647);//init Serial baudrate
  oled.begin();    // Initialize the OLED
  oled.clear(ALL); // Clear the display's internal memory
  oled.display();  // Display what's in the buffer (splashscreen)
  delay(1000);     // Delay 1000 ms
  oled.clear(PAGE); // Clear the buffer.
  randomSeed(analogRead(A0) + analogRead(A1));

  printTitle("Init", 1);
  for(uint16_t i = 0; i < LED_COUNT; i++){
    colors[i]=rgb_color(0, 0, 0);
  }
  ledStrip.write(colors, LED_COUNT);
  
  SERVO.WritePos(18,830,100);
  SERVO.WritePos(14,840,100);
  delay(2000);
  printTitle("Ready", 1);
  delay(100);
}

void deploy(){
  depart=1;
  printTitle("5", 1);
  delay(1000);
  printTitle("4", 1);
  delay(1000);
  printTitle("3", 1);
  delay(1000);
  printTitle("2", 1);
  delay(1000);
  printTitle("1", 1);
  delay(1000);
  printTitle("Deploy", 1);
  SERVO.WritePos(18,665,50);
  SERVO.WritePos(14,350,100);
  delay(2000);
  printTitle("SAIL!", 1);
  
  int ledIndex = 0;
  while(true){
    // Update the colors.
    for(uint16_t i = 0; i < LED_COUNT; i++){
      float intensity = 0;
      if(i==ledIndex) intensity = 1;
      int r = 255.f *intensity;
      int g = 0.f *intensity;
      int b = 0.f *intensity;
      colors[i] = rgb_color(r, g, b);
    }
    ledStrip.write(colors, LED_COUNT);
    ledIndex++; if(ledIndex==LED_COUNT) ledIndex=0;
    delay(75);
  }
}

void loop() 
{
  val = analogRead(2);
  if (val > 50 && depart == 0) deploy();

  #ifdef PRINT_SERIAL
    Serial.println(val); 
    delay(10); 
  #endif  
}
 

void printTitle(String title, int font)
{
  int middleX = oled.getLCDWidth() / 2;
  int middleY = oled.getLCDHeight() / 2;
  
  oled.clear(PAGE);
  oled.setFontType(font);
  // Try to set the cursor in the middle of the screen
  oled.setCursor(middleX - (oled.getFontWidth() * (title.length()/2)),
                 middleY - (oled.getFontHeight() / 2));
  // Print the title:
  oled.print(title);
  oled.display();
  delay(10);
  oled.clear(PAGE);
}
