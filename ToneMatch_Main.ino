/*
 * Exercise - Digi-Physi Musical Instrument

  Tone Match game!
  
  The game is simple, you need to match the given tone to win.
  Moving the joystick plays a different tone, and when pressing on the joystick it plays the winning sound.
  The closer you are to the winning tone, the brighter the light.
  If you are on the corret tone, pressing the joystick wins the game.
  When you win, the light starts flashing until you cover the light sensor and make a loud noise (received by the sound sensor), and then the game is restarted.

  The circuit:
  * INPUTS:
  *     Joystick
  *     Sound receiver
  *     Light receiver
  * OUTPUTS:     
  *     Speaker
  *     Light bulb

  Video link: https://youtu.be/7ftjWcbVuFA?si=QlNHWxdWnDBPtAW4
  Created By:
  Theodore_Dai_Maman #211541594
  Omer_Dan #322952466
  Amit_Kaminsky #207487661
*/ 


#include "pitches.h"

// SPEAKER
#define TONE_OUTPUT_PIN 25

// Rest pitch
#define REST 0

// SOUND SENSOR
#define SOUND_SENSOR_PIN 36

// JOYSTICK
#define JOYSTICK_X_PIN 34
#define JOYSTICK_Y_PIN 35
#define JOYSTICK_SW_PIN 32

// LED PIN
#define LEDPIN 13

// LIGHT SENSOR
#define LIGHT_SENSOR_PIN 15
#define DARK_LIGHT_LEVEL 2000
#define VOLUME_SOUND_LEVEL 3600

int sw = 0;
const int TONE_PWM_CHANNEL = 0;
int rowDistance = 0;
int colDistance = 0;
int totalDistance = 0;
int fadingValue = 0;

// Notes for win melody
int winningMelody[] = {
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};

int noteDurations[] = {
  4, 8, 8, 4, 4, 4, 4, 4
};

// A 3x3 grid of sounds (
const int soundGrid[3][3] = {
    {NOTE_C4, NOTE_D4, NOTE_E4},
    {NOTE_F4, NOTE_G4, NOTE_A4},
    {NOTE_B4, NOTE_C5, NOTE_D5}
}; 

const int rangeX = 4096 / 3; // Divide joystick x into 3 regions (octaves)
const int rangeY = 4096 / 3; // Divide joystick y into 3 regions (notes)

int destinationRow = 0; // Random row index
int destinationCol = 0; // Random column index

int currentRow = 0; // Current joystick row index
int currentCol = 0; // Current joystick column index

// ##############################################
// ##############################################
// ##############################################
// ##############################################
// ##############################################
// ##############################################

// change this to make the song slower or faster
int tempo = 114;

// change this to whichever pin you want to use
int buzzer = 11;

// notes of the moledy followed by the duration.
// a 4 means a quarter note, 8 an eighteenth , 16 sixteenth, so on
// !!negative numbers are used to represent dotted notes,
// so -4 means a dotted quarter note, that is, a quarter plus an eighteenth!!
int melody[] = {

  // Never Gonna Give You Up - Rick Astley
  // Score available at https://musescore.com/chlorondria_5/never-gonna-give-you-up_alto-sax
  // Arranged by Chlorondria
   
  //  NOTE_D5,2, NOTE_E5,8, NOTE_FS5,8, NOTE_D5,8, //13
  //  NOTE_E5,8, NOTE_E5,8, NOTE_E5,8, NOTE_FS5,8, NOTE_E5,4, NOTE_A4,4,
  //  REST,2, NOTE_B4,8, NOTE_CS5,8, NOTE_D5,8, NOTE_B4,8,
  //  REST,8, NOTE_E5,8, 
  NOTE_FS5,8, NOTE_E5,-4,
  NOTE_A4,16, NOTE_B4,16, NOTE_D5,16, NOTE_B4,16,
  NOTE_FS5,-8, NOTE_FS5,-8, NOTE_E5,-4, NOTE_A4,16, NOTE_B4,16, NOTE_D5,16, NOTE_B4,16,

  NOTE_E5,-8, NOTE_E5,-8, NOTE_D5,-8, NOTE_CS5,16, NOTE_B4,-8, NOTE_A4,16, NOTE_B4,16, NOTE_D5,16, NOTE_B4,16, //18
  NOTE_D5,4, NOTE_E5,8, NOTE_CS5,-8, NOTE_B4,16, NOTE_A4,8, NOTE_A4,8, NOTE_A4,8, 
  NOTE_E5,4, NOTE_D5,2, NOTE_A4,16, NOTE_B4,16, NOTE_D5,16, NOTE_B4,16,
  NOTE_FS5,-8, NOTE_FS5,-8, NOTE_E5,-4, NOTE_A4,16, NOTE_B4,16, NOTE_D5,16, NOTE_B4,16,
  NOTE_A5,4, NOTE_CS5,8, NOTE_D5,-8, NOTE_CS5,16, NOTE_B4,8, NOTE_A4,16, NOTE_B4,16, NOTE_D5,16, NOTE_B4,16,

  NOTE_D5,4, NOTE_E5,8, NOTE_CS5,-8, NOTE_B4,16, NOTE_A4,4, NOTE_A4,8,  //23
  NOTE_E5,4, NOTE_D5,2, REST,4,
  REST,8, NOTE_B4,8, NOTE_D5,8, NOTE_B4,8, NOTE_D5,8, NOTE_E5,4, REST,8,
  REST,8, NOTE_CS5,8, NOTE_B4,8, NOTE_A4,-4, REST,4,
  REST,8, NOTE_B4,8, NOTE_B4,8, NOTE_CS5,8, NOTE_D5,8, NOTE_B4,8, NOTE_A4,4,
  REST,8, NOTE_A5,8, NOTE_A5,8, NOTE_E5,8, NOTE_FS5,8, NOTE_E5,8, NOTE_D5,8,
  
  REST,8, NOTE_A4,8, NOTE_B4,8, NOTE_CS5,8, NOTE_D5,8, NOTE_B4,8, //29
  REST,8, NOTE_CS5,8, NOTE_B4,8, NOTE_A4,-4, REST,4,
  NOTE_B4,8, NOTE_B4,8, NOTE_CS5,8, NOTE_D5,8, NOTE_B4,8, NOTE_A4,4, REST,8,
  REST,8, NOTE_E5,8, NOTE_E5,8, NOTE_FS5,4, NOTE_E5,-4, 
  NOTE_D5,2, NOTE_D5,8, NOTE_E5,8, NOTE_FS5,8, NOTE_E5,4, 
  NOTE_E5,8, NOTE_E5,8, NOTE_FS5,8, NOTE_E5,8, NOTE_A4,8, NOTE_A4,4,

  REST,-4, NOTE_A4,8, NOTE_B4,8, NOTE_CS5,8, NOTE_D5,8, NOTE_B4,8, //35
  REST,8, NOTE_E5,8, NOTE_FS5,8, NOTE_E5,-4, NOTE_A4,16, NOTE_B4,16, NOTE_D5,16, NOTE_B4,16,
  NOTE_FS5,-8, NOTE_FS5,-8, NOTE_E5,-4, NOTE_A4,16, NOTE_B4,16, NOTE_D5,16, NOTE_B4,16,
  NOTE_E5,-8, NOTE_E5,-8, NOTE_D5,-8, NOTE_CS5,16, NOTE_B4,8, NOTE_A4,16, NOTE_B4,16, NOTE_D5,16, NOTE_B4,16,
  NOTE_D5,4, NOTE_E5,8, NOTE_CS5,-8, NOTE_B4,16, NOTE_A4,4, NOTE_A4,8, 

   NOTE_E5,4, NOTE_D5,2, NOTE_A4,16, NOTE_B4,16, NOTE_D5,16, NOTE_B4,16, //40
  NOTE_FS5,-8, NOTE_FS5,-8, NOTE_E5,-4, NOTE_A4,16, NOTE_B4,16, NOTE_D5,16, NOTE_B4,16,
  NOTE_A5,4, NOTE_CS5,8, NOTE_D5,-8, NOTE_CS5,16, NOTE_B4,8, NOTE_A4,16, NOTE_B4,16, NOTE_D5,16, NOTE_B4,16,
  NOTE_D5,4, NOTE_E5,8, NOTE_CS5,-8, NOTE_B4,16, NOTE_A4,4, NOTE_A4,8,  
  NOTE_E5,4, NOTE_D5,2, NOTE_A4,16, NOTE_B4,16, NOTE_D5,16, NOTE_B4,16,
   
  NOTE_FS5,-8, NOTE_FS5,-8, NOTE_E5,-4, NOTE_A4,16, NOTE_B4,16, NOTE_D5,16, NOTE_B4,16, //45
  NOTE_A5,4, NOTE_CS5,8, NOTE_D5,-8, NOTE_CS5,16, NOTE_B4,8, NOTE_A4,16, NOTE_B4,16, NOTE_D5,16, NOTE_B4,16,
  NOTE_D5,4, NOTE_E5,8, NOTE_CS5,-8, NOTE_B4,16, NOTE_A4,4, NOTE_A4,8,  
  NOTE_E5,4, NOTE_D5,2, NOTE_A4,16, NOTE_B4,16, NOTE_D5,16, NOTE_B4,16,
  NOTE_FS5,-8, NOTE_FS5,-8, NOTE_E5,-4, NOTE_A4,16, NOTE_B4,16, NOTE_D5,16, NOTE_B4,16, //45
  
  NOTE_A5,4, NOTE_CS5,8, NOTE_D5,-8, NOTE_CS5,16, NOTE_B4,8, NOTE_A4,16, NOTE_B4,16, NOTE_D5,16, NOTE_B4,16,
  NOTE_D5,4, NOTE_E5,8, NOTE_CS5,-8, NOTE_B4,16, NOTE_A4,4, NOTE_A4,8, 

  NOTE_E5,4, NOTE_D5,2, REST,4
};

// sizeof gives the number of bytes, each int value is composed of two bytes (16 bits)
// there are two values per note (pitch and duration), so for each note there are four bytes
int notes = sizeof(melody) / sizeof(melody[0]) / 2;

// this calculates the duration of a whole note in ms
int wholenote = (60000 * 4) / tempo;

int divider = 0, noteDuration = 0;

// ##############################################
// ##############################################
// ##############################################
// ##############################################

void setup() {
    Serial.begin(115200);

    // Speaker setup
    ledcAttachPin(TONE_OUTPUT_PIN, 0);

    //Light Sensor setup
    pinMode(LIGHT_SENSOR_PIN, INPUT);

    // Joystick setup
    pinMode(JOYSTICK_SW_PIN, INPUT_PULLUP);

    // Random destination
    randomSeed(analogRead(0)); // Make the random selection different each program run
    destinationRow = random(0, 3); // Random row (0-2)
    destinationCol = random(0, 3); // Random column (0-2)
    
    // (1,1) is defined to be silent
    while (destinationRow == 1 && destinationCol == 1) {
        destinationRow = random(0, 3); // Random row (0-2)
        destinationCol = random(0, 3); // Random column (0-2)
    }

    Serial.printf("Destination sound: Row %d, Col %d\n", destinationRow, destinationCol);
    
    delay(500);
}

void loop() {
    // Read joystick positions
    int x = analogRead(JOYSTICK_X_PIN);
    int y = analogRead(JOYSTICK_Y_PIN);
    sw = digitalRead(JOYSTICK_SW_PIN);

    // Map joystick positions to grid
    currentRow = constrain(x / rangeX, 0, 2); // Map x to 0-2 for rows
    currentCol = constrain(y / rangeY, 0, 2); // Map y to 0-2 for columns

    // Fading effect
    // Calculate distance from the destination
    rowDistance = abs(destinationRow - currentRow);
    colDistance = abs(destinationCol - currentCol);
    totalDistance = rowDistance + colDistance; // Simple distance calc

    // Adjust fading based on distance
    fadingValue = 255 - (totalDistance * 60); // Scale distance to fading value
    fadingValue = constrain(fadingValue, 0, 255); // Ensure within valid range
    analogWrite(LEDPIN, fadingValue);
    
    // Debugging info
    Serial.printf("Joystick x: %d, y: %d, \n\n Current: Row %d, Col %d, \nDestination: Row %d, Col %d\n",
                  x, y, currentRow, currentCol, destinationRow, destinationCol);
    
     // Check if joystick button is pressed and the sound matches the destination
    if (sw == 0 && currentRow == destinationRow && currentCol == destinationCol) {
        Serial.println("You reached the destination sound!");
        WonGame();
    }
    
    // Play the current sound if not (1, 1)
    else if (sw == 0) {
        ledcWriteTone(0, soundGrid[destinationRow][destinationCol]);
        delay(1000);
        
    } else if (currentRow == 1 && currentCol == 1) {
        ledcWriteTone(0, 0);
    
    // Play current tone
    } else {
      ledcWriteTone(0, soundGrid[currentRow][currentCol]);
    }

    delay(100); // Small delay for stability
}

void WonGame()
{

  for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2) {
    
    // calculates the duration of each note
    divider = melody[thisNote + 1];
    if (divider > 0) {
      // regular note, just proceed
      noteDuration = (wholenote) / divider;
    } else if (divider < 0) {
      // dotted notes are represented with negative durations!!
      noteDuration = (wholenote) / abs(divider);
      noteDuration *= 1.5; // increases the duration in half for dotted notes
    }

    analogWrite(LEDPIN, 255);
  
    // we only play the note for 90% of the duration, leaving 10% as a pause
    ledcWriteTone(TONE_PWM_CHANNEL, melody[thisNote]);
    
    // Wait for 90% of the note duration before stopping
    delay(noteDuration * 0.9);

    analogWrite(LEDPIN, 0);
    
    // Stop the tone by setting the frequency to 0
    ledcWriteTone(TONE_PWM_CHANNEL, 0);
    
    // Add a short delay to create a pause between notes
    delay(noteDuration * 0.1);

    // If joystick pressed - end song
    sw = digitalRead(JOYSTICK_SW_PIN);
    if (sw == 0) {
        break;
    }
  }

    ledcWriteTone(TONE_PWM_CHANNEL, 0); // Ensure silence
    RestartGame();
}


void RestartGame()
{
  // Game is won, waiting for sound and light sensors to restart
  while(true)
  {
    int volume = analogRead(SOUND_SENSOR_PIN);
    int light = analogRead(LIGHT_SENSOR_PIN);

    analogWrite(LEDPIN, 0);   // turn the LED on (HIGH is the voltage level)
    Serial.println("LEDPIN OFF");

    delay(20);
    Serial.println("DELAY 20");

    analogWrite(LEDPIN, 255);    // turn the LED off by making the voltage LOW
    Serial.println("LEDPIN ON");

    // Debugging
    Serial.printf("Volume: %d\nLight %d\n", volume, light);

    if (volume >= VOLUME_SOUND_LEVEL && light > DARK_LIGHT_LEVEL) 
    {
      Serial.printf("Pass values -- Volume: %d\nLight %d\n", volume, light);
      break;
    }
    delay(20);
    Serial.println("DELAY 20");

  }
  
  // Create new winning coordiinates
  destinationRow = random(0, 3); // Random row (0-2)
  destinationCol = random(0, 3); // Random column (0-2)
  // (1,1) is defined to be silent
  while (destinationRow == 1 && destinationCol == 1) {
      destinationRow = random(0, 3); // Random row (0-2)
      destinationCol = random(0, 3); // Random column (0-2)
  }
  loop();
}
