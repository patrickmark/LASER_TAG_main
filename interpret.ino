
void interpritReceivedRF(){  
  
  //Message from Master (buff[0]=1)
  if(buf[0]==1){
  myTeamID=buf[myPlayerID+1]+1;
  myGameID=buf[1];  
  buf[0]=0;
  }
  
  //Message from other player (buff[0]=2)
  HitFriend=0;
  HitEnemy=0;
  if(buf[0]==2&&buf[1]==myPlayerID){ 
    buf[0]=0;   
    if(buf[2]!=myTeamID)  {HitFriend=1; Serial.println("Target Hit");  LEDHit();}
    else {HitEnemy=1;Serial.println("FRIENDLY FIRE!!!");}
  }
  
  
}

void interpritReceived(){  // After a message has been received by the ReceiveIR subroutine this subroutine decidedes how it should react to the data

  if(hitNo == memory){hitNo = 0;} // hitNo sorts out where the data should be stored if statement means old data gets overwritten if too much is received
  team[hitNo] = 0;
  player[hitNo] = 0;
  weapon[hitNo] = 0;
  hp[hitNo] = 0;
  // Next few lines Effectivly converts the binary data into decimal
  if(received[1] == 1){team[hitNo] = team[hitNo] + 4;}    
  if(received[2] == 1){team[hitNo] = team[hitNo] + 2;}
  if(received[3] == 1){team[hitNo] = team[hitNo] + 1;} 

  if(received[4] == 1){player[hitNo] = player[hitNo] + 16;}
  if(received[5] == 1){player[hitNo] = player[hitNo] + 8;}
  if(received[6] == 1){player[hitNo] = player[hitNo] + 4;}
  if(received[7] == 1){player[hitNo] = player[hitNo] + 2;}
  if(received[8] == 1){player[hitNo] = player[hitNo] + 1;}
   
  if(received[9] == 1){weapon[hitNo] = weapon[hitNo] + 4;}
  if(received[10] == 1){weapon[hitNo] = weapon[hitNo] + 2;}
  if(received[11] == 1){weapon[hitNo] = weapon[hitNo] + 1;} 

  if(received[12] == 1){hp[hitNo] = hp[hitNo] + 16;}
  if(received[13] == 1){hp[hitNo] = hp[hitNo] + 8;}
  if(received[14] == 1){hp[hitNo] = hp[hitNo] + 4;}
  if(received[15] == 1){hp[hitNo] = hp[hitNo] + 2;}
  if(received[16] == 1){hp[hitNo] = hp[hitNo] + 1;}
   
  parity[hitNo] = received[17];

  HitsPlayer[player[hitNo]] =  HitsPlayer[player[hitNo]] + hp[hitNo]; // Addiert den Schaden der von jedem Spieler erliden wurde zusammen

    for(int i = 0; i < 8; i++) { // gibt erlitenen Schaden pro Spieler aus
         Serial.print(i);
         Serial.print("Player ");
         Serial.print(HitsPlayer[i]);
       
      }

  Serial.print("Hit No: ");
  Serial.print(hitNo);
  Serial.print("  Player: ");
  Serial.print(player[hitNo]);
  Serial.print("  Team: ");
  Serial.print(team[hitNo]);
  Serial.print("  Weapon: ");
  Serial.print(weapon[hitNo]);
  Serial.print("  HP: ");
  Serial.print(hp[hitNo]);
  Serial.print("  Parity: ");
  Serial.println(parity[hitNo]);
 
  transmitt();
  //This is probably where more code should be added to expand the game capabilities at the moment the code just checks that the received data was not a system message and deducts a life if it wasn't.
  if (player[hitNo] != 0){hit();}
  hitNo++ ;
}


