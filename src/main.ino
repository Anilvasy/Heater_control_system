#include <DHT.h>
#include<Wire.h>
#include <LiquidCrystal_I2C.h>

//pin defined
#define LED1  27//red
#define LED2  26//yellow
#define LED3  25//orage
#define LED4  33//white
#define BUZ  16
#define BUTTON 34 

#define DHTPIN 14       // GPIO pin connected to the DHT sensor
#define DHTTYPE DHT22 // DHT Sensor type

//Temperture Thresholds
#define T_Target 50
#define T_Overheat 70
#define TEMP_IDLE 30


DHT dht(DHTPIN, DHTTYPE); 
LiquidCrystal_I2C lcd(0x27, 16, 2); 

volatile bool ButtonMode = false;
unsigned long manualStartTime = 0;

// Interrupt Service Routine: triggered on button press
void IRAM_ATTR toggleModeISR() {
  ButtonMode=true;
  manualStartTime = millis(); 
}


void setup() {
  Serial.begin(115200);
  dht.begin();
  Serial.println("Heater Control System");
  Serial.println("Let start.....!");
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(BUZ, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP); 

  // Attach interrupt on button pin (rising/falling)
  attachInterrupt(digitalPinToInterrupt(BUTTON), toggleModeISR, CHANGE);

  // Initialize LCD
  lcd.init();    
  lcd.backlight(); 
  lcd.clear();
  
}

void loop() {
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();

  // Check for sensor read errors
  if (isnan(temp) || isnan(hum)) {
    Serial.println("Failed to read from DHT sensor!");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Sensor Error");
    return;
  }

  //Print readings on Serial Monitor
  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.print(" °C\tHumidity: ");
  Serial.print(hum);
  Serial.println(" %");
  Serial.print("state of button:");
  Serial.println(ButtonMode);


  //Display readings on LCD
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temp);
  lcd.print(" C");

  lcd.setCursor(0, 1);
  lcd.print("Hum: ");
  lcd.print(hum);
  lcd.print(" %");

  if (ButtonMode) {
    Serial.println("Manual Mode: Heater ON");
    lcd.clear();
    lcd.setCursor(1,0);
    lcd.print("System In");
    lcd.setCursor(1,1);
    lcd.print("Cooling...");
    // Turn everything OFF
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);
    digitalWrite(LED4, LOW);
    noTone(BUZ);
    delay(500);
    if (temp<T_Overheat || millis() - manualStartTime > 10000) {
      lcd.clear();
      ButtonMode = false;
      Serial.println("Switched back to AUTO Mode");
    }
  }
  else {
    //normal mode
    if (temp >= T_Overheat) {
      // Overheat
      digitalWrite(LED1, HIGH);//red
      digitalWrite(LED2, LOW);
      digitalWrite(LED3, LOW);
      digitalWrite(LED4, LOW);
      tone(BUZ, 1000);
      delay(200);
      noTone(BUZ);
      delay(200);
      Serial.println("Overheat!");
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print(" Over heat!");
      lcd.setCursor(0,1);
      lcd.print("push the button");
      delay(200);
    }
    else if (temp >= T_Target && temp < T_Overheat) {
    // Target Reached
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, HIGH);//yellow
    digitalWrite(LED3, LOW);
    digitalWrite(LED4, LOW);
    noTone(BUZ);
    Serial.println("Target Reached");
    }
    else if (temp >= TEMP_IDLE && temp < T_Target) {
     // Heating
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, HIGH);//orage
    digitalWrite(LED4, LOW);
    noTone(BUZ);
    Serial.println("Heating...");
    }
    else {
    // Idle
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);
    digitalWrite(LED4, HIGH);//white
    noTone(BUZ);
    Serial.println("Idle");
    }
  }  
  delay(500);
}
