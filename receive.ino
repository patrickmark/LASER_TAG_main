
void receiveRF()
{

    if (driver.recv(buf, &buflen)) // Non-blocking
    {
	// Message with a good checksum received, dump it.
	driver.printBuffer("Got:", buf, buflen);
     }
}


//Empfänger
void receiveIR() { // Void checks for an incoming signal and decodes it if it sees one.
  int error = 0;
  
  
  if(digitalRead(IRreceivePin) == LOW){    // If the receive pin is low a signal is being received.
  //  digitalWrite(hitPin,HIGH);  //??
 
    while(digitalRead(IRreceivePin) == LOW){}
    
    
    for(int i = 1; i <= 17; i++) {                        // Repeats several times to make sure the whole signal has been received
      received[i] = pulseIn(IRreceivePin, LOW, timeOut);  // pulseIn command waits for a pulse and then records its duration in microseconds.
      //Serial.print("t= ");Serial.print(received[i]);  
    }
    
   //Serial.println("");
   
   // Serial.print(received[0]); 
   // Serial.print("...");
   int receivedTemp[18];
    for(int i = 1; i <= 17; i++) {  // Looks at each one of the received pulses
      
      receivedTemp[i] = 2;
      if(received[i] > (IRpulse - 200) &&  received[i] < (IRpulse + 800)) {receivedTemp[i] = 0;}                      // Works out from the pulse length if it was a data 1 or 0 that was received writes result to receivedTemp string
      if(received[i] > (IRpulse + IRpulse - 200) &&  received[i] < (IRpulse + IRpulse + 800)) {receivedTemp[i] = 1;}  // Works out from the pulse length if it was a data 1 or 0 that was received  
      received[i] = 3;                   // Wipes raw received data
      received[i] = receivedTemp[i];     // Inputs interpreted data
     
     // Serial.print(" ");
     // Serial.print(received[i]);         // Print interpreted data results
    }
    //Serial.println("");                  // New line to tidy up printed results
   
    // Parity Check. Was the data received a valid signal?
    check = 0;
    for(int i = 1; i <= 16; i++) { 
      if(received[i] == 1){check = check + 1;}
      if(received[i] == 2){error = 1;}
    }
    // Serial.println(check);
    check = check >> 0 & B1;
    // Serial.println(check);
    if(check != received[17])  error = 1;
    if(error == 0){
      Serial.println("Valid Signal");
      interpritReceived();
      //digitalWrite(hitPin,HIGH);  //indicate you got hit ... is set to low in interpret
                  }
                  
    else  Serial.println("ERROR");
  }
}

