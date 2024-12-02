#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

// Ultrasonic Sensor Pins
const int trigPin = 5;
const int echoPin = 18;

// Sound speed in cm/uS
#define SOUND_SPEED 0.034

// Initialize the OLED display
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

long duration;
float distanceCm;

void setup() {
  // Initialize serial communication
  Serial.begin(115200);

  // Initialize I2C communication with specific pins
  Wire.begin(21, 22);  // SDA, SCL

  // Initialize OLED display
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Initialize Ultrasonic Sensor Pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Clear the display
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
}

void loop() {
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Reads the echoPin, with a timeout to prevent hanging
  duration = pulseIn(echoPin, HIGH, 30000); // 30000 microseconds = 30 ms timeout
  
  // Calculate the distance
  distanceCm = duration * SOUND_SPEED/2;
  
  // More aggressive filtering
  if (duration == 0 || distanceCm < 2.0 || distanceCm > 400.0) {
    distanceCm = 0.0;  // Invalid or out-of-range measurement
  }
  
  // Debugging output
  Serial.print("Raw Duration: ");
  Serial.print(duration);
  Serial.print(" Distance: ");
  Serial.println(distanceCm);
  
  // Clear the display
  display.clearDisplay();
  
  // Set text size and center alignment
  display.setTextSize(2);
  int16_t x1, y1;
  uint16_t width, height;
  display.getTextBounds("Distance:", 0, 0, &x1, &y1, &width, &height);
  display.setCursor(
    (display.width() - width) / 2,
    (display.height() - height) / 2
  );
  
  // Print distance
  display.print(distanceCm, 1);  // Print with 1 decimal place
  display.println(" cm");
  
  // Update the display
  display.display();
  
  // Small delay to prevent overwhelming the display
  delay(100);
}