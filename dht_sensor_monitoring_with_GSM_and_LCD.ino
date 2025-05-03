// Include Software Serial library to communicate with GSM
#include <LiquidCrystal_I2C.h>
#include "DHT.h"
#include <SoftwareSerial.h>
SoftwareSerial SIM900(7, 8); // RX (8), TX (7) for GSM SIM900A

int DHTPIN= 2;

// set the LCD number of columns and rows
int lcdColumns = 16;
int lcdRows = 2;
// set LCD address, number of columns and rows
// if you don't know your display address, run an I2C scanner sketch
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);  


#define DHTTYPE DHT11   // DHT 11
DHT dht(DHTPIN, DHTTYPE);


float temp; //to store the temperature value
float hum; // to store the humidity value


char mobile_no[] = "+9XXXXXXXX"; //change with phone number to which you want to call use the country code for example(+92XXXXXXXX)


// Variable to store text message
String textMessage;

// Create a variable to store Lamp state
String message;



void setup() {
  
  dht.begin();          //Begins to receive Temperature and humidity values.  
  Serial.begin(9600); 
   // initialize LCD
  lcd.init();
  // turn on LCD backlight                      
  lcd.backlight();
  SIM900.begin(9600);

  // Give time to your GSM shield log on to network
  delay(20000);
  Serial.print("SIM900 ready...");
  // AT command to set SIM900 to SMS mode
  SIM900.print("AT+CMGF=1\r"); 
  delay(1000);
//SIM900.print("AT+CNMI=2,2,0,0,0\r");
  delay(1000);
SIM900.print("AT+CNMI=1,2,0,0,0");
   delay(5000);
  Serial.println("Ready...");
}

void loop(){
   readDHTsensor();
  if(SIM900.available()>0){
 
    textMessage = SIM900.readString();
    Serial.print(textMessage); 
    textMessage.toUpperCase();    
    delay(2000);
  } 
  
  if(textMessage.indexOf("DHT")>=0){
    
    String message = ("Temperature: " + String(temp) + "*C " + " Humidity: " + String(hum) + "%");
    sendSMS(message);
    textMessage = "";
    message=""; 
  }

  

} 

void  readDHTsensor(){
temp = dht.readTemperature();
 hum = dht.readHumidity(); 
 lcd.print("Temp: ");
 lcd.print(temp );  // Prints the temperature value from the sensor
    lcd.print(" ");
    lcd.print((char)223);  //shows degrees character
    lcd.print("C");

      lcd.setCursor(0, 1);
  lcd.print("Humidity: ");
  lcd.print( hum);
  lcd.print("%");
 Serial.print("temperature = ");
 Serial.println(temp);
 Serial.print("humidity = ");
 Serial.println(hum);
 if(temp>35)
 {
  message="Temperature Exceeded";
   sendSMS(message);
   message="";
   delay(3000);
  }
  delay(1000);
 
  }

// Function that sends SMS
void sendSMS(String message){
  // AT command to set SIM900 to SMS mode
  SIM900.print("AT+CMGF=1\r"); 
  delay(1000);

  // REPLACE THE X's WITH THE RECIPIENT'S MOBILE NUMBER
  // USE INTERNATIONAL FORMAT CODE FOR MOBILE NUMBERS
  SIM900.println("AT + CMGS = \"+9XXXXXXXX\""); 
  delay(1000);
  // Send the SMS
  SIM900.println(message); 
  delay(100);
  // End AT command with a ^Z, ASCII code 26
  SIM900.println((char)26); 
  delay(1000);
  SIM900.println();
  // Give module time to send SMS
  delay(5000);  
}
