// LED pins
const int rLED = 2;
const int yLED = 3;
const int gLED = 4;
const int bLED = 5;

// Button pins
const int rButton = A3;
const int yButton = A2;
const int gButton = A1;
const int bButton = A0;
const int startButton = A4;

// Loop cycle for button press before timeout
int waitTime = 2000;

// Track whether the game is active
bool gameActive = false;

// The setup function runs once when board is reset or powered
void setup() {
  // Set up LEDs
  pinMode(rLED, OUTPUT);
  pinMode(yLED, OUTPUT);
  pinMode(gLED, OUTPUT);
  pinMode(bLED, OUTPUT);

  // Set up input buttons
  pinMode(rButton, INPUT);
  pinMode(yButton, INPUT);
  pinMode(gButton, INPUT);
  pinMode(bButton, INPUT);
  pinMode(startButton, INPUT);

  // Initialize serial communication
  Serial.begin(9600);
  randomSeed(analogRead(0));
}

// The loop function runs over and over again forever
void loop() {
  // Check if the game is inactive and the start button is pressed
  if (!gameActive) {
    if (digitalRead(startButton) == LOW) {
      // Start a new game
      Serial.println("Game started!");
      gameActive = true;
      waitTime = 2000;  // Reset the wait time for a new game
      delay(500);       // Debounce delay
    } else {
      Serial.println("Press the start button to begin...");
      delay(100);
      return;
    }
  }

  // Choose a new LED to light up
  int newColor = pickNewColor();
  ledOn(newColor);

  if (isWhacked(newColor)) {
    Serial.println("You whacked the mole!");
    ledOff(newColor);
    delay(500);  // Pause briefly before showing a new mole
    waitTime *= 0.9;
  } else {
    Serial.println("You failed to whack the mole. Game over!");
    gameover();
    gameActive = false;  // Deactivate the game after game over
  }
}

// Function to check if it is whacked
boolean isWhacked(int newColor) {
  int i = 0;
  int checkButton;
  boolean whacked = false;
  boolean buttonPressed = false;

  Serial.print("Wait Time: ");
  Serial.print(waitTime);

  while ((i < waitTime) && (!buttonPressed)) {
    checkButton = isButtonPressed();   // 0 if nothing is pressed
    if (newColor == checkButton) {     // Correct button is pressed
      whacked = true;
      buttonPressed = true;
    } else if (checkButton > 0) {      // Incorrect button is pressed
      whacked = false;
      buttonPressed = true;
    }
    i++;
    delay(1);
  }

  if (buttonPressed) {
    Serial.println("..button detected.");
  } else {
    Serial.println("..time out");
  }

  return whacked;
}

// Function that monitors all buttons and returns an integer
// 0 = nothing pressed, 2-5 = button pressed
int isButtonPressed() {
  int buttonPressed = 0;

  if (digitalRead(rButton) == LOW) {
    ledOn(rLED);
    buttonPressed = rLED;
  }
  if (digitalRead(bButton) == LOW) {
    ledOn(bLED);
    buttonPressed = bLED;
  }
  if (digitalRead(yButton) == LOW) {
    ledOn(yLED);
    buttonPressed = yLED;
  }
  if (digitalRead(gButton) == LOW) {
    ledOn(gLED);
    buttonPressed = gLED;
  }

  return buttonPressed;
}

// Function for when the game is over
void gameover() {
  // Flash all the LEDs on and off thrice
  for (int i = 0; i <= 2; i++) {
    ledOn(rLED);
    ledOn(bLED);
    ledOn(yLED);
    ledOn(gLED);
    delay(200);
    
    ledOff(rLED);
    ledOff(bLED);
    ledOff(yLED);
    ledOff(gLED);
    delay(200);
  }
  
  delay(2000);
}

// Function to randomly pick a new color
int pickNewColor() {
  // Generate random from 2 to 5 that matches LED pins
  return random(2, 6);  
}

// Function to turn on a specified LED
void ledOn(int colorON) {
  digitalWrite(colorON, HIGH);
}

// Function to turn off a specified LED
void ledOff(int colorOFF) {
  digitalWrite(colorOFF, LOW);
}