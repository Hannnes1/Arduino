#include <Servo.h>

Servo servoLeft;
Servo servoRight;
int oldLeftVelocity = 0;
int oldRightVelocity = 0;
int accelDuration;
int accelSteps;

//----------Accelerera och kör hjul----------
void Drive(int leftVelocity, int rightVelocity, int duration) {

  accelDuration = 1000; //Tid som accelerationen pågår
  if (duration < accelDuration) {
    accelDuration = duration; //Snabba upp acceleration om den valda tiden är för kort
  }
  accelSteps = 20; //Antal accelerationssteg

  //Hastightetsökning per acceletationssteg
  float leftAccelStep = ((1500.00 + leftVelocity) - (1500.00 + oldLeftVelocity)) / accelSteps;
  float rightAccelStep = ((1500.00 - rightVelocity) - (1500.00 - oldRightVelocity)) / accelSteps;

  //Accelerationsloop. Ökar den gamla farten till den nya och skriver ut värdena i två kolumner
  for (int i = 0; i < accelSteps; i++) {
    servoLeft.writeMicroseconds(1500 + oldLeftVelocity + leftAccelStep * i);
    Serial.print(1500 + oldLeftVelocity + leftAccelStep * i);
    Serial.print("     ");
    servoRight.writeMicroseconds(1500 - oldRightVelocity + rightAccelStep * i);
    Serial.println(1500 - oldRightVelocity + rightAccelStep * i);
    delay(accelDuration / accelSteps); // Fördröjningen mellan varje fartökning
  }

  //Om accelerationsloppen inte träffar exakt rätt pga avrundning
  servoLeft.writeMicroseconds(1500 + leftVelocity);
  Serial.print(1500 + leftVelocity);
  Serial.print("        ");
  servoRight.writeMicroseconds(1500 - rightVelocity);
  Serial.println(1500 - rightVelocity);
  Serial.println();

  delay(duration - accelDuration); //Tid på full fart

  oldLeftVelocity = leftVelocity;
  oldRightVelocity = rightVelocity;

  tone(4, 3000, 200); //Ton vid hastighetsändring
}

void setup() {

  Serial.begin(9600);

  //Startton
  tone(4, 3000, 500);
  delay(1000);

  servoLeft.attach(11);
  servoRight.attach(12);

  //Fart vänster hjul, hastighet höger hjul, tid
  //Minsta tid är ca. 500. Antagligen pga hårdvarubegränsningar
  //Max rekomenderad hastighet är 100 (1600 respektive 1400) pga att det det änder så pass lite mellan 100 och 200
  Drive(100, 100, 500);

  Drive(0, 0, 500); //Stanna när den är klar

  tone(4, 3000, 600); //Slutton

}

void loop() {

}
