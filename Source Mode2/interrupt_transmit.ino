#ifndef AcceleroMMA7361_h
#define AcceleroMMA7361_h
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#if defined(ARDUINO) && ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#include <pins_arduino.h>
#endif

class AcceleroMMA7361
{
  public:
    AcceleroMMA7361();
    void begin();
    void begin(int sleepPin, int selfTestPin, int zeroGPin, int gSelectPin, int xPin, int yPin, int zPin);
    int getXRaw();
    int getYRaw();
    int getZRaw();
    int getXVolt();
    int getYVolt();
    int getZVolt();
    int getXAccel();
    int getYAccel();
    int getZAccel();
    void getAccelXYZ(int *_XAxis, int *_YAxis, int *_ZAxis);
    int getTotalVector();
    void setOffSets(int xOffSet, int yOffSet, int zOffSet);
    void calibrate();                             // only to be executed when Z-axis is oriented to the ground
// it calculates the offset values by assuming  Z = +1 G ; X and Y  = 0 G
    void setARefVoltage(double _refV);
    void setAveraging(int avg);
    int getOrientation();
    void setSensitivity(boolean sensi);
    void sleep();
    void wake();
    int _mapMMA7361V(int value);
    int _mapMMA7361G(int value);
        int _offSets[3];

  private:

    int _sleepPin;
    int _selfTestPin;
    int _zeroGPin;
    int _gSelectPin;
    int _xPin;
    int _yPin;
    int _zPin;

    int _polarities[3];
    double _refVoltage;
    int _average;
    boolean _sleep;
    boolean _sensi;
};
#endif



// acceleroMMA7361.h - Library for retrieving data from the MMA7361 accelerometer. 
// Copyright 2011-2012 Jef Neefs (neefs@gmail.com) and Jeroen Doggen (jeroendoggen@gmail.com)
// For more information: variable declaration, changelog,... see AcceleroMMA7361.h 
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

/// acceleroMMA7361.cpp - Library for retrieving data from the MMA7361 accelerometer.
/// For more information: variable declaration, changelog,... see AcceleroMMA7361.h


/// constructor
AcceleroMMA7361::AcceleroMMA7361()
{
    _sleepPin=0;
    _selfTestPin=0;
    _zeroGPin=0;
    _gSelectPin=0;
    _xPin=0;
    _yPin=0;
    _zPin=0;
    _offSets[0]=0;
    _offSets[1]=0;
    _offSets[2]=0;
    _polarities[0]=0;
    _polarities[1]=0;
    _polarities[2]=0;
    _refVoltage=0;
    _average=0;
    _sleep=false;
    _sensi=false;
}

/// begin function to set pins: sleepPin = 13, selfTestPin = 12, zeroGPin = 11, gSelectPin = 10, xPin = A0, yPin = A1, zPin = A2.
/// When you use begin() with an empty parameter list, these standard values are used
void AcceleroMMA7361::begin()
{
  begin(13, 12, 11, 10, A0, A1, A2);
}

/// begin variables
/// - int sleepPin: number indicating to which pin the sleep port is attached. DIGITAL OUT
/// - int selfTestPin: number indicating to which pin the selftest port is attached. DIGITAL OUT
/// - int zeroGPin: number indicating to which pin the ZeroGpin is connected to. DIGITAL IN
/// - int gSelectPin: number indication to which pin the Gselect is connected to. DIGITAL OUT
/// - int xPin: number indicating to which pin the x-axis pin is connected to. ANALOG IN
/// - int yPin: number indicating to which pin the y-axis  pin is connected to. ANALOG IN
/// - int zPin: number indicating to which pin the z-axis  pin is connected to. ANALOG IN
/// - int offset: array indicating the G offset on the x,y and z-axis
/// When you use begin() without variables standard values are loaded: A0,A1,A2 as input for X,Y,Z and digital pins 13,12,11,10 for sleep, selftest, zeroG and gSelect
void AcceleroMMA7361::begin(int sleepPin, int selfTestPin, int zeroGPin, int gSelectPin, int xPin, int yPin, int zPin)
{
  pinMode(sleepPin, OUTPUT);
  pinMode(selfTestPin, OUTPUT);
  pinMode(zeroGPin, INPUT);
  pinMode(gSelectPin, OUTPUT);
  pinMode(xPin, INPUT);
  pinMode(yPin, INPUT);
  pinMode(zPin, INPUT);
  digitalWrite(sleepPin,HIGH);
  digitalWrite(selfTestPin,LOW);
  _sleepPin = sleepPin;
  _selfTestPin = selfTestPin;
  _zeroGPin = zeroGPin;
  _gSelectPin = gSelectPin;
  _xPin = xPin;
  _yPin = yPin;
  _zPin = zPin;
  _sleep = false;
  setOffSets(0,0,0);
  setARefVoltage(5);
  setAveraging(10);
  setSensitivity(HIGH);
}

/// setOffSets( int offSetX, int offSetY, int offSetZ): Sets the offset values for the x,y,z axis.
/// The parameters are the offsets expressed in G-force (100 = 1 G)
/// Offsets are added to the raw datafunctions
void AcceleroMMA7361::setOffSets(int xOffSet, int yOffSet, int zOffSet)
{
  if (_refVoltage==3.3)
  {
    _offSets[0]= map(xOffSet,0,3300,0,1024);
    _offSets[1]= map(yOffSet,0,3300,0,1024);
    _offSets[2]= map(zOffSet,0,3300,0,1024);
  }
  else
  {
    _offSets[0]= map(xOffSet,0,5000,0,1024);
    _offSets[1]= map(yOffSet,0,5000,0,1024);
    _offSets[2]= map(zOffSet,0,5000,0,1024);
  }
}

/// setARefVoltage(double _refV): Sets the AREF voltage to external, (now only takes 3.3 or 5 as parameter)
/// default is 5 when no AREF is used. When you want to use 3.3 AREF, put a wire between the AREF pin and the
/// 3.3 V VCC pin. This increases accuracy
void AcceleroMMA7361::setARefVoltage(double refV)
{
  _refVoltage = refV;
  if (refV == 3.3)
  {
    analogReference(EXTERNAL);
  }
}

/// setAveraging(int avg): Sets how many samples have to be averaged in getAccel default is 10.
void AcceleroMMA7361::setAveraging(int avg)
{
  _average = avg;
}

/// setSensitivity sets the sensitivity to +/-1.5 G (HIGH) or +/-6 G (LOW) using a boolean HIGH (1.5 G) or LOW (6 G)
void AcceleroMMA7361::setSensitivity(boolean sensi)
{
  _sensi = sensi;
  digitalWrite(_gSelectPin, !sensi);
}

/// sleep lets the device sleep (when device is sleeping already this does nothing)
void AcceleroMMA7361::sleep()
{
  if (!_sleep)
  {
    digitalWrite(_sleepPin, LOW);
    _sleep = true;
  }
}

/// wake enables the device after sleep (when device is not sleeping this does nothing) there is a 2 ms delay, due to enable response time (datasheet: typ 0.5 ms, max 2 ms)
void AcceleroMMA7361::wake()
{
  if (_sleep == true)
  {
    digitalWrite(_sleepPin, HIGH);
    _sleep = false;
    delay(2);
  }
}

/// getXRaw(): Returns the raw data from the X-axis analog I/O port of the Arduino as an integer
int AcceleroMMA7361::getXRaw()
{
  return analogRead(_xPin)+_offSets[0]+2;
}

/// getYRaw(): Returns the raw data from the Y-axis analog I/O port of the Arduino as an integer
int AcceleroMMA7361::getYRaw()
{
  return analogRead(_yPin)+_offSets[1]+2;
}

/// getZRaw(): Returns the raw data from the Z-axis analog I/O port of the Arduino as an integer
int AcceleroMMA7361::getZRaw()
{
  return analogRead(_zPin)+_offSets[2];
}

/// getXVolt(): Returns the voltage in mV from the X-axis analog I/O port of the Arduino as a integer
int AcceleroMMA7361::getXVolt()
{
  return _mapMMA7361V(getXRaw());
}

/// getYVolt(): Returns the voltage in mV from the Y-axis analog I/O port of the Arduino as a integer
int AcceleroMMA7361::getYVolt()
{
  return _mapMMA7361V(getYRaw());
}

/// getZVolt(): Returns the voltage in mV from the Z-axis analog I/O port of the Arduino as a integer
int AcceleroMMA7361::getZVolt()
{
  return _mapMMA7361V(getZRaw());
}

/// getXAccel(): Returns the acceleration of the X-axis as a int (1 G = 100.00)
int AcceleroMMA7361::getXAccel()
{
   return _mapMMA7361G(getXRaw());
}

/// getYAccel(): Returns the acceleration of the Y-axis as a int (1 G = 100.00)
int AcceleroMMA7361::getYAccel()
{
  
  return _mapMMA7361G(getYRaw());
}

/// getZAccel(): Returns the acceleration of the Z-axis as a int (1 G = 100.00)
int AcceleroMMA7361::getZAccel()
{
 return _mapMMA7361G(getZRaw());
}

/// getAccelXYZ(int *_XAxis, int *_YAxis, int *_ZAxis) returns all axis at once as pointers
void AcceleroMMA7361::getAccelXYZ(int *_XAxis, int *_YAxis, int *_ZAxis)
{
  int sum[3];
  sum[0] = 0;
  sum[1] = 0;
  sum[2] = 0;
  for (int i = 0;i<_average;i++)
  {
    sum[0] = sum[0] + _mapMMA7361G(getXRaw());
    sum[1] = sum[1] + _mapMMA7361G(getYRaw());
    sum[2] = sum[2] + _mapMMA7361G(getZRaw());
  }
  *_XAxis = sum[0]/_average;
  *_YAxis = sum[1]/_average;
  *_ZAxis = sum[2]/_average;
}

/// mapMMA7361V: calculates and returns the voltage value derived from the raw data. Used in getXVoltage, getYVoltage, getZVoltage
int AcceleroMMA7361::_mapMMA7361V(int value)
{
  if (_refVoltage==3.3)
  {
    return map(value,0,1024,0,3300);
  }
  else
  {
    return map(value,0,1024,0,5000);
  }
}

/// mapMMA7361G: calculates and returns the accelerometer value in degrees derived from the raw data. Used in getXAccel, getYAccel, getZAccel
int AcceleroMMA7361::_mapMMA7361G(int value)
{
  if(_sensi == false)
  {
    if (_refVoltage==3.3)
    {
      return map(value,0,1024,-825,800);
    }
    else
    {
      return map(value,0,1024,-800,1600);
    }
  }
  else
  {
    if (_refVoltage==3.3)
    {
      return map(value,0,1024,-206,206);
    }
    else
    {
      return map(value,0,1024,-260,419);
    }
  }
}

/// calibrate(): Sets X and Y values via setOffsets to zero. The Z axis will be set to 100 = 1G
/// WARNING WHEN CALIBRATED YOU HAVE TO MAKE SURE THE Z-AXIS IS PERPENDICULAR WITH THE EARTHS SURFACE
void AcceleroMMA7361::calibrate()
{
  Serial.println(getOrientation());
  Serial.print("\nCalibrating MMA7361011");
  double var = 5000;
  double sumX = 0;
  double sumY = 0;
  double sumZ = 0;
  for (int i = 0;i<var;i++)
  {
    sumX = sumX + getXVolt();
    sumY = sumY + getYVolt();
    sumZ = sumZ + getZVolt();
    if (i%100 == 0)
    {
      Serial.print(".");
    }
  }
  if (_sensi == false)
  {
    setOffSets(1672 - sumX / var,1671 - sumY / var, + 1876 - sumZ / var);
  }
  else
  {
    setOffSets(1650 - sumX / var,1650 - sumY / var, + 2450 - sumZ / var);
  }
  if (abs(getOrientation())!=3)
  {
    Serial.print("\nunable to calibrate");
    setOffSets(0,0,0);
  }
  else
  {
    Serial.print("\nDONE");
  }
}

/// getOrientation returns which axis perpendicular with the earths surface x=1,y=2,z=3 is positive or
/// negative depending on which side of the axis is pointing downwards
int AcceleroMMA7361::getOrientation()
{
  int gemiddelde = 10;
  int x = 0;
  int y = 0;
  int z = 0;
  int xAbs = 0;
  int yAbs = 0;
  int zAbs = 0;
  for(int i = 0; i<gemiddelde ; i++)              //We take in this case 10 measurements to average the error a little bit
  {
    x = x+getXAccel();
    y = y+getYAccel();
    z = z+getZAccel();
  }
  x= x/gemiddelde;
  y = y/gemiddelde;
  z = z/gemiddelde;
  xAbs = abs(100-abs(x));
  yAbs = abs(100-abs(y));
  zAbs = abs(100-abs(z));
  if (xAbs<yAbs&&xAbs<zAbs)
  {
    if (x>0)
    {
      return 1;
    }
    return -1;
    
  }
  if (yAbs<xAbs&&yAbs<zAbs)
  {
    if (y>0)
    {
      return 2;
    }
    return -2;
  }
  if (zAbs<xAbs&&zAbs<yAbs)
  {
    if (z>0)
    {
      return 3;
    }
    return -3;
  }
  return 0;
}

/// getTotalVector returns the magnitude of the total acceleration vector as an integer
int AcceleroMMA7361::getTotalVector()
{
  return sqrt(square(_mapMMA7361G(getXRaw())) + square(_mapMMA7361G(getYRaw())) + square(_mapMMA7361G(getZRaw())));
}




AcceleroMMA7361 accelero;
int changes=0;
volatile bool sampling =false;
byte Stampedsamples[1210];
int next,dum;
int numb=48;
long int starttime;
int datainterrupt=3;
volatile bool datasend=false;
void setup()
{ cbi(ADCSRA, ADPS2);
  sbi(ADCSRA, ADPS1);
  cbi(ADCSRA, ADPS0);
  
  Serial.begin(230400);
  accelero.begin(13, 12, 11, 10, A0, A1, A2);
  accelero.setARefVoltage(5);                   //sets the AREF voltage to 3.3V
  accelero.setSensitivity(LOW);//sets the sensitivity to +/-6G
  
  pinMode(datainterrupt, INPUT);
  
  Serial.println("ready");
  //accelero.calibrate();
  Serial.flush();
  Serial.end();
  // initialize timer1 
  noInterrupts();           // disable all interrupts
  TCCR1A = 0;
  TCCR1B = 0;

  TCNT1 = 65536- 3200 ;         // no prescale , 2.5khz frequency
 TCCR1B |= (1 << CS10); //| (1 << CS10);
// 1 prescaler 
     // enable timer overflow interrupt
              // enable all interrupts
  
 // starttime=micros();
  //interrupts(); 
 

while(digitalRead(datainterrupt)!=HIGH);

attachInterrupt(digitalPinToInterrupt(datainterrupt), dataISR, CHANGE);

TIMSK1 |= (1 << TOIE1);
TCNT1 = 65536- 3200 ; 
interrupts();
}


void dataISR(){
sampling=true;
 
  
}



ISR(TIMER1_OVF_vect)        // interrupt service routine that wraps a user defined function supplied by attachInterrupt
{//interrupts();
  TCNT1 = 65536-3200;  
  //noInterrupts(); 
  
  analogRead(A0);   
  Stampedsamples[next] = analogRead(A0)>>2;
  //Stampedsamples[next]= numb;
  analogRead(A1);  
  Stampedsamples[next+1] = analogRead(A1)>>2 ;
  //Stampedsamples[next+1] =48 + (numb-48+1)%9;
  analogRead(A2);  
  Stampedsamples[next+2]=  analogRead(A2)>>2  ;
  //Stampedsamples[next+2] = 48 + (numb-48+2)%9;
  numb=48 + (numb-48+3)%9; 

  next=next+3;
 if(next==1200){datasend=true;} 
}

  





void loop()
{ 
 if(datasend){
  TIMSK1 =0;
  //noInterrupts();
  Serial.begin(230400);
  //starttime=micros();
 Serial.write("s1");
 dum=next;
 next=0;
 Serial.write(Stampedsamples, dum);
 Serial.write("e1");
 Serial.println(dum);
 //starttime=micros()-starttime;
 //Serial.println(starttime);
 Serial.flush();
 Serial.end();
 sampling=false;
 datasend=false;
  
 
 while(!sampling);
 

 TIMSK1 |= (1 << TOIE1);
 //interrupts();
 
 TCNT1=65536-3200;

 //while(1);
 
}}

