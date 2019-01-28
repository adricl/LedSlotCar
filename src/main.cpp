#include <Arduino.h>
#include <FastLED.h>

// How many leds in your strip?
#define NUM_LEDS 180

// For led chips like Neopixels, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806, define both DATA_PIN and CLOCK_PIN
#define DATA_PIN 2
#define CLOCK_PIN 3
#define LED_TYPE    APA102
#define COLOR_ORDER GRB

#define CYCLE_SPEED 50
struct CAR {
	int8_t player;
	int32_t position;
	int8_t colourH;
	int8_t colourS;
	int8_t colourV;
	int8_t gradientV;
};

#define CAR_SIZE 7
#define TRACK_SIZE 10
// Define the array of leds
CRGB leds[NUM_LEDS];

CAR cars[2];

void setup() { 
	Serial.begin(57600);
	Serial.println("resetting");
	FastLED.addLeds<LED_TYPE,DATA_PIN,CLOCK_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
	FastLED.setBrightness(255);

	//Setting Up Player 1
	cars[0].position = 0;
	cars[0].player = 1;
	cars[0].colourH = 0;
	cars[0].colourS = 255;
	cars[0].colourV = 255;
	cars[0].gradientV = 255;

	cars[1].position = 0;
	cars[1].player = 2;
	cars[1].colourH = 96;
	cars[1].colourS = 255;
	cars[1].colourV = 255;
	cars[1].gradientV = 255;
	
}

//void fadeall() { for(int i = 0; i < NUM_LEDS; i++) { leds[i].nscale8(250); } }

int getStripPos(int distance) { 
	if (distance < 0) {
		return NUM_LEDS + distance;
	} else {
		return distance % NUM_LEDS;
	}
}

void blankLeds() {
	for(int i = 0; i < NUM_LEDS; i++){
		leds[i] = CRGB::Black;
	}
	FastLED.show(); 
}


void placeCar(int carNum) {
	CAR car = cars[carNum];
	int pos = car.position;
	uint8_t vChange = (int)(car.gradientV / CAR_SIZE);
	CHSV colour = CHSV(car.colourH, car.colourS, car.colourV);
	leds[getStripPos(pos)] += colour;
	for(int i = 1; i < CAR_SIZE; i++){
		colour.setHSV(colour.h, colour.s ,colour.v - vChange); 
		leds[getStripPos(pos - i)] += colour;
		Serial.printf("Car: %d ", carNum);
		Serial.printf("POS: %d ", i);
		Serial.printf("Colour: %d ", (int)colour.v);
	}
}

void blankCars(int carNum){
	CAR car = cars[carNum];
	for(int i = 0; i < CAR_SIZE; i++){
		leds[getStripPos(car.position - i)] = CRGB::Black;
	}
}

void race(){
	int laps = 10;

	while (cars[0].position < (laps * NUM_LEDS) || cars[1].position < (laps * NUM_LEDS) )
	{
		//display car 0
		placeCar(0);
		placeCar(1);
		
		FastLED.show();

		blankCars(0);
		blankCars(1);

		cars[0].position ++;
		cars[1].position ++;
		cars[1].position ++;

		delay(CYCLE_SPEED);
	}
}


void loop(){
	blankLeds();
	race();

}
