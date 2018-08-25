
/*
In this section all relevant code for battery monitoring, protection and arduino shutdown should be placed!
Planed:  
        * readVcc (for correct Vbat measuring with a Voltage devider)  //done
        * read Vbat 
        * Arduino shutdown (...not really figured out how to do this (maybe low power relay oder Mosfet oder TR) or just a sleep mode and a warning to signal the user sth is wrong...)
        * warnings if battery Volgate is low
*/



////////READ VCC FOR ACCURATE ADC VOLTAGE MEASUREMENTS/////////////
long readVcc() { 
  long result;
  // Read 1.1V reference against AVcc
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Convert
  while (bit_is_set(ADCSRA,ADSC));
  result = ADCL;
  result |= ADCH<<8;
  result = 1125300L / result; // Back-calculate AVcc in mV
  return result;
} 

