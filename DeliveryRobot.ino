// -----------------------------
// Global Variables and Settings
// -----------------------------

int last_direction  = 0;         // Stores the last movement direction
int robot_speed     = 150;       // Default robot motor speed (PWM value)

// Time delay between loop cycles (in milliseconds)
#define DELAYMS 10

// Movement direction flags (used for robot_move function)
#define FORWARD     0
#define HARD_LEFT   1
#define HARD_RIGHT  2
#define SOFT_LEFT   3
#define SOFT_RIGHT  4

// PWM motor speed pins
#define leftMotorPWM  10  
#define rightMotorPWM 11  

// Line sensor input pins
#define leftSensor    12
#define rightSensor   13

// Motor direction control pins
#define leftMotor1  2
#define leftMotor2  3
#define rightMotor1 4
#define rightMotor2 5

// Ultrasonic sensor pins
#define trigPin A0
#define echoPin A1


// ---------------------------------------------------------
// Ultrasonic distance measurement function
// Returns distance in centimeters
// ---------------------------------------------------------
int search(void)
{
      float duration = 0.00;       // Time for echo pulse
      float CM = 0.00;             // Distance in cm

      // Send a 10us trigger pulse
      digitalWrite(trigPin, LOW);
      delayMicroseconds(2);
      digitalWrite(trigPin, HIGH);
      delayMicroseconds(10);
      digitalWrite(trigPin, LOW);

      // Read the time pulse remains HIGH (echo received)
      duration = pulseIn(echoPin, HIGH);

      // Convert time to distance (standard formula for HC-SR04)
      CM = (duration / 58.82);

      return CM;   // Return the distance in centimeters
}


// ---------------------------------------------------------
// Setup function (runs once at start)
// Initializes pins and motor speed
// ---------------------------------------------------------
void setup()
{
  // Motor direction pins
  pinMode(leftMotor1, OUTPUT);
  pinMode(leftMotor2, OUTPUT);
  pinMode(rightMotor1, OUTPUT);
  pinMode(rightMotor2, OUTPUT);

  // Motor PWM control pins
  pinMode(leftMotorPWM, OUTPUT);
  pinMode(rightMotorPWM, OUTPUT);

  delay(100);

  // IR Line sensors
  pinMode(leftSensor, INPUT);
  pinMode(rightSensor, INPUT);

  delay(100);

  // Ultrasonic sensor pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  delay(100);

  // Set initial motor speed
  analogWrite(leftMotorPWM, robot_speed);
  analogWrite(rightMotorPWM, robot_speed);

  delay(100);
}


// ---------------------------------------------------------
// Main Loop (runs repeatedly)
// Handles obstacle detection + line following
// ---------------------------------------------------------
void loop()
{
  // Read IR line sensors
  int left_reading = digitalRead(leftSensor);
  int right_reading = digitalRead(rightSensor);

  // Measure distance from obstacle
  float distance = search();

  // ---------------------------
  // Obstacle Avoidance
  // ---------------------------
  if (distance < 10)    // If obstacle is within 10 cm
  {
      RobotStop();      // Stop robot immediately
  }

  // ---------------------------
  // Line Following Logic
  // ---------------------------

  // Both sensors detect the black line → move forward
  else if(left_reading == 1 && right_reading == 1)
  {
      robot_move(FORWARD);
      last_direction = FORWARD;
  }

  // Left sensor on line, right sensor off → soft left turn
  else if(left_reading == 1 && right_reading == 0)
  {
      robot_move(SOFT_LEFT);
      last_direction = HARD_LEFT;   // Save last direction as left
  }

  // Right sensor on line, left sensor off → soft right turn
  else if(left_reading == 0 && right_reading == 1)
  {
      robot_move(SOFT_RIGHT);
      last_direction = HARD_RIGHT;  // Save last direction as right
  }

  // Both sensors OFF the line → continue in last direction
  else if(left_reading == 0 && right_reading == 0)
  {
      robot_move(last_direction);   // Use memory direction
  }

  delay(DELAYMS);  // Small delay for stability
}



// ---------------------------------------------------------
// Robot movement function
// Controls motor direction based on 'direc' value
// ---------------------------------------------------------
void robot_move(int direc)
{
  if (direc == FORWARD)
  {
    // Both motors move forward
    digitalWrite(leftMotor1, LOW);
    digitalWrite(leftMotor2, HIGH);
    digitalWrite(rightMotor1, HIGH);
    digitalWrite(rightMotor2, LOW);
  }

  else if (direc == SOFT_LEFT)
  {
    // Left motor stops, right motor moves → soft left turn
    digitalWrite(leftMotor1, LOW);
    digitalWrite(leftMotor2, LOW);
    digitalWrite(rightMotor1, HIGH);
    digitalWrite(rightMotor2, LOW);
  }

  else if (direc == HARD_LEFT)
  {
    // Left motor reverses, right motor forward → sharp left turn
    digitalWrite(leftMotor1, HIGH);
    digitalWrite(leftMotor2, LOW);
    digitalWrite(rightMotor1, HIGH);
    digitalWrite(rightMotor2, LOW);
  }

  else if (direc == SOFT_RIGHT)
  {
    // Right motor stops, left motor moves → soft right turn
    digitalWrite(leftMotor1, LOW);
    digitalWrite(leftMotor2, HIGH);
    digitalWrite(rightMotor1, LOW);
    digitalWrite(rightMotor2, LOW);
  }

  else if (direc == HARD_RIGHT)
  {
    // Left forward, right reverse → sharp right turn
    digitalWrite(leftMotor1, LOW);
    digitalWrite(leftMotor2, HIGH);
    digitalWrite(rightMotor1, LOW);
    digitalWrite(rightMotor2, HIGH);
  }
}



// ---------------------------------------------------------
// Stop all motors (robot stops completely)
// ---------------------------------------------------------
void RobotStop()
{
    digitalWrite(leftMotor1, LOW);
    digitalWrite(leftMotor2, LOW);
    digitalWrite(rightMotor1, LOW);
    digitalWrite(rightMotor2, LOW);
}
