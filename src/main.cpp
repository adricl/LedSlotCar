#include <Arduino.h>
#include <FastLED.h>

// How many leds in your strip?
#define NUM_LEDS 180

#define DATA_PIN 2
#define CLOCK_PIN 3
#define LED_TYPE    APA102
#define COLOR_ORDER GRB

#define CYCLE_SPEED 5

struct CAR {
  int8_t player;
  float position;
  int8_t colourH;
  int8_t colourS;
  int8_t colourV;
  int pin;
  int crashTimer;
};

#define CAR_SIZE 8
#define TRACK_SIZE 50
#define CAR_NUMBER 4
// Define the array of leds
CRGB leds[NUM_LEDS];

CAR cars[CAR_NUMBER];

void setup() { 
  Serial.begin(57600);
  Serial.println("resetting");
  FastLED.addLeds<LED_TYPE,DATA_PIN,CLOCK_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(127);
  FastLED.setDither( 255 );

  //Car Setup for Colours and Numbers
  //Green
  cars[0].position = 0;
  cars[0].player = 1;
  cars[0].colourH = 0;
  cars[0].colourS = 255;
  cars[0].colourV = 200;
  cars[0].pin = -1; //normally 0
  cars[0].crashTimer = 0;
  
  //blue
  cars[1].position = 0;
  cars[1].player = 2;
  cars[1].colourH = 96;
  cars[1].colourS = 255;
  cars[1].colourV = 200;
  cars[1].pin = -1;
  cars[1].crashTimer = 0;

  //yellow
  cars[2].position = 0;
  cars[2].player = 3;
  cars[2].colourH = 192;
  cars[2].colourS = 255;
  cars[2].colourV = 255;
  cars[2].pin = -1;
  cars[2].crashTimer = 0;

  //aqua
  cars[3].position = 0;
  cars[3].player = 4;
  cars[3].colourH = 32;
  cars[3].colourS = 255;
  cars[3].colourV = 255;
  cars[3].pin = -1;
  cars[3].crashTimer = 0;
}

//Wraping the track and dealing with -ve numbers that the tails can generate
int getStripPos(int distance) { 
  if (distance < 0) {
    return NUM_LEDS + distance;
  } else {
    return distance % NUM_LEDS;
  }
}

//Blanks all leds
void blankAllLeds() {
  for(int i = 0; i < NUM_LEDS; i++){
    leds[i] = CRGB::Black;
  }
  FastLED.show(); 
}

//Places car on track with tail
//Tail is caculated as the v in hsv fading over the tail length
void renderNormalCar(CAR car, bool clear){
    if (clear){
      for(int i = 0; i < CAR_SIZE; i++){
        leds[getStripPos(car.position - i)] = CRGB::Black;
      }
    }
    else {
      uint8_t vChange = (uint8_t) car.colourV / (CAR_SIZE - 1);
      
      float percent = car.position - (int)car.position;
      leds[getStripPos(car.position)] += CHSV(car.colourH, car.colourS, car.colourV * percent);
      
      CHSV colour = CHSV(car.colourH, car.colourS, car.colourV);
      leds[getStripPos(car.position - 1)] += colour;
      for(int i = 2; i < CAR_SIZE; i++){
        colour.setHSV(colour.h, colour.s ,colour.v - vChange); 
        leds[getStripPos(car.position - i)] += colour;
      }
    }
}

void renderCrashedCar(CAR car, bool clear){
  if (!clear && ((car.crashTimer % 5) == 0)) {
    renderNormalCar(car, clear);
  }
  car.crashTimer --;
}

void renderCar(int carNum, bool clear) {
  CAR car = cars[carNum];

  if (car.crashTimer > 0){
    renderCrashedCar(car, clear);
  }
  else {
    renderNormalCar(car, clear);
  }
}

void updateCarPosition(int carNum, double updateAmount){
  CAR car = cars[carNum];
  if (car.crashTimer > 0){
    return;
  } 
  else if(car.pin > 0){
    cars[carNum].position += (float)analogRead(car.pin)/1023;
  }
  else {
    cars[carNum].position += updateAmount;
  }
}


//Main race loop
void race(){
  //int laps = 10;

  //display car 0
  for(int i = 0; i < CAR_NUMBER; i++) {
    renderCar(i, false);  
  }

  FastLED.show();

  for(int i = 0; i < CAR_NUMBER; i++) {
    renderCar(i, true); 
  }

  updateCarPosition(0, .5);
  updateCarPosition(1, .75);
  updateCarPosition(2, .25);
  updateCarPosition(3, .2);

  delay(CYCLE_SPEED);
}


void loop(){
  blankAllLeds();
  race();

}
