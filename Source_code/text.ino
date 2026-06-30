#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3); // RX, TX

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

int relayPin = 7;
int buzzer = 8;

void setup() {
  Serial.begin(9600);
  mySerial.begin(57600);

  pinMode(relayPin, OUTPUT);
  pinMode(buzzer, OUTPUT);

  digitalWrite(relayPin, HIGH); // lock closed

  Serial.println("Fingerprint Door Lock System");

  finger.begin(57600);

  if (finger.verifyPassword()) {
    Serial.println("Sensor Found!");
  } else {
    Serial.println("Sensor Not Found :(");
    while (1);
  }
}

void loop() {
  getFingerprintID();
  delay(50);
}

uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK) return p;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK) return p;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK) {
    Serial.println("Access Denied");
    digitalWrite(buzzer, HIGH);
    delay(1000);
    digitalWrite(buzzer, LOW);
    return p;
  }

  Serial.println("Access Granted!");
  Serial.print("ID: ");
  Serial.println(finger.fingerID);

  openDoor();
  return finger.fingerID;
}

void openDoor() {
  digitalWrite(relayPin, LOW);  // unlock
  delay(5000);                  // door open time
  digitalWrite(relayPin, HIGH); // lock again
}
 

