

int decider = 6;
int count = 0;
long int time4 = 0, starttime = 0;
void setup() {
  // put your setup code here, to run once:

pinMode(decider, OUTPUT);
digitalWrite(decider, LOW);
Serial.begin(230400);
Serial.println("central started. Waiting for 20seconds");
Serial.end();
delay(20000);
//Timer1.initialize(41000); // set a timer of length 100000 microseconds (or 0.1 sec - or 10Hz => the led will blink 5 times, 5 cycles of on-and-off, per second)
//Timer1.attachInterrupt( timerIsr ); // attach the service routine here
// Set CS01 and CS00 bits for 64 prescaler
noInterrupts();           // disable all interrupts
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 65536- 62500 ;          // no prescale , 1/0.041 Hz frequency
  TCCR1B |= (1 << CS11) | (1 << CS10);   // setting prescale = 64
// 1 prescaler 
  TIMSK1 |= (1 << TOIE1);   // enable timer overflow interrupt
 // accelero.calibrate();
   starttime=micros();   
  interrupts(); 
}

void loop() {
  // put your main code here, to run repeatedly:
}

ISR(TIMER1_OVF_vect)
{
    TCNT1 = 65536-62500; 
      digitalWrite( decider, !(digitalRead(decider)) );
    Serial.begin(230400);
      time4 = micros()-starttime;
      Serial.println(time4);
     Serial.flush();
     
     Serial.end(); 
    
  

}

