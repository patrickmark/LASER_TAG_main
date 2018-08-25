

uint8_t PlayerHitMsg[3];
//void transmitt(uint8_t msg[64]) {

void transmitt() {        //If you get hit send a signal to the shooting person
    
    PlayerHitMsg[0]=(2);                //Update Flag  
    PlayerHitMsg[1]=(player[hitNo]);    //Player ID of the shooting person
    PlayerHitMsg[2]=(myTeamID);         //your team id
    driver.send((uint8_t *)PlayerHitMsg, 3);
    driver.waitPacketSent();
    //Serial.println("Player MSG sent");
}

void shoot() {

  if(FIRE == 1){ // Has the trigger been pressed?
    Serial.println("FIRE 1");
    sendPulse(IRtransmitPin, header); // Transmit Header pulse, send pulse subroutine deals with the details
    delayMicroseconds(IRpulse);
 
     for(int i = 0; i < 8; i++) { // Transmit Byte1  
      if(byte1[i] == 1){
        sendPulse(IRtransmitPin, 1); // Sendet 1 ZEiteinheit wenn Puls byte1 gleich 1
        Serial.print("1 ");
      }
      else{Serial.print("0 ");}   // Sendet 1 ZEiteinheit wenn Puls byte1 gleich 0
      sendPulse(IRtransmitPin, 1);
      delayMicroseconds(IRpulse);
    }

    for(int i = 0; i < 8; i++) { // Transmit Byte2
      if(byte2[i] == 1){
        sendPulse(IRtransmitPin, 1);  // Sendet 1 ZEiteinheit wenn Puls byte2 gleich 1
        Serial.print("1 ");
      }
      else{Serial.print("0 ");}     // Sendet 2 ZEiteinheit wenn Puls byte2 gleich 0
      sendPulse(IRtransmitPin, 1);
      delayMicroseconds(IRpulse);
    }
    
    if(myParity == 1){ // Parity
      sendPulse(IRtransmitPin, 1);    // Sendet Parity
    }
    sendPulse(IRtransmitPin, 1);
    delayMicroseconds(IRpulse);
    
    Serial.println("");
    Serial.println("DONE 1");
    Serial.println(Ammo);
  }
 FIRE = 0;
}

