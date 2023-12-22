//define channel enable output pins/		
#define LMSpeed 6				//initial speed of car >=0 to <=255
#define RMSpeed 5

//define logic control output pins/
#define LMD1 7 		//Left Motors direction
#define LMD2 8		
#define RMD1 9		//Right Motors direction
#define RMD2 4

//encoder define
#define encoderSpeedPinR 2   // Connect yellow wire to pin 9
#define encoderdirectionPinR 3  // Connect green wire to pin 12
//#define encoderSpeedPinL ??
//#define encoderdirectionPinL ??
#define ENC_COUNT_REV 616         //11 * 56:::::: Resolution = 11 *(gear Ration )
#define DIR_INTERVAL 200
#define COUNT_INTERVAL 1000
//1634 pulses for 1 meter 
//The Mega 2560 is capable of six external interrupts, which are 0-5 on pins 2, 3, 21, 20, 19, and 18, respectively.


/*		RIGHT AND LEFT ENCODERS VARIABLES		*/
volatile long pulseCountR = 0, dirRight = 0;       //Right encoder pulses and Dirction counter
long preDirMillisR = 0, preCountMillisR = 0;				
long curDirMillisR = 0, curCountMillisR = 0;
int rmpR = 0, speedR = 0;
typedef enum {Forward, Backward, Stop} encoderRightDirection;
encoderRightDirection rightState = Stop;				//by default dirRightection is Stop

volatile long pulseCountL = 0, dirLeft = 0;
long preDirMillisL = 0, preCountMillisL = 0;	   //Left time variables needed
long curDirMillisL = 0, curCountMillisL = 0;
int rmpL = 0, speedL = 0;
typedef enum {Forward, Backward, Stop} encoderLeftDirection;
encoderRightdirection leftState = Stop;				//by default Stop



void setup() {
  Serial.begin(9600);
  //Set motor Pins
  pinMode(LMD1, OUTPUT);
  pinMode(LMD2, OUTPUT);
  pinMode(LMSpeed, OUTPUT);
  pinMode(RMD1, OUTPUT);
  pinMode(RMD2, OUTPUT);
  pinMode(RMSpeed, OUTPUT);
  //Set Encoder pins and interrupts
  pinMode(encoderSpeedPinR, INPUT);
  pinMode(encoderdirectionPinR, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(encoderSpeedPinR), encRightChA, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encoderdirectionPinR), encRightChB, CHANGE);
  //attachInterrupt(digitalPinToInterrupt(encoderSpeedPinL), encLeftChA, CHANGE);
  //attachInterrupt(digitalPinToInterrupt(encoderdirectionPinL), encLeftChB, CHANGE);
  
  //MoveForward(210, 250);
  preDirMillisR = millis();
  preCountMillisR = millis();
}



void loop() {
  
}

void encRightChA(){					//Check the direction each 200 ms and 
  if (digitalRead(encoderSpeedPinR) == HIGH){
    if (digitalRead(encoderdirectionPinR == LOW)){
      dirRight++;
    }
    else{
      dirRight--;
    }
  }
  else{
    if (digitalRead(encoderdirectionPinR) == HIGH){
      dirRight++;
    }
    else{
      dirRight--;
     }
  }
  
  //Determine the Direction of the Right Wheels each 200 ms
  curDirMillisR = millis();
  if (curDirMillisR - preDirMillisR > DIR_INTERVAL){
    preDirMillisR = curDirMillisR;
    //Serial.println(dirRight);
    if (dirRight < 20){
		rightState = Backward;
		Serial.println("R: Backward...");
    }
    else{
		rightState = Forward;
		Serial.println("R: Forward...");
    }
    dirRight = 0;
  }
  
  pulseCountR++;			//increment the Right encoder pulse count each interrupts
  
  /*NOTE THAT THIS CALCULATION MAY BE IN THE LOOP AND NOT HEREE Because here pulseCountR may not go to 0 if we don't go to interrupt*/
  curCountMillisR = millis();
  if (curCountMillisR - preCountMillisR > COUNT_INTERVAL) {			//each one second calculate the rpm (60 seconds)
    previousMillis = curCountMillisR;
    rpmR = (float)(pulseCountR * 60 / ENC_COUNT_REV); 
	speedR = (float)((rpmR * 2 * 3.14159 * 0.06) / 60.0);			//m/s speed = rpm * 2piR /60
    pulseCountR = 0;
  }
}


void encRightChB(){
  if (digitalRead(encoderdirectionPinR) == HIGH){
    if (digitalRead(encoderSpeedPinR == HIGH)){
      dirRight++;
    }
    else{
      dirRight--;
    }
  }
  else{
    if (digitalRead(encoderSpeedPinR) == LOW){
      dirRight++;
    }
    else{
      dirRight--;
     }
  }
}

void MoveForward(int speedL, int speedR){
  analogWrite(RMSpeed,speedR);
  analogWrite(LMSpeed,speedL);
  digitalWrite(RMD1,LOW); 
  digitalWrite(RMD2,HIGH);
  digitalWrite(LMD1,LOW); 
  digitalWrite(LMD2,HIGH);
}

void MoveBackward(int speedL, int speedR){
  digitalWrite(RMSpeed,speedR);
  digitalWrite(LMSpeed,speedL);
  digitalWrite(RMD1,HIGH); 
  digitalWrite(RMD2,LOW);
  digitalWrite(LMD1,HIGH); 
  digitalWrite(LMD2,LOW);
}

void MoveLeft(int speed){
  analogWrite(RMSpeed,speed);
  analogWrite(LMSpeed,speed);
  digitalWrite(RMD1,HIGH); 
  digitalWrite(RMD2,LOW);
  digitalWrite(LMD1,LOW); 
  digitalWrite(LMD2,HIGH);
}

void MoveRight(int speed){
  analogWrite(RMSpeed,speed);
  analogWrite(LMSpeed,speed);
  digitalWrite(RMD1,LOW); 
  digitalWrite(RMD2,HIGH);
  digitalWrite(LMD1,HIGH); 
  digitalWrite(LMD2,LOW);
}

void Stop(){
  digitalWrite(RMSpeed, LOW);
  digitalWrite(LMSpeed, LOW);
}



/*
ENA  Orange
IN1  BLUE   
IN2  RED

ENB  Yellow
IN3  Purple
In4  Green


GND Black from driver
GREY +5volts from driver
*/
