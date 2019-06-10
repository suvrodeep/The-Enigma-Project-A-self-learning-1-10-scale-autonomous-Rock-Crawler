#include <NewPing.h>
#include <Servo.h>

#define ESCDetect A3  // Define pin for analog read of ESC voltage

const int steering_right_max = 35;    // Maximum angle of steering in right turns
const int steering_left_max = 131;    // Maximum angle of steering in left turns
const int steering_straight = 85;     // Value for steering to hold straight
const int maximumRange = 300;         // Maximum range
const int minimumRange = 15;          // Minimum range

int throttle_pos = 90;   // Variable to store current throttle position. Initialize to neutral
int distance[] = {0, 0, 0, 0, 0, 0};
int *check_distance;
int check_clearance;
int counter = 0;         // Global counter
int index = 0;

bool esc_detect = 0;        // ESC detection flag
//bool sensor_test = 1;       // Sensor test flag
bool surrounding_test = 1;   // Surrounding test

NewPing sensorArray[] = {NewPing(13, 8, 300),
                         NewPing(13, 7, 300),
                         NewPing(13, 9, 300),
                         NewPing(13, 11, 300),
                         NewPing(13, 10, 300),
                         NewPing(13, 12, 300)
};

Servo throttle;               // Initiate throttle as a servo object
Servo steering;               // Initiate steering as a servo object


int smooth_accel(int throttle_input) {     // Define smooth acceleration function
    if (throttle_input > throttle_pos) {
        if (throttle_pos < 90 && throttle_input > 90) {   // If commanded to move forward when moving in reverse
            throttle.write(90); // Release throttle
            delay(1000);        // Allow time to stop
            for (counter = 90; counter <= throttle_input; counter++) {   // Accelerate in forward direction
                throttle.write(counter);
                delay(30);
            }
        } else {
            for (counter = throttle_pos;
                 counter <= throttle_input; counter++) {   // Accelerate smmothly in forward direction
                throttle.write(counter);
                delay(30);
            }
        }
    } else if (throttle_input < throttle_pos) {
        if (throttle_pos > 90 && throttle_input < 90) {    //If commanded to reverse when moving forward
            throttle.write(90); // Release throttle
            delay(1000);        // Allow time to stop
            for (counter = 90; counter >= throttle_input; counter--) {   // Accelerate in reverse
                throttle.write(counter);
                delay(30);
            }
        } else {
            for (counter = throttle_pos; counter >= throttle_input; counter--) {   // Accelerate smoothly in reverse
                throttle.write(counter);
                delay(30);
            }
        }
    } else {
        throttle.write(throttle_input);
        counter = throttle_input;
    }
    return (counter);
}


void stop_immediate() {             //Immediate stop function
    throttle.write(90);
    steering.write(steering_straight);
}


int *get_distance() {             // Define get_distance function. Pings sensors and returns the distance
    for (index = 0; index < 6; index++) {
        distance[index] = sensorArray[index].ping_cm();
        delay(50);
    }

    for (index = 0; index < 6; index++) {
        Serial.print(distance[index]);
        Serial.print(",");
        if (index == 5) {
            Serial.print("\n");
        }
    }

    return (distance);
}


void ardu_command() {
    if (sensorArray[1].ping_cm() >= maximumRange) {
        steering.write(steering_straight);
        throttle_pos = smooth_accel(115);  // Increase speed if there is no obstacle within 200 cm
    } else if (sensorArray[1].ping_cm() > minimumRange && check_distance[1] < maximumRange) {
        steering.write(steering_straight);
        throttle_pos = smooth_accel(108);  // Maintain moderate speed if obstacle is within 200 cm
    } else {
        do {
            throttle_pos = smooth_accel(78);   // Stop the car and initiate reverse if there is an obstacle within 10 cm
            steering.write(40);                // Turn 40 degrees and continue backing up until front is clear
            check_clearance = sensorArray[1].ping_cm();
        } while (check_clearance < 70 && check_clearance > 0);
    }
}


void check_sensors() {
    check_distance = get_distance();              // Check for sensor error and surroundings
    surrounding_test = 1;
    for (index = 0; index < 6; index++) {
        if (check_distance[index] < minimumRange) {
            surrounding_test = 0;
            break;
        } else {
            continue;
        }
    }
}


void esc_initialize() {
    throttle.write(90);   // Arms the ESC with zero throttle
    steering.write(85);
    Serial.println(analogRead(ESCDetect));
    esc_detect = 1;
    delay(5000);
}


void setup() {                    // Code to run once during initial setup goes here 
    Serial.begin(9600);
    pinMode(5, OUTPUT); // Steering pin
    pinMode(6, OUTPUT); // Throttle pin
    pinMode(4, OUTPUT); // Pin to control relay

    digitalWrite(4, HIGH); // Switch relay and change ESC to Arduino Controlled mode

    throttle.attach(6);   // Attaches the throttle on pin 6
    steering.attach(5);   // Attaches the steering on pin 5
    check_sensors();
    esc_initialize();
}

void loop() {                   // Code to run over and over in a loop goes here
    if (esc_detect == 1 && surrounding_test == 1) {
        ardu_command();
    } else {
        check_sensors();
        esc_initialize();
    }

    if (analogRead(ESCDetect) < 470) {
        esc_detect = 0;
    } else {}
}
