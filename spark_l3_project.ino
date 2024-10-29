#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

#define DHTPIN 2             // DHT11 data pin connected to digital pin 2
#define DHTTYPE DHT11        // DHT 11 sensor type
#define RELAYPIN 3           // Relay pin connected to digital pin 3

// Initialize the DHT sensor
DHT dht(DHTPIN, DHTTYPE);

// Initialize the LCD with I2C address 0x27 and dimensions (16 columns x 2 rows)
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  lcd.init();            // Initialize the LCD (alternative to lcd.begin())
  lcd.backlight();       // Turn on the backlight
  dht.begin();           // Initialize the DHT sensor

  // Set up the relay pin
  pinMode(RELAYPIN, OUTPUT);
  digitalWrite(RELAYPIN, LOW); // Ensure relay is off initially
}

void loop() {
  // Read temperature and humidity from the DHT11 sensor
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  // Check if reading failed
  if (isnan(humidity) || isnan(temperature)) {
    lcd.setCursor(0, 0);
    lcd.print("Sensor Error!");
    delay(2000); // Wait before retrying
    return;
  }

  // Display temperature and humidity on the LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temperature);
  lcd.print((char)223); // Degree symbol
  lcd.print("C");

  lcd.setCursor(0, 1);
  lcd.print("Humidity: ");
  lcd.print(humidity);
  lcd.print("%");

  // Check conditions for triggering the relay
  if (temperature < 25 && humidity < 60) {
    digitalWrite(RELAYPIN, HIGH); // Turn relay on
    lcd.setCursor(10, 1);         // Display relay status
    lcd.print(" ");
  } else {
    digitalWrite(RELAYPIN, LOW); // Turn relay off
    lcd.setCursor(10, 1);        // Display relay status
    lcd.print("");
  }

  delay(2000); // Update every 2 seconds
}
