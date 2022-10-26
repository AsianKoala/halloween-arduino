int ledPin = 13;                // choose the pin for the LED
int pirPin = 10;               // choose the input pin (for PIR sensor)
int pirState = LOW;             // we start, assuming no motion detected
int pirValue; // Place to store read PIR Value

int runtime = 5000;
int stoptime = 1000;

int val;
int RPWM=3;
int LPWM=11;
int L_EN=7;
int R_EN=8;

void setPWMfrequency(int freq){
  TCCR1B = TCCR2B & 0b11111000 | freq ;
  TCCR3B = TCCR2B & 0b11111000 | freq ;
}

void MotorActiveStatus(char Side,boolean s){
  boolean state=s;
  if(Side=='R'){
    digitalWrite(R_EN,s);
  }
  if(Side=='L'){
    digitalWrite(L_EN,s);
  }    
}

void setMotor(char side,byte pwm){
  if(side=='R'){
  analogWrite(RPWM,pwm);
  }
  if(side=='L'){
    analogWrite(LPWM,pwm);
  }
}

void closeMotor(char side){
  if(side=='R'){
    digitalWrite(RPWM,LOW);
  }
  if(side=='L'){
    digitalWrite(LPWM,LOW);
  }
}

void setup() {
  setPWMfrequency(0x02);// timer 2 , 3.92KHz
  for(int i=5;i<9;i++) {
    pinMode(i,OUTPUT);
  }

  for(int i=5;i<9;i++) {
    digitalWrite(i,LOW);
  }

  delay(1000);
  MotorActiveStatus('R',true);
  MotorActiveStatus('L',true);

  pinMode(pirPin, INPUT);
  Serial.begin(9600);
}

void doMotor() {
  if(pirState == HIGH) {
    Serial.println("Motion detected, starting motor");
    for(int i=0;i<256;i++){
      setMotor('R',i);
      delay(1);
    }
    delay(runtime);
    Serial.println("Stopping forward movement");
    closeMotor('R');
    delay(stoptime);
    Serial.println("Reversing motor");
    for(int i=0;i<256;i++){
      setMotor('L',i);
      delay(1);
    }
    delay(runtime);
    closeMotor('L');
    delay(stoptime);
    Serial.println("Stopping reverse movement");
  }
}

void readPir() {
  val = digitalRead(pirPin);  
  if (val == HIGH) {            
    digitalWrite(ledPin, HIGH); 
    if (pirState == LOW) {
      Serial.println("Motion detected!");
      pirState = HIGH;
      doMotor();
    }
  } else {
    digitalWrite(ledPin, LOW); 
    if (pirState == HIGH){
      Serial.println("Motion ended!");
      pirState = LOW;
    }
  }
}

void loop() {
  readPir();
}


