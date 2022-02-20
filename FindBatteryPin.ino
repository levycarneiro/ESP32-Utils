
// This sketch will help you find which pin in your Arduino/ESP32 board provides USB / Battery voltage level.
// v0.1 - 2022-02-19
// LC / levy@levycarneiro.com

// Enter all the available pins of your board in the array, and adjust the array size.
const int max_pins = 16;
int pins[max_pins] = {0, 2, 4, 5, 18, 19, 21, 22, 23, 25, 26, 27, 32, 33, 34, 35};    // TTGO T7 v1.5

void setup() {

  Serial.begin (115200);
  Serial.println("");
  Serial.println("Detecting Voltage Pin...");
  Serial.println("");

  for (int i=0; i<max_pins; i++) {
    int pin = pins[i];
    pinMode (pin, INPUT);
  }

  // Checking the voltage level in all Pins to see if any of them are within the expected range for a Voltage pin.

  for (int i=0; i<max_pins; i++) {

    int pin = pins[i];
    Serial.print("Pin: " + String(pin) + "\t");
    float batt_vol = batt_read_voltage_simple (pin);
    Serial.print("Level: " + String(batt_vol));

    // The check below assumes you are running this sketch while powering your board via USB
    // (which would indicate a little more than 4 volts on the Voltage pin),
    // or with a battery with at least 3v. Adjust if needed.
    // On a TTGO T7 v1.5, only pin 35 shows within the 3-5v range with 4.07v, 
    // so probably the Voltage pin. I will confirm this when I attach a battery to it, 
    // and connect a display to display the results.

    if (batt_vol > 3 && batt_vol < 5) {                 
      Serial.print("\tWithin expected range for USB power / battery power.");
    }
    Serial.println("");
  }
}

void loop() {
}

float batt_read_voltage_simple (int batt_pin) {

  const int batt_numReadings = 32;          // the higher the value, the smoother the average
  int batt_readings[batt_numReadings];      // the readings from the analog input

  int batt_total = 0;
  for (int thisReading = 0; thisReading < batt_numReadings; thisReading++) { //used for smoothening
    batt_readings[thisReading] = analogRead(batt_pin);
    batt_total = batt_total + batt_readings[thisReading];
    delay (20);
  }
  float batt_avg = batt_total / batt_numReadings;
  float batt_voltage = (float)batt_avg / 4095*2*3.3*1.1;    // Formula from https://gist.github.com/jenschr/dfc765cb9404beb6333a8ea30d2e78a1

  return batt_voltage;
}
