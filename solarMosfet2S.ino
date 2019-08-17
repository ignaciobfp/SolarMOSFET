//Pin definitions
const int batteryPin = A7;
const int ledPin = 13;
const int mosfetGatePin = 4;

//Auxiliary values
const float refVoltage = 4.97;
const float vDividerFactor = 2.0;
const float batMaxVoltage = 8.6; //2s LiPo battery, i.e. 2*4.25V. Use sightly lower values to preserve battery heatlh.
const float batThresholdVoltage = 8.3; //If battery falls below this voltage and some time passed since last charge, we start charging again
const float batMinVoltage = 7.9; //If battery falls below this voltage, we always turn on charge
const float maxCycles = 60;
const int secondsBetweenChecks = 1;

//Global definitions
float vBat = 0.0;
unsigned int cyclesSinceLastCharge = 0;
bool isCharging = false;


inline void checkVBat() {
  vBat = (float) analogRead(batteryPin) * refVoltage * vDividerFactor / 1023.0;
}

inline void startSolarCharge() {
  if (!isCharging) {
    digitalWrite(mosfetGatePin, HIGH);
    digitalWrite(ledPin, HIGH);
    cyclesSinceLastCharge = 0;
    isCharging = true;
  }
}

inline void stopSolarCharge() {
  //We do not check if isCharging is true here, because we want to stop always and as soon as possible
  digitalWrite(mosfetGatePin, LOW);
  digitalWrite(ledPin, LOW);
  cyclesSinceLastCharge = 0;
  isCharging = false;
}

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(mosfetGatePin, OUTPUT);
  stopSolarCharge();
  Serial.begin(9600);
  Serial.println("Init OK");
}

void loop() {
  checkVBat();
  Serial.print("Batt: ");
  Serial.print(vBat, 2);
  Serial.println();
  if (vBat > batMaxVoltage) {
    stopSolarCharge();
  } else if (vBat < batThresholdVoltage && cyclesSinceLastCharge > maxCycles) {
    startSolarCharge();
  } else if (vBat < batMinVoltage) {
    startSolarCharge();
  }
  //Note there is a few edge cases that fall outside the prior conditional block.
  //That are precisely the cases in which we want to stay in the same state, so we do nothing

  if (!isCharging)
    ++cyclesSinceLastCharge;

  delay(1000 * secondsBetweenChecks);
}
