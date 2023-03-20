#include <Servo.h>
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
String cards[] ={ "53 E2 02 17","5C 9D 09 38"};
String users[] = {"Smart home ","Team"};

Servo tap_servo;
Servo myservo;

int sensor_pin = 2;
int tap_servo_pin =6;
int val;

int Buzzer = 4; // Use buzzer for alert  
int FlamePin = 3;  // This is for input pin
int Flame;

int led = 11; 
int PIR = 5; 
int PIR_val = 0; 
     
void setup(){
  pinMode(sensor_pin,INPUT);
  tap_servo.attach(tap_servo_pin);

  pinMode(Buzzer, OUTPUT);
  pinMode(FlamePin, INPUT);
  Serial.begin(9600);  

  pinMode(led, OUTPUT); 
  pinMode(PIR, INPUT); 

  Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus   
  mfrc522.PCD_Init();   // Initiate MFRC522
  Serial.println("Scan Your Tag...");
  Serial.println();
  myservo.attach(7); 
  
}

void loop(){
  val = digitalRead(sensor_pin);

  if (val==0)
  {tap_servo.write(90);
  }
  if (val==1)
  {tap_servo.write(0);
    }

  Flame = digitalRead(FlamePin);
  if (Flame == 1)
  {
    digitalWrite(Buzzer, HIGH);
  }
  else
  {
    digitalWrite(Buzzer, LOW);
  }
  
  PIR_val = digitalRead(PIR); 
  if (PIR_val == HIGH)
   { 
  digitalWrite(led, HIGH); 
  } 
  else 
  {
  digitalWrite(led, LOW); 
  }

  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent())
  {
    return;
  }
  // Select one of the cards-
  if ( ! mfrc522.PICC_ReadCardSerial())
  {
    return;
  }
  //Show UID on serial monitor
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  if (content.substring(1) == cards[0] )
  {
   
    Serial.println("Access Granted");
    Serial.print("User: ");
    Serial.println(users[0]);
    unlock();
    lock();
    }
    else if (content.substring(1) == cards[1] )
  {
   
    Serial.println("Access Granted");
    Serial.print("User: ");
    Serial.println(users[1]);
    unlock();
    lock();
    }
 else   {
    Serial.println(" Access denied");
    delay(1000);
  }
   
}
void unlock()
{
 for (int pos = 0; pos <= 180; pos += 1) 
     { 
     myservo.write(90);              
     delay(15);                       
     }
 delay(3000);  
}
void lock()
{
   for (int pos = 180; pos >= 0; pos -= 1) { 
    myservo.write(180);              
    delay(15);                       
  }  
}
