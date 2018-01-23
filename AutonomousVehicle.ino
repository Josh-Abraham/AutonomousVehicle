#include <NewPing.h>
//Designed as a first year lab experience for new students


#define brake_A 9  // Brake Pin of Channel A
#define motor_A 12  // Motor Pin of Channel A
#define rotor_A 3 // Rotor Pin for speed control

#define brake_B 1  // Beake Pin of Channel B
#define motor_B 13  // Motor Pin of Channel B
#define rotor_B 11 //Rotor Pin for Speed control

#define MAX_DISTANCE 40
#define MID_TRIGGER 5
#define MID_ECHO 6
#define LEFT_TRIGGER 4
#define LEFT_ECHO 2
#define RIGHT_TRIGGER 7
#define RIGHT_ECHO 8

long time_centre;
long time_left;
long time_right;
long distance_centre;
long distance_left;
long distance_right;

/// Instantiate Ultrasonic Sensors
NewPing centre(MID_TRIGGER, MID_ECHO, MAX_DISTANCE);
NewPing left(LEFT_TRIGGER, LEFT_ECHO, MAX_DISTANCE);
NewPing right(RIGHT_TRIGGER, RIGHT_ECHO, MAX_DISTANCE);

// Configure
void setup()
{

  //Setup Channel A
  pinMode(motor_A, OUTPUT); //Initiates motor pin for channel A
  pinMode(brake_A, OUTPUT); //Initiates break pin for channel A

  //Setup Channel B
  pinMode(motor_B, OUTPUT); //Initiates motor pin for channel B
  pinMode(brake_B, OUTPUT);  //Initiates break pin for channel B

  //Diengages the motor breaks first
  digitalWrite(brake_A, LOW);   //Disengage the Brake for Channel A
  digitalWrite(brake_B, LOW);   //Disengage the Brake for Channel B

  Serial.begin(9600);
  delay(3000);
}



void loop()
{
    // Using the ping() function, we are able to return the time
    // in milliseconds between pulse trigger and pulse echo.

    time_centre = centre.ping();
    time_left = left.ping();
    time_right = right.ping();
    // Now we need to convert milliseconds to centimeters
    // However, due to the limitations of ultrasonic sensors if the time returned is never
    // returned it will be set to 0, and so this must be taken into account within the code
     if (time_centre == 0)
     {
      distance_centre = MAX_DISTANCE;
     }
     else
     {
      distance_centre = microsecondsToCentimeters(time_centre);
     }

     if (time_left == 0)
     {
      distance_left = MAX_DISTANCE;
     }
     else
     {
      distance_left = microsecondsToCentimeters(time_left);
     }

     if (time_right == 0)
     {
      distance_right = MAX_DISTANCE;
     }
     else
     {
      distance_right = microsecondsToCentimeters(time_right);
     }

     /*********************************************************
      * The first condition is having the robot               *
      * do a hard left (pivot) if there are obstacles         *
      * in front of and to the right side of the sensors      *
      * within 10 cm.                                         *
      *                                                       *
      * To achieve this motor A (Right Motor) spins           *
      * backwards (HIGH) & motor B (Left Motor) spins         *
      * forwards (LOW)                                        *
      *********************************************************/

    //HARD LEFT
    //WALLS TO THE RIGHT & INFRONT OF CAR
    if ((distance_right < 10) && (distance_centre < 10))
    {
      digitalWrite(motor_A, LOW); //Establishes forward direction of Channel A
      analogWrite(rotor_A, 100);   //Spins the motor on Channel A
      digitalWrite(motor_B, HIGH);  //Establishes backward direction of Channel B
      analogWrite(rotor_B, 150);    //Spins the motor on Channel B
      delay(200);
    }

  //HARD RIGHT
  //WALLS TO THE LEFT & INFRONT OF CAR
     else if ((distance_left < 10) && (distance_centre < 10))
    {
      digitalWrite(motor_A, HIGH); //Establishes backward direction of Channel A
      analogWrite(rotor_A, 150);   //Spins the motor on Channel A
      digitalWrite(motor_B, LOW);  //Establishes forward direction of Channel B
      analogWrite(rotor_B, 100);    //Spins the motor on Channel B
      delay(200);
    }

    //GO RIGHT
  else if (distance_left < 10)
    {

        digitalWrite(motor_A, LOW); //Establishes forward direction of Channel A
        analogWrite(rotor_A, 60);   //Spins the motor on Channel A slower
        digitalWrite(motor_B, LOW);  //Establishes forward direction of Channel B
        analogWrite(rotor_B, 94);    //Spins the motor on Channel B
        delay(200);
    }
    else if (distance_right < 10)
    {
        // GO LEFT
        digitalWrite(motor_A, LOW); //Establishes forward direction of Channel A
        analogWrite(rotor_A, 94);   //Spins the motor on Channel A
        digitalWrite(motor_B, LOW);  //Establishes forward direction of Channel B
        analogWrite(rotor_B, 60);    //Spins the motor on Channel B slower
        delay(200);
    }
    else if (distance_centre < 10)
    {
        // CENTRE RESET
        if (distance_left < ditance_right)
        {
          digitalWrite(motor_A, HIGH); //Establishes backward direction of Channel A
          analogWrite(rotor_A, 54);   //Spins the motor on Channel A slower
          digitalWrite(motor_B, HIGH);  //Establishes backward direction of Channel B
          analogWrite(rotor_B, 70);    //Spins the motor on Channel B
          delay(200);
        }
        else
        {
          digitalWrite(motor_A, HIGH); //Establishes backward direction of Channel A
          analogWrite(rotor_A, 70);   //Spins the motor on Channel A slower
          digitalWrite(motor_B, HIGH);  //Establishes backward direction of Channel B
          analogWrite(rotor_B, 54);    //Spins the motor on Channel B
          delay(200);
        }
    }
else
   {

    // STRAIGHT AHEAD
    digitalWrite(motor_A, LOW); //Establishes direction of Channel A
    analogWrite(rotor_A, 98);   //Spins the motor on Channel A
    digitalWrite(motor_B, LOW);  //Establishes backward direction of Channel B
    analogWrite(rotor_B, 98);    //Spins the motor on Channel B
    delay(200);
   }
}

/******************************************************
 * The sensors give the time from trigger to echo     *
 * in microseconds, and we wish to find the distance  *
 * in centimeters.                                    *
 *                                                    *
 * Consider the parameter below (the part in brackets)*
 * "long microseconds" to be your x value             *
 * We wish to create a mathematical formula that      *
 * takes in the parameter x and returns some value y  *
 *                                                    *
 * In our case we have a simple formula of y = mx     *
 * and to send back our y value all we need to do     *
 * is calculate m and return it as follows:           *
 * return m * microseconds;                           *
 *                                                    *
 * The speed of sound is 343.13 m/s                   *
 * The ping travels out and back, so to find the      *
 * distance of the object we take half of the         *
 * distance travelled.                                *
 * CONVERT FROM M/S TO CM/μs                          *
 ******************************************************/
long microsecondsToCentimeters(long microseconds) {
  return microseconds * 0.0171565;
}
