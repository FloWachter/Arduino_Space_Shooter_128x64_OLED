
/*********************************************************************
  Written by Florian Wachter
  florianwachter.com
  UX Designer & Developer
  
  Graphics created with:
  arduino.cc/reference/en/language/variables/data-types/array/

*********************************************************************/
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "tones.h"
#include "starfield.h" 

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

// Global varaibles
uint8_t color = WHITE;
int speakerPin = 8;
int bullet_button = 2;
int buttonState = 0;


// ----------------------------------------------------------
// Init Functions
// ----------------------------------------------------------
void init_display() {
  Serial.begin(9600);
  // initialize with the I2C addr 0x3D (for the 128x64)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3D);  
  display.display();
  delay(10);
  display.clearDisplay();
}

void init_poti() {
  int poti = A0;
  int potiValue = 0;
  pinMode(poti, INPUT);
}

void init_bullet_button(){
  pinMode(bullet_button, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  
}

// ----------------------------------------------------------
// Display Functions
// ----------------------------------------------------------
void display_ship(int pos){
  static const unsigned char PROGMEM spaceShip_img[] = {
  0x78, 0x00, 0x7c, 0x00, 0x30, 0x00, 0x3f, 0x80, 0xfc, 0xc0, 0xfc, 
  0xc0, 0x3f, 0x80, 0x30, 0x00,0x7c, 0x00, 0x78, 0x00 };
  display.drawBitmap(10, pos,spaceShip_img, 10, 10, 1);
}

int shipPosition(){
  int potiValue = analogRead(A0);
  return map(potiValue, 40, 1024, 0, display.height()-10); 
}

bool fire_bullet() {
  buttonState = digitalRead(bullet_button);
  if (buttonState == !HIGH) {
    tone(speakerPin, NOTE_A5, 100);
    digitalWrite(LED_BUILTIN, HIGH);
    return true;
  }else{
    digitalWrite(LED_BUILTIN, LOW);
    return false;
  }
}

void bullet(bool fire) {
  if( fire == true ){
    for (uint8_t i = 0; i < display.width()-10; i++) {
      display.drawPixel(i, shipPosition()+5, WHITE);
    } 
  } 
}
 
// ----------------------------------------------------------
// SETUP
// ----------------------------------------------------------
void setup() {
  init_display();
  init_poti();
  init_starfield();
  init_bullet_button();
  
}

// ----------------------------------------------------------
// LOOP 
// ----------------------------------------------------------
void loop() {
  move_stars();
  draw_stars();
  
  display_ship(shipPosition());
  
  bullet(fire_bullet());
  
  display.display();
  delay(5);
  display.clearDisplay();
}


