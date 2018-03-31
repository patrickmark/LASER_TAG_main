void triggers() { // Checks to see if the triggers have been presses
//VL Waffenwechsel wo anderst hin?? 

//-----------------------Waffenwechsel---------------------------//
  if(digitalRead(WeaponChangePin) == LOW){
    myWeaponID++;
    Wechselstatus = 1;
    if(myWeapon>3){
      myWeapon = 1;
    }
    Serial.println("Waffenwechsel");
    Serial.println(myWeaponID);
    Serial.println(digitalRead(WeaponChangePin));
    
    delay(5); // muss noch vergrössert werden!
   }
//------------------------------------------------------------//


//--------------------Waffen Deklaration------------------//
//Pistole
if(myWeaponID == 1 && Wechselstatus ==1){
  maxAmmo = 15;
  Ammo = 15;
  myWeaponHP = 2;
     Wechselstatus = 0;
}

//Sniper
if(myWeaponID == 2 && Wechselstatus ==1){
  maxAmmo = 1;
  Ammo = 1;
  myWeaponHP = 10;
   Wechselstatus = 0;
}


//MG
 if(myWeaponID == 3 && Wechselstatus ==1){
  maxAmmo = 30;
  Ammo = 30;
  myWeaponHP = 1;
   Wechselstatus = 0;
}
//----------------------------------------------------//


//--------------------Nachladen------------------------//
 if(digitalRead(ReloadPin) == LOW &&(Ladestatus==0)){
    Ammo = maxAmmo;
    Ladestatus = 1;
    Serial.println("Nachladen1");
    delay(5);
   }

  if(digitalRead(ReloadPin) == HIGH){
   Ladestatus = 0;
  }
//---------------------------------------------------//


//-------------------Abzug--------------------------//
  LTR = TR;       // Records previous state. Primary fire
  TR = digitalRead(FirePin);      // Looks up current trigger button state

 
  // Code looks for changes in trigger state to give it a semi automatic shooting behaviour
  if(myWeaponID == 1 && TR == LOW && TR != LTR){ //Pistole einzelschuss
    last_interrupt_time=millis();
    if ((last_interrupt_time-interrupt_time)>250)
    {
      Serial.println("trigger");
    FIRE = 1;
    }
  }
  if(myWeaponID == 2 && TR == LOW && TR != LTR){ // ScharfschÃ¼tze einzelschuss
     last_interrupt_time=millis();
    if ((last_interrupt_time-interrupt_time)>350)
    {
    FIRE = 1;
    }
  }
  if(myWeaponID == 3 && TR == LOW){              // MG dauerfeuer
     last_interrupt_time=millis();
    if ((last_interrupt_time-interrupt_time)>350)
    {
    FIRE = 1;
    }
  }
  
  if(FIRE == 1){
    interrupt_time=millis();
    if(Ammo < 1){FIRE = 0;}
    if(health < 1){FIRE = 0; dead();}
    // Fire rate code to be added here  
  }
//---------------------------------------------------------------------// 
}

