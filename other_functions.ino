void sendPulse(int pin, int length){ // importing variables like this allows for secondary fire modes etc.
// This void genertates the carrier frequency for the information to be transmitted
  int i = 0;
  int o = 0;
  while( i < length ){
    i++;
    o=0;
    while( o < IRpulses ){
      o++;
      digitalWrite(pin, HIGH);
      delayMicroseconds(IRt);
      digitalWrite(pin, LOW);
      delayMicroseconds(IRt);
    }
  }
}

  


void configureGame() { // Spieleinstellungen, spÃ¤ter zugriff von ausen Ã¼ber PC
  if(myGameID == 0){    
    maxHealth = 3;
    health = 3;
  
  }
  if(myGameID == 1){
    maxHealth = 30;
    health = 30;
   
  }
}


void frequencyCalculations() { // Works out all the timings needed to give the correct carrier frequency for the IR signal
  IRt = (int) (500/IRfrequency);  
  IRpulses = (int) (IRpulse / (2*IRt));
  IRt = IRt - 4;
  // Why -4 I hear you cry. It allows for the time taken for commands to be executed.
  // More info: http://j44industries.blogspot.com/2009/09/arduino-frequency-generation.html#more

  Serial.print("Oscilation time period /2: ");
  Serial.println(IRt);
  Serial.print("Pulses: ");
  Serial.println(IRpulses);
  timeOut = 2* IRpulse + 500; // Adding 100 to the expected pulse time gives a little margin for error on the pulse read time out value
}

void tagCode() { // Works out what the players tagger code (the code that is transmitted when they shoot) is
  byte1[0] = myTeamID >> 2 & B1;
  byte1[1] = myTeamID >> 1 & B1;
  byte1[2] = myTeamID >> 0 & B1;

  byte1[3] = myPlayerID >> 4 & B1;
  byte1[4] = myPlayerID >> 3 & B1;
  byte1[5] = myPlayerID >> 2 & B1;
  byte1[6] = myPlayerID >> 1 & B1;
  byte1[7] = myPlayerID >> 0 & B1;


  byte2[0] = myWeaponID >> 2 & B1;
  byte2[1] = myWeaponID >> 1 & B1;
  byte2[2] = myWeaponID >> 0 & B1;

  byte2[3] = myWeaponHP >> 4 & B1;
  byte2[4] = myWeaponHP >> 3 & B1;
  byte2[5] = myWeaponHP >> 2 & B1;
  byte2[6] = myWeaponHP >> 1 & B1;
  byte2[7] = myWeaponHP >> 0 & B1;

  myParity = 0;
  for (int i=0; i<8; i++) {
   if(byte1[i] == 1){myParity = myParity + 1;}
   if(byte2[i] == 1){myParity = myParity + 1;}
   myParity = myParity >> 0 & B1;
  }

  // Next few lines print the full tagger code.
  Serial.print("Byte1: ");
  Serial.print(byte1[0]);
  Serial.print(byte1[1]);
  Serial.print(byte1[2]);
  Serial.print(byte1[3]);
  Serial.print(byte1[4]);
  Serial.print(byte1[5]);
  Serial.print(byte1[6]);
  Serial.print(byte1[7]);
  Serial.println();

  Serial.print("Byte2: ");
  Serial.print(byte2[0]);
  Serial.print(byte2[1]);
  Serial.print(byte2[2]);
  Serial.print(byte2[3]);
  Serial.print(byte2[4]);
  Serial.print(byte2[5]);
  Serial.print(byte2[6]);
  Serial.print(byte2[7]);
  Serial.println();

  Serial.print("Parity: ");
  Serial.print(myParity);
  Serial.println();
}

void hit() { // Make some noise and flash some lights when you get shot
  digitalWrite(hitPin,HIGH);
  health = health - hp[hitNo];    //??
  Serial.print("hp hitNR: ");    //debunging
  Serial.println(hp[hitNo]);      // debungging
    Serial.println(hp[hitNo-1]);      // debungging
      Serial.println(hp[hitNo+1]);      // debungging
  Serial.print("health: ");
  Serial.println(health);
  if(health <= 0){dead();}
  digitalWrite(hitPin,LOW);
 
}

void dead() { // void determines what the tagger does when it is out of lives
  // Makes a few noises and flashes some lights
  Serial.println("DEAD");
 
  for (int i=0; i<10; i++) {
   digitalWrite(hitPin,HIGH);
   delay (500);
   digitalWrite(hitPin,LOW);
   delay (500);
  }
}

void LEDReload() {
  
  digitalWrite(StatPin1,HIGH);
  digitalWrite(StatPin2,HIGH);
  digitalWrite(StatPin3,HIGH);
  delay(200);
  
  digitalWrite(StatPin1,LOW);
  digitalWrite(StatPin2,LOW);
  digitalWrite(StatPin3,LOW);
  delay(100);
  
  digitalWrite(StatPin1,HIGH);
  digitalWrite(StatPin2,HIGH);
  digitalWrite(StatPin3,HIGH);
  delay(200);
  
  digitalWrite(StatPin1,LOW);
  digitalWrite(StatPin2,LOW);
  digitalWrite(StatPin3,LOW);
  
}

void LEDHit() {
  
  digitalWrite(StatPin1,HIGH);
  digitalWrite(StatPin2,HIGH);
  digitalWrite(StatPin3,HIGH);
  delay(1000);
  
  digitalWrite(StatPin1,LOW);
  digitalWrite(StatPin2,LOW);
  digitalWrite(StatPin3,LOW);
  
}


