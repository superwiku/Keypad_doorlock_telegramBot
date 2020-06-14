#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <Servo.h>
#include <Keypad.h>
#include "CTBot.h"
#include "RTClib.h"
#define relay D0
LiquidCrystal_I2C lcd(0x27, 16, 2);
RTC_DS3231 rtc;

CTBot myBot;
String ssid = "superwiku";
String pass = "7129wiku";
String token = "1095665862:AAEIRJp-qg4MCpl2PvX-u2PjdMiSfmvBOSk";
const int id = 896305362;

const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] = { 
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'}, 
  {'*','0','#'}
};

byte rowPins[ROWS] = {D1, D2, D3, D5};
byte colPins[COLS] = {D6, D7, D8};
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
char namaHari[7][12] = {"Minggu", "Senin", "Selasa", "Rabu", "Kamis", "Jumat", "Sabtu"};
String passs;
String pesan;

void setup() {
  Serial.begin(115200);  
  Wire.begin(D2,D1);
  lcd.begin();  
  
  pinMode(relay, OUTPUT);
  digitalWrite(relay, HIGH);
   
  myBot.wifiConnect(ssid, pass);
  myBot.setTelegramToken(token);
  myBot.sendMessage(id, "Penjaga Pintu siaapp...");
  lcd.setCursor(1,0);
  lcd.print("ENTER PASSWORD");
  keypad.addEventListener(keypadEvent);
  
}
 
void loop() {
// if(!myBot.testConnection()){ 
//  reconnect();  
//  Serial.println("terkoneksi kembali");
//  delay(1000);
// }
  char keynya=keypad.getKey(); 
  if(isDigit(keynya)){    
        passs+=keynya;
        Serial.println(passs);
  }
}

void keypadEvent(KeypadEvent keynya){
  DateTime now= rtc.now();
  switch(keypad.getState()){
    case PRESSED :
      switch(keynya){
        case '*':
          lcd.setCursor(0,0);
          lcd.print(passs);
          if(passs=="7129"){
            lcd.clear();
            lcd.setCursor(2,0);lcd.print("PASSWORD OK!");
            lcd.setCursor(1,1);lcd.print("SILAHKAN MASUK");           
            digitalWrite(relay, LOW);  
            delay(2000);    
            lcd.clear();   
            lcd.setCursor(2,1);lcd.print("TERIMA KASIH");    
            digitalWrite(relay, HIGH);
            myBot.sendMessage(id,"Pintu dibuka dengan pass = " +passs+ " on "+namaHari[now.dayOfTheWeek()]+" , "+now.day()+"-"+now.month()+"-"+now.year()+"  "
            +printAngka(now.hour())+":"+printAngka(now.minute())+":"+printAngka(now.second()));
            lcd.clear();
            lcd.setCursor(1,0);lcd.print("ENTER PASSWORD");
          }else{
            lcd.clear();
            lcd.setCursor(3,0);lcd.print("PASS= "+passs);   
            lcd.setCursor(1,1);lcd.print("PASSWORD SALAH");  
            myBot.sendMessage(id,"Password salah = " +passs+ " on "+namaHari[now.dayOfTheWeek()]+" , "+now.day()+"-"+now.month()+"-"+now.year()+"  "
            +printAngka(now.hour())+":"+printAngka(now.minute())+":"+printAngka(now.second())); 
            lcd.clear();
            lcd.setCursor(1,0);lcd.print("ENTER PASSWORD");
          }
          passs="";
          break;
        case '0':
          TBMessage msg;
          if(myBot.getNewMessage(msg)){
            if(msg.text.equalsIgnoreCase("buka")){
              lcd.clear();
              lcd.setCursor(1,0);
              lcd.print("DIBUKA DGN BOT");
              lcd.setCursor(1,1);
              lcd.print("SILAHKAN MASUK");           
              digitalWrite(relay, LOW);  
              delay(2000);    
              lcd.clear();   
              lcd.setCursor(2,1);
              lcd.print("TERIMA KASIH");    
              digitalWrite(relay, HIGH);
              myBot.sendMessage(id,"Pintu dibuka dengan BOT = " +passs+ " on "+namaHari[now.dayOfTheWeek()]+" , "+now.day()+"-"+now.month()+"-"+now.year()+"  "
              +printAngka(now.hour())+":"+printAngka(now.minute())+":"+printAngka(now.second()));
              delay(1000); 
              passs="";
              lcd.clear();
              lcd.setCursor(1,0);
              lcd.print("ENTER PASSWORD"); 
            }else{
              lcd.clear();
              lcd.setCursor(3,0);
              lcd.print("TIDAK ADA");
              lcd.setCursor(2,1);
              lcd.print("PESAN MASUK");  
              passs=""; 
              delay(3000);    
              lcd.clear();
              lcd.setCursor(1,0);
              lcd.print("ENTER PASSWORD");     
            }
          }else{
              lcd.clear();
              lcd.setCursor(3,0);
              lcd.print("TIDAK ADA");
              lcd.setCursor(2,1);
              lcd.print("PESAN MASUK");  
              passs="";
              delay(3000);    
              lcd.clear();
              lcd.setCursor(1,0);
              lcd.print("ENTER PASSWORD");   
            }
          break;
      }
   }
}  

String printAngka(int digits){
   if(digits < 10){
    String digitss=String(digits);
    return ("0"+digitss);
   }
   else return String(digits);
}

void reconnect(){
  myBot.wifiConnect(ssid, pass);  
  myBot.setTelegramToken(token);
}
