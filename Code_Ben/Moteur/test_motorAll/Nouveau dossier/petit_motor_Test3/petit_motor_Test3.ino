const int desiredValue = 20;//time in sec for a Revolution

const int dir = 2; // 1 for CW and 2 for CCW

int Pin1 = 8;//IN1 is connected to 10 
int Pin2 = 9;//IN2 is connected to 11  
int Pin3 = 10;//IN3 is connected to 12  
int Pin4 = 11;//IN4 is connected to 13 
int potPin = A0;// center pin of pot is connected to this pin
int vcc2 = 2;// additional 5V for potentiometer

int pole1[] ={0,0,0,0, 0,1,1,1, 0};//pole1, 8 step values
int pole2[] ={0,0,0,1, 1,1,0,0, 0};//pole2, 8 step values
int pole3[] ={0,1,1,1, 0,0,0,0, 0};//pole3, 8 step values
int pole4[] ={1,1,0,0, 0,0,0,1, 0};//pole4, 8 step values


int poleStep = 0; 
int calcDelayfromTime(int t);
void driveStepper(int c);
int N = calcDelayfromTime(desiredValue);


void setup() 
{ 
 pinMode(Pin1, OUTPUT);//define pin for ULN2003 in1 
 pinMode(Pin2, OUTPUT);//define pin for ULN2003 in2   
 pinMode(Pin3, OUTPUT);//define pin for ULN2003 in3   
 pinMode(Pin4, OUTPUT);//define pin for ULN2003 in4 
 pinMode(vcc2, OUTPUT);//define pin as output for 5V for pot
 digitalWrite(vcc2, HIGH);//set vcc2 pin HIGH to have 5V for pot 
 Serial.begin(9600);
 Serial.println("Robojax Stepper Control");
 Serial.print("Max time per rev set: ");
 Serial.print(desiredValue);
 Serial.print(" loop delay: ");
 Serial.println(N);
  delay(4000);//give use time to read
}// setup

 void loop() 
{ 
  // Robojax Stepper code S8BYJ-48
 if(dir ==1){ 
   poleStep++; 
    driveStepper(poleStep);    
 }else if(dir ==2){ 
   poleStep--; 
    driveStepper(poleStep);    
 }else{
  driveStepper(8);   
 }
 if(poleStep>7){ 
   poleStep=0; 
 } 
 if(poleStep<0){ 
   poleStep=7; 
 } 
 int potValue =analogRead(potPin);// read potentiometer value
 int speed = map(potValue,0, 1023, 1,N);

 delay(speed); 


}// loop

int calcDelayfromTime(int t){
  return ((t-5)/4)+1;
}//



/*
 * @brief sends signal to the motor
 * @param "c" is integer representing the pol of motor
 * @return does not return anything
 * 
 * www.Robojax.com code June 2019
 */
void driveStepper(int c)
{
     digitalWrite(Pin1, pole1[c]);  
     digitalWrite(Pin2, pole2[c]); 
     digitalWrite(Pin3, pole3[c]); 
     digitalWrite(Pin4, pole4[c]);   
}//driveStepper end here
