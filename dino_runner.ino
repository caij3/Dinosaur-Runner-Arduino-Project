#include <LiquidCrystal.h>

const int rs = 22, en = 24, d4 = 26, d5 = 28, d6 = 30, d7 = 32;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const int buttonPin = 13; // Button connected to pin 13
const int cactusHeight = 5; // Height of the cactus character
const int cactusWidth = 3; // Width of the cactus character

// Define custom characters for the cactus and dinosaur
byte cactusBitmap[cactusHeight] = {
  B01010,
  B11111,
  B01010,
  B11111,
  B01010
};

byte dinosaurBitmap[cactusHeight] = {
  B00100,
  B01110,
  B11111,
  B10101,
  B00100
};

char ground = '-';   // Ground character
char space = ' ';    // Empty space

bool isJumping = false;
bool gameOver = false;
int jumpCounter = 0;
int obstaclePos = 15; // Start obstacle from the rightmost position

unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50; // Debounce time in milliseconds
bool buttonState = HIGH;
bool lastButtonState = HIGH;

void setup() {
  lcd.begin(16, 2);
  Serial.begin(9600); // Start serial communication
  lcd.createChar(0, cactusBitmap); // Define the custom character for the cactus
  lcd.createChar(1, dinosaurBitmap); // Define the custom character for the dinosaur
  lcd.setCursor(0, 0);
  lcd.print("Dino Runner!");
  delay(2000);
  lcd.clear();

  pinMode(buttonPin, INPUT_PULLUP); // Configure button pin as input with pull-up resistor
}

void loop() {
  // Read the state of the button
  bool reading = digitalRead(buttonPin);

  // Debounce the button
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;

      // If the button was pressed
      if (buttonState == LOW) {
        if (gameOver) {
          // Reset the game if it is over
          resetGame();
        } else if (!isJumping) {
          // Initiate jump if not jumping
          isJumping = true;
          jumpCounter = 0;
        }
      }
    }
  }

  lastButtonState = reading;

  if (gameOver) {
    return; // If game over, do not continue the game loop
  }

  // Update jump status
  if (isJumping) {
    jumpCounter++;
    if (jumpCounter > 3) { // Adjust jump duration
      isJumping = false;
    }
  }

  // Update obstacle position
  obstaclePos--;
  if (obstaclePos < 0) {
    obstaclePos = 15; // Reset obstacle position
  }

  // Clear the display
  lcd.clear();

  // Draw the ground
  lcd.setCursor(0, 1);
  for (int i = 0; i < 16; i++) {
    lcd.print(ground);
  }

  // Draw the dinosaur and obstacle
  lcd.setCursor(0, isJumping ? 0 : 1);
  lcd.write(byte(1)); // Display the custom character for the dinosaur

  if (obstaclePos != 0) {
    lcd.setCursor(obstaclePos, 1);
    lcd.write(byte(0)); // Display the custom character for the cactus
  }

  // Check for collision
  if (obstaclePos == 0 && !isJumping) {
    lcd.clear();
    lcd.setCursor(4, 0);
    lcd.print("Game Over!");
    gameOver = true;
  }

  delay(200); // Game speed
}

void resetGame() {
  isJumping = false;
  gameOver = false;
  jumpCounter = 0;
  obstaclePos = 15;
  lcd.clear();
}
