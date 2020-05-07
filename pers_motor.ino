const int stegPin = 7;
const int riktPin = 6;

String state = "";
int currentHeading = 0;
int currentAngle = 0;
int lastAngle = 0;
int angle = 0;
int rotate = 0;
int runAuto = 0;
String mode = "Manual";
boolean riktRotation = HIGH;
int rotHast = 1500;

void setup() {
  // Sätter pin 7 och 6 till output. riktpin bestämmer motorns polaritet, och stegpin att motorn ska rotera.
  pinMode(stegPin, OUTPUT);
  pinMode(riktPin, OUTPUT);
  Serial.begin(38400); // Överföringshastighet för bluetooth
}
void loop() {
  delayMicroseconds(1);
  if (Serial.available() > 0) { // Tittar om det finns data i Serial buffern att läsa in (i bytes)
    state = Serial.readString(); // Läser från Serial port
  }
  // När Auto trycks
  if (mode == "Auto") {
    if (state == "Reverse") {
      delay(10);
      if (riktRotation == HIGH) {
        riktRotation = LOW;
      }
      else {
        riktRotation = HIGH;
      }
      //koden ovan ändrar rotationsriktningen, skiftar high till low och vice versa. (givet att motorn har en ingång för polarisering)
      digitalWrite(riktPin, riktRotation);
      delay(10);
      state = "";
    }
    
    rotHast = state.toInt();
    if (rotHast >= 300 && rotHast <= 3000) {
      digitalWrite(stegPin, HIGH);
      delayMicroseconds(rotHast);
      digitalWrite(stegPin, LOW);
      delayMicroseconds(rotHast);
    }
    else {
      digitalWrite(stegPin, HIGH);
      delayMicroseconds(1500);
      digitalWrite(stegPin, LOW);
      delayMicroseconds(1500);
    }

    if (state == "Manual") {
      mode = state;
    }
  }
  // När appen skickar "Manual"
  else if (mode == "Manual") {
    currentHeading = state.toInt();
  
    if (currentHeading < 0 ) {
      currentHeading = 360 + currentHeading;
    }
    currentAngle = map(currentHeading, 0, 359, 0, 200); //200 pulser för en full cykel
    digitalWrite(riktPin, HIGH); // Gör att motorn kan rotera åt olika håll (separat pin för riktning, kan eventuellt uppnås med ändrad polaritet)
    if (currentAngle != lastAngle) {
      if (currentAngle > lastAngle) {
        rotate = currentAngle - lastAngle;
        for (int x = 0; x < rotate; x++) {
          digitalWrite(stegPin, HIGH);
          delayMicroseconds(500);
          digitalWrite(stegPin, LOW);
          delayMicroseconds(500);
        }
      }
      if (currentAngle < lastAngle) {
        rotate = lastAngle - currentAngle;
        digitalWrite(riktPin, LOW); //Ändrar rotationsriktning
        for (int x = 0; x < rotate; x++) {
          digitalWrite(stegPin, HIGH);
          delayMicroseconds(500);
          digitalWrite(stegPin, LOW);
          delayMicroseconds(500);
        }
      }
    }
    lastAngle = currentAngle;
    if (state == "Auto") {
      mode = state;
    }
  }
}
