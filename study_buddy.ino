/* ------------------------------------------------------*/
/* Student 1: Maksat Khobdabayev, mk7090*/
/* Student 2: Pelin Demirci, pd2164*/
/* Student 1: Juno Yoon Park, jyp399*/
/* Date: December 16, 2021*/
/* Program: study_buddy.cpp*/
/* Description: Program for device that sets productive and healthy study routine. */


#include <M5Core2.h>

// The screen is 320 x 240 pixels
#define LCD_MAX_X 320
#define LCD_MAX_Y 240

//Function prototypes
void free_state();
void busy_state();
void pomodoro_state(int study_min, int break_min);
void ultrasonic_break();
void ultrasonic_eyekeep();

//Variables for ultrasonic functions
const int trigPin = 33;
const int echoPin = 32;
long duration;
int distance;


void setup() {
    // put your setup code here, to run once:

    // Initialize the M5Stack object
    M5.begin();
    //Set text size to 10px
    M5.Lcd.setTextSize(10);

    pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
    pinMode(echoPin, INPUT); // Sets the echoPin as an Input
    Serial.begin(9600); // Starts the serial communication
}

void loop() {
    // put your main code here, to run repeatedly:

    M5.update();


    if (M5.BtnA.wasPressed()) {
        //Call function to indicate that user is Free now
        free_state();
    }
    else if (M5.BtnC.wasPressed()) {
        //Call function to indicate that user is Busy now
        busy_state();
    }
    else if (M5.BtnB.wasPressed()) {
        //Call function to use pomodoro
        //Study for 25 min, break for 5 min
        pomodoro_state(25,5);
    }   
}

void free_state() {
    //Clear the display
    M5.Lcd.clear();
    M5.Lcd.setRotation(1);
    //Center the text
    M5.Lcd.setCursor(75, 90);
    //Change color to red
    M5.Lcd.setTextColor(GREEN);
    //Print the state
    M5.Lcd.print("FREE");
    //Change state
    if (M5.BtnC.wasPressed()) {
        busy_state();
    }
    else if (M5.BtnB.wasPressed()) {
        pomodoro_state(25,5);
    }
}

void busy_state() {
    //Clear the display
    M5.Lcd.clear();
    M5.Lcd.setRotation(1);
    //Center the text
    M5.Lcd.setCursor(75, 90);
    //Change color to red
    M5.Lcd.setTextColor(RED);
    //Print the state
    M5.Lcd.print("BUSY");
    //Change state
    if (M5.BtnA.wasPressed()) {
        free_state();
    }
    else if (M5.BtnB.wasPressed()) {
        pomodoro_state(25, 5);
    }
}

void ultrasonic_break() {
    // Clears the trigPin
    digitalWrite(trigPin, LOW);
    delay(2);
    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(trigPin, HIGH);
    delay(10);
    digitalWrite(trigPin, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration = pulseIn(echoPin, HIGH);
    // Calculating the distance
    distance = duration * 0.034 / 2;
    //Prints in Serial Monitor - used for debugging 
    Serial.println(distance);
    //Keep the user away from computer
    if (distance < 80) {
        M5.Lcd.clear();
        M5.Lcd.setRotation(1);
        M5.Lcd.setCursor(45, 60);
        M5.Lcd.setTextColor(GREEN);
        //Prints warning to encourage the user to relax
        M5.Lcd.print("NO, GO");
        M5.Lcd.setCursor(70, 120);
        M5.Lcd.print("RELAX");
        delay(2000);
    }
}
void ultrasonic_eyekeep() {
    // Clears the trigPin
    digitalWrite(trigPin, LOW);
    delay(2);
    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(trigPin, HIGH);
    delay(10);
    digitalWrite(trigPin, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration = pulseIn(echoPin, HIGH);
    // Calculating the distance
    distance = (duration * 0.034 / 2) * cos(30 * PI / 180);
    //Prints in Serial Monitor - used for debugging 
    Serial.println(distance);
    //Keep distance at least 50 cm away from monitor
    if (distance < 50) {
        M5.Lcd.clear();
        M5.Lcd.setRotation(1);
        M5.Lcd.setCursor(15, 90);
        M5.Lcd.setTextColor(RED);
        //Print warning for keeping distance
        M5.Lcd.print("GO BACK");
        //Wait 2 sec
        delay(2000);
    }
}


void pomodoro_state(int study_min, int break_min) {
    //wait for 0.25 sec to avoid bouncing double presses in button B
    delay(250); 
    M5.Lcd.clear();
    M5.Lcd.setRotation(1);
    //Set black screen
    M5.Lcd.fillScreen(BLACK);
    //Prepare the display
    M5.Lcd.clear();
    M5.Lcd.setRotation(1);
    M5.Lcd.setCursor(60, 90);
    M5.Lcd.setTextColor(RED);
    //Indicate the start of pomodoro
    M5.Lcd.print("STUDY");
    delay(2000);

    //Ex: Study for 25 min
    //Multiply by 60 sec to convert min to sec as it waits for 1 sec for a cycle
    for (int i = 0; i < study_min * 60; i++) {
        M5.Lcd.clear();
        M5.Lcd.setRotation(1);
        M5.Lcd.setCursor(90, 60);
        M5.Lcd.setTextColor(RED);
        //Print time left
        //Divide by 60 sec to convert sec back to min
        M5.Lcd.printf("%d/%d", i / 60, study_min);
        M5.Lcd.setCursor(90, 120);
        M5.Lcd.print("min");
        //Call function to keep distance between eyes and monitor
        ultrasonic_eyekeep();
        //Wait for 1 sec
        delay(1000);
    }
    //Prepare the display
    M5.Lcd.clear();
    M5.Lcd.setRotation(1);
    M5.Lcd.setCursor(60, 90);
    M5.Lcd.setTextColor(GREEN);
    //Indicate the start of break
    M5.Lcd.print("BREAK");
    delay(2000);


    //Ex: Break for 5 min
    //Multiply by 60 sec to convert min to sec as it waits for 1 sec for a cycle
    for (int j = 0; j < break_min * 60; j++) {
        //Prepare the display
        M5.Lcd.clear();
        M5.Lcd.setRotation(1);
        M5.Lcd.setCursor(90, 60);
        M5.Lcd.setTextColor(GREEN);
        //Print time left
        //Divide by 60 sec to convert sec back to min
        M5.Lcd.printf("%d/%d", j / 60, break_min);
        M5.Lcd.setCursor(90, 120);
        M5.Lcd.print("min");
        //Call function to measure the distance the person and computer
        ultrasonic_break();
        //Wait for 1 sec
        delay(1000);
    }
    //Prepare the display
    M5.Lcd.clear();
    M5.Lcd.setRotation(1);
    M5.Lcd.setCursor(45, 90);
    M5.Lcd.setTextColor(GREEN);
    //Indicate the end of the pomodoro session
    M5.Lcd.print("Great!");

}
