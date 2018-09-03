
//STILL EXPERMITENAL!

void my_trigger() { // Checks to see if the trigger, weaponchange or reload have been pressed; 
//-----------------------Waffenwechsel---------------------------//
  if(digitalRead(WeaponChangePin) == LOW){
    
    last_interrupt_time=millis();
    if ((last_interrupt_time-interrupt_time)>ChangeReload_Time[2])
    {    
          myWeaponID++;
          if(myWeaponID>3){
            myWeaponID = 1;
          }        
          //change weapon settings to current setings
          myWeaponHP =WeaponHP[myWeaponID-1];
          maxAmmo = MaxAmmo[myWeaponID-1];
          Ammo = ammo[myWeaponID-1];
          
          Serial.println("Waffenwechsel");
          Serial.println(myWeaponID);
          Serial.println(digitalRead(WeaponChangePin));
     }  
     interrupt_time=millis();
   }
//-----------------------------------------------------//
//--------------------Nachladen------------------------//
 if(digitalRead(ReloadPin) == LOW){
    last_interrupt_time=millis();
    if ((last_interrupt_time-interrupt_time)>ChangeReload_Time[1])
    {
          Ammo = maxAmmo;
          Serial.println("Nachladen");         
    }
    interrupt_time=millis();
  }
//---------------------------------------------------//
//-------------------Abzug---------------------------//

  LTR = TR;                       // Records previous state. Primary fire
  TR = digitalRead(FirePin);      // Looks up current trigger button state

  // Code looks for changes in trigger state to give it a semi automatic shooting behaviour
  if(myWeaponID == 1 && TR == LOW && TR != LTR){ //Pistole einzelschuss
    //Serial.println("trigger");
    last_interrupt_time=millis();
    if ((last_interrupt_time-interrupt_time)>WeaponFireRate[1])            FIRE = 1;

  }
  if(myWeaponID == 2 && TR == LOW && TR != LTR){ // ScharfschÃ¼tze einzelschuss
     last_interrupt_time=millis();
    if ((last_interrupt_time-interrupt_time)>WeaponFireRate[2])            FIRE = 1;

  }
  if(myWeaponID == 3 && TR == LOW){              // MG dauerfeuer
     last_interrupt_time=millis();
    if ((last_interrupt_time-interrupt_time)>WeaponFireRate[3])            FIRE = 1;
  }
  
  if(FIRE == 1){
    interrupt_time=millis();
    if (Ammo<=0){ Serial.println("no Ammo!"); reloadflag=1;}
      else{
        ammo[myWeaponID-1]--;       
        Ammo--;
      }
    if(Ammo < 1){FIRE = 0; }
    if(health < 1){FIRE = 0; dead();}
    // Fire rate code to be added here  
  }
//---------------------------------------------------------------------// 
}

