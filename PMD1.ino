#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#include<Wire.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(13,7);
SoftwareSerial nodeSerial(3,2);
int sensorValue=0;
int gasSensor=0;
int gasSensor1=0;
char phoneno[50]="";
char phoneno1[50]="";
char PINNUMBER[5]="";
char gasppm[5]="";
byte h=0,v=0;    //variables used in for loops
const unsigned long period=50;  //little period used to prevent error
unsigned long kdelay=0;        // variable used in non-blocking delay 
const byte rows=4;             //number of rows of keypad
const byte columns=4;            //number of columnss of keypad
const byte Output[rows]={12,11,10,9}; //array of pins used as output for rows of keypad
const byte Input[columns]={8,6,5,4}; //array of pins used as input for columnss of keypad
uint64_t phone=0;
String str;
String str1;
String send_data;
int cursorpos=0;

LiquidCrystal_I2C  lcd(0x3F,2,1,0,4,5,6,7);// 0x3F is the I2C bus address for an unmodified module
byte keypad() // function used to detect which button is used 
{
 static bool no_press_flag=0;  //static flag used to ensure no button is pressed 
  for(byte x=0;x<columns;x++)   // for loop used to read all inputs of keypad to ensure no button is pressed 
  {
     if (digitalRead(Input[x])==HIGH);   //read evry input if high continue else break;
     else
     break;
     if(x==(columns-1))   //if no button is pressed 
     {
      no_press_flag=1;
      h=0;
      v=0;
     }
  }
  if(no_press_flag==1) //if no button is pressed 
  {
    for(byte r=0;r<rows;r++) //for loop used to make all output as low
    digitalWrite(Output[r],LOW);
    for(h=0;h<columns;h++)  // for loop to check if one of inputs is low
    {
      if(digitalRead(Input[h])==HIGH) //if specific input is remain high (no press on it) continue
      continue;
      else    //if one of inputs is low
      {
          for (v=0;v<rows;v++)   //for loop used to specify the number of row
          {
          digitalWrite(Output[v],HIGH);   //make specified output as HIGH
          if(digitalRead(Input[h])==HIGH)  //if the input that selected from first sor loop is change to high
          {
            no_press_flag=0;                //reset the no press flag;
            for(byte w=0;w<rows;w++) // make all outputs as low
            digitalWrite(Output[w],LOW);
            return v*4+h;  //return number of button 
          }
          }
      }
    }
  }
 return 50;
}
void setup() 
{
   pinMode(3, OUTPUT);
lcd.setBacklightPin(3,POSITIVE);
lcd.setBacklight(HIGH); // NOTE: You can turn the backlight off by setting it to LOW instead of HIGH
lcd.begin(16, 2);
Serial.begin(2400);
mySerial.begin(9600); 
//Serial.begin(115200);
nodeSerial.begin(115200); 
 //to use serial monitor we set the buad rate  
lcd.setCursor(0,0);
lcd.print("P M D");
delay(1500);
pinMode(A0,INPUT);
lcd.clear();
lcd.print("Sensor Warming");
delay(6000);
  for(byte i=0;i<rows;i++)  //for loop used to make pin mode of outputs as output
  {
  pinMode(Output[i],OUTPUT);
  }
  for(byte s=0;s<columns;s++)  //for loop used to makk pin mode of inputs as inputpullup
  {
    pinMode(Input[s],INPUT_PULLUP);
  }
  

   reading_sensor();
   output();

  
}

void loop()
{
  lcd.setCursor(cursorpos,1);
switch(keypad()){
  
    
    case 0: 
     phone=phone * 10 + 1;
     lcd.print("1");
     cursorpos++;
    break;

    case 1: 
     phone=phone * 10 + 2;
     lcd.print("2");
     cursorpos++;
     break;
     
    case 2:
     phone=phone * 10 + 3;
     lcd.print("3");
     cursorpos++;
     break;
     
    case 4: 
     phone=phone * 10 + 4;
     lcd.print("4");
     cursorpos++;
     break;
     
    case 5: 
     phone=phone * 10 + 5;
     lcd.print("5");
     cursorpos++;
     break;
     
    case 6: 
     phone=phone * 10 + 6;
     lcd.print("6");
     cursorpos++;
     break;
     
    case 8:
     phone=phone * 10 + 7;
     lcd.print("7");
     cursorpos++;
     break;
     
    case 9:
     phone=phone * 10 + 8;
     lcd.print("8");
     cursorpos++;
     break;

     case 10:
     phone=phone * 10 + 9;
     lcd.print("9");
     cursorpos++;
     break;

      case 13:
     phone=phone * 10 + 0;
     lcd.print("0");
     cursorpos++;
     break;

     case 3:
     setup();
     loop();
     break;

//case to del single character.
     case 11:
     phone=phone/10;
   lcd.setCursor(cursorpos-1,1);
   lcd.print(" ");
   cursorpos=cursorpos-1;
     break;

    case 15:
  
    send_message(); 
      

    break;
    
   }
 }

void output()
{
  
  
  if(gasSensor>=400)
{
  
   digitalWrite(3, HIGH);  
  lcd.setBacklight(HIGH); // NOTE: You can turn the backlight off by setting it to LOW instead of HIGH
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("High Alert");
  lcd.setCursor(0,1);
  lcd.print("ArQ=");
  lcd.print(gasSensor,DEC);
  lcd.print(" PPM");
  delay(1000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Enter Phone no>");
 loop();
   digitalWrite(3,LOW);

}
else
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Good Air Quality");
   lcd.setCursor(0,1);
  lcd.print("ArQ=");
  lcd.print(gasSensor,DEC);
lcd.print(" PPM");

  }
 
  }

int reading_sensor()
{
  for(int k=0;k<50;k++)
{
sensorValue = analogRead(A0);
lcd.clear();
lcd.setCursor(0,0);
lcd.print("ArQ=");
lcd.print(sensorValue,DEC);
lcd.print(" PPM");
lcd.println("       "); 
lcd.print("  ");
delay(100);
  }

gasSensor=sensorValue;
return gasSensor;
loop();
  }
  

String ToString(uint64_t x)
{
     boolean flag = false; // For preventing string return like this 0000123, with a lot of zeros in front.
     String str = "";      // Start with an empty string.
     uint64_t y = 10000000000000000000;
     int res;
     if (x == 0)  // if x = 0 and this is not testet, then function return a empty string.
     {
           str = "0";
           return str;  // or return "0";
     }   
     while (y > 0)
     {               
            res = (int)(x / y);
            if (res > 0)  // Wait for res > 0, then start adding to string.
                flag = true;
            if (flag == true)
                str = str + String(res);
            x = x - (y * (uint64_t)res);  // Subtract res times * y from x
            y = y / 10;                   // Reducer y with 10   
     }
     return str;
} 


int readSerial(char result[]) {
  int i = 0;
  while (1) {
    while (Serial.available() > 0) {
      char inChar = Serial.read();
      if (inChar == '\n') {
        result[i] = '\0';
        Serial.flush();
        return 0;
      }
      if (inChar != '\r') {
        result[i] = inChar;
        i++;
      }
    }
  }
}

void send_message()
{
  gasSensor1=gasSensor;
  str1=String(gasSensor);
  str1.toCharArray(gasppm,5);
  Serial.print(gasppm);
  //nodeSerial.write(gasppm);
  
    lcd.clear();
     lcd.setCursor(0,0);
     str=ToString(phone);
     lcd.print(str);
     str.toCharArray(phoneno,12);
     send_data= str1+","+str;
     nodeSerial.println(send_data);
     delay(100);
     send_data="";
     lcd.setCursor(0,1);
     mySerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
     delay(2000);  // Delay of 1 second
     lcd.print("Message Sending");
     delay(1000);
     lcd.clear();
    lcd.setCursor(0,0);
     sprintf(phoneno1,"AT+CMGS=\"+91%s\" \r" ,phoneno);
      mySerial.println(phoneno1); // Replace x with mobile number
     delay(1000);
     mySerial.println("High Alert!!");// The SMS text you want to send
     mySerial.print("\n Airquality: ");
     mySerial.print(gasSensor);
     mySerial.print(" PPM");
     mySerial.println("\n Repair Your car as soon as Possible");
     delay(100);
     mySerial.println((char)26);// ASCII code of CTRL+Z for saying the end of sms to  the module 
      delay(1000);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Message Sent");
      cursorpos=0;
      phone=0;
      str="";
  }

