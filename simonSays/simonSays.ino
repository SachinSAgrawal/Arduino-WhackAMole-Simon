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

// Other variables
const int levelMax = 32;
int sequence[levelMax];
int sequenceInput[levelMax];
int level = 1;
int velocity = 1000;
bool gameActive = false; // Track if the game is in progress

// The setup function runs once when board is reset or powered
void setup() {
  Serial.begin(9600);  // Initialize serial communication

  // Set up input buttons
  pinMode(rButton, INPUT);
  pinMode(yButton, INPUT);
  pinMode(gButton, INPUT);
  pinMode(bButton, INPUT);

  // Set up LEDs
  pinMode(rLED, OUTPUT);
  pinMode(yLED, OUTPUT);
  pinMode(gLED, OUTPUT);
  pinMode(bLED, OUTPUT);

  // Turn off LEDs at start
  digitalWrite(rLED, LOW);
  digitalWrite(yLED, LOW);
  digitalWrite(gLED, LOW);
  digitalWrite(bLED, LOW);
}

// The loop function runs over and over again forever
void loop() {
  if (!gameActive) {
    if (digitalRead(startButton) == LOW) {
      // Start a new game
      gameActive = true;
      level = 1;  // Reset level
      velocity = 1000;  // Reset velocity
      Serial.println("Game started!");
      Serial.print("Level: ");
      Serial.println(level);

      // Generate a new sequence
      generateSequence(); 
    } else {
      Serial.println("Press the start button to begin...");
      delay(100);
      return;
    }
  }
  
  if (gameActive) {
    showSequence();    // Show the sequence
    getSequence();     // Wait for player input
  }

  // Add a small delay to debounce the start button to prevent multiple presses
  delay(100); 
}

void showSequence() {
  // Turn off LEDs initially
  digitalWrite(rLED, LOW);
  digitalWrite(yLED, LOW);
  digitalWrite(gLED, LOW);
  digitalWrite(bLED, LOW);

  // Display each LED in the sequence
  for (int i = 0; i < level; i++) {
    digitalWrite(sequence[i], HIGH);
    delay(velocity);
    digitalWrite(sequence[i], LOW);
    delay(200);
  }
}

void getSequence() {
  int flag = 0; // Flag to indicate if the sequence is correct

  for (int i = 0; i < level; i++) {
    flag = 0;

    // Loop until correct input is detected
    while (flag == 0 && gameActive) {
      if (digitalRead(bButton) == LOW) {
        processInput(bLED, i, flag);
      } else if (digitalRead(gButton) == LOW) {
        processInput(gLED, i, flag);
      } else if (digitalRead(yButton) == LOW) {
        processInput(yLED, i, flag);
      } else if (digitalRead(rButton) == LOW) {
        processInput(rLED, i, flag);
      }
    }
  }

  rightSequence();
}

void processInput(int led, int index, int &flag) {
  digitalWrite(led, HIGH);
  sequenceInput[index] = led;
  flag = 1;
  delay(200);

  if (sequenceInput[index] != sequence[index]) {
    wrongSequence();
    return;
  }

  digitalWrite(led, LOW);
}

void generateSequence() {
  randomSeed(millis()); // Generate a random sequence

  for (int i = 0; i < levelMax; i++) {
    sequence[i] = random(2, 6);
  }
}

void wrongSequence() {
  Serial.println("Game over!");

  // Blink all LEDs three times to indicate incorrect sequence
  for (int i = 0; i < 3; i++) {
    digitalWrite(rLED, HIGH);
    digitalWrite(yLED, HIGH);
    digitalWrite(gLED, HIGH);
    digitalWrite(bLED, HIGH);
    delay(250);

    digitalWrite(rLED, LOW);
    digitalWrite(yLED, LOW);
    digitalWrite(gLED, LOW);
    digitalWrite(bLED, LOW);
    delay(250);
  }

  // Game over, reset game state
  gameActive = false;

  // Reset sequences
  for (int i = 0; i < levelMax; i++) {
    sequence[i] = 0;
    sequenceInput[i] = 0; 
  }
}

void rightSequence() {
  if (!gameActive) {
    return;
  }

  // Short flash to indicate correct sequence
  digitalWrite(rLED, HIGH);
  digitalWrite(yLED, HIGH);
  digitalWrite(gLED, HIGH);
  digitalWrite(bLED, HIGH);
  delay(500);

  digitalWrite(rLED, LOW);
  digitalWrite(yLED, LOW);
  digitalWrite(gLED, LOW);
  digitalWrite(bLED, LOW);
  delay(500);

  // Increase level and decrease delay to increase difficulty
  if (level < levelMax) {
    level++;
    Serial.println("Correct!");
    Serial.print("Level: ");
    Serial.println(level);
  }
  velocity -= 50;
}