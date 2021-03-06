


/*
LASER TAG by Martin (alias King of ninjas) & Patrick (der eigentlich als einziger  den Code versteht) ;p

latest version: 2018/03/30

Notes: 

Debug mode: 1 ... aktiv ... in der Konsole kann mittels eingagbe (in funktion definiert) befehle ausgeführt werden

TODO   * prototyp bauen 
       * reichweite mit neuer linses -> testen
       * reichweite mit RF (momentan nur ein paar meter!)
       * master pogramm zur einrichung des spiels -- ?
       * weitere kommentare
       * Akkuüberwachung und abschaltung (angefangen)
       * #defines einführen und evtl auslagern in defines.h
       * visuelle Ausagebe (health, shot, hit...) angefangen
       * audio angefangen

       !!  my trigger funktion momentan in verwendung !!
*/
/*
#define TX_RX_LED_INIT  DDRD |= (1<<5), DDRB |= (1<<0)
#define TXLED1          PORTD |= (1<<5)
#define TXLED0          PORTD &= ~(1<<5)
#define RXLED1          PORTB |= (1<<0)
#define RXLED0          PORTB &= ~(1<<0)
*/



#include<EEPROM.h>
//#include <VirtualWire.h>
#include <avr/sleep.h>

#include <RH_ASK.h>
#include <SPI.h> // Not actualy used but needed to compile

//RH_ASK driver (RF T/R)
RH_ASK driver(1000, 5, 4, 6); // ESP8266: do not use pin 11 // 5:rx; 4:tx

//debugging
int debug                  = 1;
int debugmsg               = 0;

//IOs

int FirePin                = 3;      // Schiessen. Low = pressed
int WeaponChangePin        = 12;     // Waffenwechsel. Low = pressed
int ReloadPin              = 10;     // Nachladen
int hitPin                 = 13;     // Buzzer for tones
int BuzzerPin              = 9;     // Buzzer for tones
int IRtransmitPin          = 2;      // Primary fire mode IR transmitter pin: Use pins 2,4,7,8,12 or 13. DO NOT USE PWM pins!! More info: http://j44industries.blogspot.com/2009/09/arduino-frequency-generation.html#more
int IRreceivePin           = 11;     // The pin that incoming IR signals are read from
int StatPin1               = 6;      // Status Leds for indicating HP,Ammo,Weapon
int StatPin2               = 7;      // Status Leds for indicating HP,Ammo,Weapon
int StatPin3               = 8;      // Status Leds for indicating HP,Ammo,Weapon
int VbatPin                = A0;     // Battery Voltage Pin


// Player and Game details
int myTeamID               = 2;      // 1-7 (0 = system message)
int myPlayerID             = 4;      // Player ID (0-31)
int myGameID               = 1;      // Interprited by configureGane subroutine; allows for quick change of game types.
int myWeaponID             = 1;      // Deffined by gameType and configureGame subroutine.
int myWeaponHP             = 0;      // Deffined by gameType and configureGame subroutine.
int maxAmmo                = 0;      // Deffined by gameType and configureGame subroutine.
int maxHealth              = 1;      // Deffined by gameType and configureGame subroutine.
int automatic              = 0;      // Deffined by gameType and configureGame subroutine. Automatic fire 0 = Semi Auto, 1 = Fully Auto.
int myWeapon               = 1;      // Weapon Pistol sniper or mashiengun
int Ladestatus             = 0;      // Verhindert dauerhaftes Laden
int Wechselstatus          = 0;      // Verhindert dauerhaftes Wechseln
int HitsPlayer[31];                  // Gibt an wie oft man von welchem Spieler getroffen wurde
int HitEnemy               = 0;
int HitFriend              = 0;

int eepromUpdate           = 0;

//Waffen Deklaration
int MaxAmmo[3]={15,1,40};
int WeaponHP[3]={3,10,1};
int ammo[3]={MaxAmmo[0],MaxAmmo[1],MaxAmmo[2]};
int WeaponFireRate[3]={250,1200,50};                               //waiting time in ms (250 maybe to low ->ERROR) (weapon1,weapon2, weapon3)
int ChangeReload_Time[2]={3000,1500};                               //waiting time after Weaponchange and Reload in ms (change,reload)

int Ammo                   = 0;      // Current ammunition
int health                 = 30;      // Current health

//Incoming signal Details
int received[18];                    // Received data: received[0] = which sensor, received[1] - [17] byte1 byte2 parity (Miles Tag structure)
int check                  = 0;      // Variable used in parity checking

// Code Variables
int timeOut                = 0;      // Deffined in frequencyCalculations (IRpulse + 50)
int FIRE                   = 0;      // 0 = don't fire, 1 = Primary Fire, 2 = Secondary Fire(not in use)
int TR                     = 0;      // Trigger Reading
int LTR                    = 0;      // Last Trigger Reading

// Signal Properties
int IRpulse                = 1500;    // Basic pulse duration of 600uS MilesTag standard 4*IRpulse for header bit, 2*IRpulse for 1, 1*IRpulse for 0.
int IRfrequency            = 38;     // Frequency in kHz Standard values are: 38kHz, 40kHz. Choose dependant on your receiver characteristics
int IRt                    = 0;      // LED on time to give correct transmission frequency, calculated in setup.
int IRpulses               = 0;      // Number of oscillations needed to make a full IRpulse, calculated in setup.
int header                 = 6;      // Header lenght in pulses. 4 = Miles tag standard

// Transmission data
int byte1[8];                        // String for storing byte1 of the data which gets transmitted when the player fires.
int byte2[8];                        // String for storing byte2 of the data which gets transmitted when the player fires.
int myParity               = 0;      // String for storing parity of the data which gets transmitted when the player fires.

// Received data
int memory                 = 10;     // Number of signals to be recorded: Allows for the game data to be reviewed after the game, no provision for transmitting / accessing it yet though.
int hitNo                  = 0;      // Hit number
// Byte1
int player[10];                      // Array must be as large as memory
int team[10];                        // Array must be as large as memory
// Byte2
int weapon[10];                      // Array must be as large as memory
int hp[10];                          // Array must be as large as memory //stores the damage, the last hit makes 
int parity[10];                      // Array must be as large as memory

//for debouncing
unsigned long last_interrupt_time = 0;
unsigned long interrupt_time = 0;

//for RF
uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];
uint8_t buflen = sizeof(buf);

float Vcc=0;
float Vbat=0;
int counter1 = 0;
int gothitflag=0;
int youhitflag=0;
int reloadflag=0;
int toneshootflag=0;

void setup() {
  // Serial coms set up to help with debugging.
  Serial.begin(115200);
  Serial.println("Startup...");
  Serial.print("Reading Startup VCC... "); 
  Vcc = readVcc() / 1000.0;  
  Serial.println(Vcc);
  
  Serial.print("Reading Startup Vbat... ");
  Vbat = readVbat();  
  Serial.println(Vbat);
  
  if (!driver.init())
  Serial.println("init failed");
         
  // Pin declarations
  pinMode(FirePin, INPUT_PULLUP);
  pinMode(WeaponChangePin, INPUT_PULLUP);
  pinMode(ReloadPin, INPUT_PULLUP);
  pinMode(hitPin, OUTPUT);
  pinMode(StatPin1, OUTPUT);
  pinMode(StatPin2, OUTPUT);
  pinMode(StatPin3, OUTPUT);
  pinMode(IRtransmitPin, OUTPUT);
  pinMode(IRreceivePin, INPUT);
  
 
  frequencyCalculations();        // Calculates pulse lengths etc for desired frequency
  configureGame();                // Look up and configure game details
  //tagCode();                      // Based on game details etc works out the data that will be transmitted when a shot is fired

  digitalWrite(FirePin, HIGH);              // Not really needed if your circuit has the correct pull up resistors already but doesn't harm
  digitalWrite(WeaponChangePin, HIGH);      // Not really needed if your circuit has the correct pull up resistors already but doesn't harm
  digitalWrite(ReloadPin, HIGH);            // Not really needed if your circuit has the correct pull up resistors already but doesn't harm

 
  //first time load default weapon id      STILL EXPERIMENTAL!!!!
  myWeaponHP =WeaponHP[myWeaponID-1];
  maxAmmo = MaxAmmo[myWeaponID-1];
  Ammo = ammo[myWeaponID-1];

  Serial.print("Player ID ");  Serial.println(myPlayerID);
  Serial.print("Team ID ");  Serial.println(myTeamID);
  Serial.print("Game Mode ");  Serial.println(myGameID);
  Serial.print("Weapon ID ");  Serial.println(myWeaponID);
  Serial.print("Weapon HP ");  Serial.println(myWeaponHP);
  Serial.println("Ready....");
}
  unsigned long temptime1=0; //einen werrt zuweisen um beim ersten durchgang keine ungewolten dinge zu tun
  unsigned long temptime2=0;
  unsigned long temptime3=0;
  unsigned long temptime4=0;
  int note=3000;


void loop() {
 
////////////////////// COUNTER AND TIMER SECTION //////////////////// 

  counter1++;
  if (counter1==100) checkBat();      //if counter1 = 100 check Battery (counter var is set to 0 in checkBat()
  
//signalise you got hit 
  if(gothitflag==1) {temptime1=millis();  digitalWrite(StatPin3,HIGH);  note=800;  gothitflag=2;
  }
   if ((millis()- temptime1) < 50 && gothitflag==2) {tone(BuzzerPin, note, 1); note -=10;}
   if ((millis()- temptime1) >1000 && gothitflag==2) {digitalWrite(StatPin3,LOW); gothitflag=0;}
  
//signalise you hit sb.
  if(youhitflag==1) {temptime2=millis(); digitalWrite(StatPin2,HIGH); youhitflag=2;
    }
  if ((millis()- temptime2) < 100  && youhitflag==2) {tone(BuzzerPin, note, 1); note -=5;}
  if ((millis()- temptime2) > 1000 && youhitflag==2) {digitalWrite(StatPin2,LOW); youhitflag=0;}
 
 //signalise ran out of ammo  
  if(reloadflag==1) {temptime3=millis();  digitalWrite(StatPin3,HIGH);  note=500;  reloadflag=2;
    }
   if ((millis()- temptime3) < 50  && reloadflag==2) {tone(BuzzerPin, note, 1);}
   else if ((millis()- temptime3) < 150  && reloadflag==2) {}
   else if ((millis()- temptime3) < 250  && reloadflag==2) {tone(BuzzerPin, note, 1);}
   if (200 < millis()- temptime3 && reloadflag==2) {digitalWrite(StatPin3,LOW); reloadflag =3;}
   if (400 < millis()- temptime3 && reloadflag==3) { digitalWrite(StatPin3,HIGH); reloadflag =4;}
   if (600 < millis()- temptime3 && reloadflag==4) {digitalWrite(StatPin3,LOW); reloadflag =5;}
   if (800 < millis()- temptime3 && reloadflag==5) {digitalWrite(StatPin3,HIGH); reloadflag =6;}
   if (1000 < millis()- temptime3 && reloadflag==6) {digitalWrite(StatPin3,LOW); reloadflag =0;} 
   
  ////////////buzzer////////////////
  if(toneshootflag==1) {temptime4=millis(); note=1500;  toneshootflag=2;
  } 
    else if (millis()- temptime4 < 30 && toneshootflag==2) {tone(BuzzerPin, note, 1); note +=10;}
    else if (millis()- temptime4 < 150 && toneshootflag==2) {tone(BuzzerPin, note, 1); note -=5;}
  

///////////////////////CODE SECTION ////////////////////
  //Serial.println(millis());
  receiveIR(); //receive IR data (in this funktion also "interpritReceived" is called)
  receiveRF();
  if (FIRE != 0) {
    tagCode();              // works out which data will be transmitted when a shot is fired
    shoot();
    toneshootflag=1;
  }
  my_trigger();
  interpritReceivedRF();
  //Serial.println("rec"); 
  //Serial.print("Team: "); Serial.println(myTeamID);
  
////////////////////DEBUGGING SECITION /////////////////

if (debug==1){
 debugmsg = 0;
 int shootflag=0;
 int reloadflag=0;
 int wpchangeflag=0;
 
 if (Serial.available() > 0) {
   debugmsg = Serial.read();
   if( debugmsg==115){ shootflag=1; FIRE =1;}               //s
   if( debugmsg==114){ reloadflag=1; Ammo = maxAmmo; }      //r
   if( debugmsg==119){ wpchangeflag=1;}                     //w
   if( debugmsg==104){ }     //h
   if( debugmsg=='a'){  
     Serial.print("Reading VCC... "); 
     Vcc = readVcc() / 1000.0;  
     Serial.println(Vcc);
     Serial.print("Reading Vbat... ");
     Vbat = readVbat();  
     Serial.println(Vbat);
     checkBat();
                     }
   //Serial.println(debugmsg); 
   }
  
  }
  
}

//TeamID, GameID und PlayerID mÃ¼ssen hÃ¤ndisch oben definiert werden, spÃ¤ter Programm fÃ¼r zugriff von ausen
//einbindung von Display und Lautsprecher fehlen
//kabellose Ã¼bertragung, nicht mehr wie 3-4m











