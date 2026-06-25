#include <Servo.h>

// ================= PIN DEFINITIONS =================
#define TRIG_PIN 8
#define ECHO_PIN 9
#define SERVO_PIN 3

// Motor Driver Pins
#define IN1 A0
#define IN2 A1
#define IN3 A2
#define IN4 A3
#define ENA 5
#define ENB 6

// Vacuum Motor / Fan Relay Pin
#define VACUUM_MOTOR 7

// ================= OBJECTS & VARIABLES =================
Servo myServo;

int motorSpeed = 105;      // Normal motor speed: 0-255
int turnSpeed = 150;       // Turning speed: 0-255
int safeDistance = 30;     // Minimum safe distance in cm

// ================= SETUP =================
void setup() {
  Serial.begin(9600);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  pinMode(VACUUM_MOTOR, OUTPUT);

  myServo.attach(SERVO_PIN);
  myServo.write(78);  // Center position

  analogWrite(ENA, motorSpeed);
  analogWrite(ENB, motorSpeed);

  digitalWrite(VACUUM_MOTOR, HIGH);  // Vacuum motor ON

  Serial.println("Automatic Vacuum Cleaner Started");
  delay(1000);
}

// ================= MAIN LOOP =================
void loop() {
  long frontDistance = readDistance();

  Serial.print("Front Distance: ");
  Serial.print(frontDistance);
  Serial.println(" cm");

  if (frontDistance > safeDistance) {
    Serial.println("Moving Forward");
    moveForward();
  } else {
    Serial.println("Obstacle Detected");
    stopMotor();
    delay(150);

    Serial.println("Moving Backward");
    moveBackward();
    delay(300);
    stopMotor();
    delay(150);

    // Look left
    myServo.write(148);
    delay(400);
    long leftDistance = readDistance();

    Serial.print("Left Distance: ");
    Serial.print(leftDistance);
    Serial.println(" cm");

    // Look right
    myServo.write(18);
    delay(400);
    long rightDistance = readDistance();

    Serial.print("Right Distance: ");
    Serial.print(rightDistance);
    Serial.println(" cm");

    // Center servo again
    myServo.write(78);
    delay(200);

    analogWrite(ENA, turnSpeed);
    analogWrite(ENB, turnSpeed);

    if (leftDistance > rightDistance) {
      Serial.println("Turning Left");
      turnLeft();
      delay(450);
    } else {
      Serial.println("Turning Right");
      turnRight();
      delay(450);
    }

    stopMotor();

    analogWrite(ENA, motorSpeed);
    analogWrite(ENB, motorSpeed);
  }

  Serial.println("-----------------------------");
  delay(100);
}

// ================= DISTANCE FUNCTION =================
long readDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 25000);

  if (duration == 0) {
    Serial.println("Ultrasonic: No Echo");
    return 200;
  }

  long distance = duration * 0.034 / 2;
  return distance;
}

// ================= MOTOR FUNCTIONS =================
void moveForward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void moveBackward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void turnLeft() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void turnRight() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void stopMotor() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
