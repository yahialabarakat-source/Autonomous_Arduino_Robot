#include <math.h>

// Base speed
int baseSpeed = 160;

// Ultrasonic sensors
#define trigL 9
#define echoL 10
#define trigR 11
#define echoR 12
#define trigF 13
#define echoF 8

// Motor Driver Pins
#define IN1 3
#define IN2 4
#define IN3 5
#define IN4 6
#define ENA 2
#define ENB 7

// Control
const int MIN_PWM = 75;
float Kp = 1.7;
float Kd = 0.9;

// State
float ePrev = 0;
unsigned long tPrev = 0;

void setup() {
  pinMode(ENA, OUTPUT); 
  pinMode(IN1, OUTPUT); 
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT); 
  pinMode(IN3, OUTPUT); 
  pinMode(IN4, OUTPUT);

  pinMode(trigL, OUTPUT); 
  pinMode(echoL, INPUT);
  pinMode(trigR, OUTPUT); 
  pinMode(echoR, INPUT);
  pinMode(trigF, OUTPUT); 
  pinMode(echoF, INPUT);

  Serial.begin(9600);
  tPrev = millis();
}

void loop() {
  // Read Sensors
  float dL = readUltrasonicCM(trigL, echoL);
  float dR = readUltrasonicCM(trigR, echoR);
  float dF = readUltrasonicCM(trigF, echoF);

  // Timing
  unsigned long tNow = millis();
  float dt = (tNow - tPrev) / 1000.0;
  if (dt <= 0) dt = 0.01;
  tPrev = tNow;

  // Control
  float e = dR - dL;        // stay centered
  float de = (e - ePrev) / dt;
  ePrev = e;

  // avoid spikes
  if (de > 50) de = 50;
  if (de < -50) de = -50;

  float correction = (Kp * e) + (Kd * de);

  // Deadband to reduce jitter
  if (abs(e) < 1.0) correction = 0;

  // Speed Adjustment Based on Front Sensor
  if (dF > 7) baseSpeed = 130;   // speed up
  else baseSpeed = 90;           // slow down

  // steer using adaptive speed
  int leftPWM  = baseSpeed - correction;
  int rightPWM = baseSpeed + correction;

  leftPWM  = constrain(leftPWM,  MIN_PWM, 255);
  rightPWM = constrain(rightPWM, MIN_PWM, 255);

  Serial.print("L: "); 
  Serial.print(leftPWM);
  Serial.print(" R: "); 
  Serial.print(rightPWM);
  Serial.print(" Front: "); 
  Serial.println(dF);

  driveMotors(leftPWM, rightPWM);
}

void driveMotors(int lPWM, int rPWM) {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, lPWM);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENB, rPWM);
}

// Ultrasonic Sensor
float readUltrasonicCM(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 30000);

  if (duration == 0) return 40; // fallback
  else {
    float cm = duration * 0.0343 / 2.0; // speed of sound
    if (cm < 2 || cm > 200) cm = 40;    // validate reading
    return cm;
  }
}
