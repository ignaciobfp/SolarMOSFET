//Pin definitions
const int batteryPin = A7;
const int ledPin = 13;
const int mosfetGatePin = 4;

//Auxiliary values
const float refVoltage = 4.86;
const float vDividerFactor = 2.0;
const float batMaxVoltage = 8.4; //2s LiPo battery, i.e. 2*4.2V. Use sightly lower values to preserve battery heatlh.
const float batMinVoltage = 7.9; //If battery falls below this voltage, we always turn on charge
const int secondsBetweenChecks = 0.25;

//Global definitions
float vBat = 0.0;
bool isCharging = false;


inline void checkVBat() {
  vBat = (float) analogRead(batteryPin) * refVoltage * vDividerFactor / 1023.0;
}

inline void startSolarCharge() {
  if (!isCharging) {
    digitalWrite(mosfetGatePin, HIGH);
    digitalWrite(ledPin, HIGH);
    isCharging = true;
  }
}

inline void stopSolarCharge() {
  //We do not check if isCharging is true here, because we want to stop always and as soon as possible
  digitalWrite(mosfetGatePin, LOW);
  digitalWrite(ledPin, LOW);
  isCharging = false;
}

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(mosfetGatePin, OUTPUT);
  stopSolarCharge();
  //Serial.begin(9600);
  //Serial.println("Init OK");
}

void loop() {
  checkVBat();
  //Serial.print("Batt: ");
  //Serial.print(vBat, 2);
  //Serial.println();
  if (vBat > batMaxVoltage) {
    stopSolarCharge();
  } else if (vBat < batMinVoltage) {
    startSolarCharge();
  }
  delay(1000 * secondsBetweenChecks);
}
