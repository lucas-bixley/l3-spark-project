#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define DHTPIN 2                  // Pin for DHT sensor
#define DHTTYPE DHT11             // Define the type of DHT sensor
#define RELAYPIN 6                // Pin for relay

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2); // Initialize I2C LCD

const int buttonPin = 3;         // button pin is digital 3
volatile bool buttonPressed = false;

void setup() {
    Serial.begin(9600);
    pinMode(buttonPin, INPUT_PULLUP); // Set button pin as input with pull-up
    attachInterrupt(digitalPinToInterrupt(buttonPin), buttonReleasedISR, FALLING);// define to when pressed button function
   
    dht.begin();     // initialize dht sensor
    lcd.init();     // initialize lcd
    lcd.noBacklight(); // Turn off the backlight initially
    pinMode(RELAYPIN, OUTPUT); // define pin mode
    digitalWrite(RELAYPIN, LOW); // Ensure relay is off initially

    delay(2000); // Allow sensor stabilization
}

void buttonReleasedISR() {
    buttonPressed = true; // Set flag to indicate button was pressed
}

void loop() {
    // Read humidity and temperature values from DHT sensor
    float humidity = dht.readHumidity();// read values
    float temperature = dht.readTemperature(); // read values

    // Check if readings are valid
    if (isnan(humidity) || isnan(temperature)) { //define when to run program
        lcd.clear(); // clear lcd befor printing new info
        lcd.setCursor(0, 0); // set place toprint
        lcd.print("Sensor Error!"); // print sensore error
        delay(2000); // set 2 second delay
        return; // Exit loop if sensor error
    }

    // Print valid sensor values
    Serial.print("Humidity: ");//print humidity ti serial monitor
    Serial.print(humidity);
    Serial.print(" %, Temperature: ");//print temp to serial monitor
    Serial.print(temperature);
    Serial.println(" *C");//print c for celcius

    // Control relay based on temperature
    if (temperature > 20) {  // Trigger relay if temperature is greater than 20°C
        digitalWrite(RELAYPIN, HIGH); // Turn relay on
    } else {
        digitalWrite(RELAYPIN, LOW); // Turn relay off
    }

    // Check if button was pressed
    if (buttonPressed) {
        buttonPressed = false; // Reset flag

        // Display data on the LCD
        lcd.backlight();
        lcd.clear(); // Clear LCD before displaying new data
        lcd.setCursor(0, 0); // set to first row
        lcd.print("Humidity: "); // print humidity to lcd
        lcd.print(humidity);
        lcd.print(" %"); //print % to lcd

        lcd.setCursor(0, 1); // set to second row
        lcd.print("Temp: "); // print temp to lcd
        lcd.print(temperature);
        lcd.print(" C"); //print c for celcius t lcd

        delay(10000); // Keep data displayed for 10 seconds
        lcd.noBacklight(); // Turn off backlight after displaying
        lcd.clear(); // clear lcd
    }

    delay(1000); // Short delay to avoid flooding the loop
}
