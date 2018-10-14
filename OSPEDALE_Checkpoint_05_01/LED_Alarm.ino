/*
 * 
 * FUNZIONE CHE SI OCCUPOA DI FAR LAMPEGGIARE UN LED SULL'OSPEDALE
 * 
 * 
 */




void LED_Alarm(void)
{
  Serial.print("Auto incidentata: ");
  Serial.print(MAC_ADDRESS_ACCIDENT[0]);
  Serial.print(":");
  Serial.print(MAC_ADDRESS_ACCIDENT[1]);
  Serial.print(":");
  Serial.print(MAC_ADDRESS_ACCIDENT[2]);
  Serial.print(":");
  Serial.print(MAC_ADDRESS_ACCIDENT[3]);
  Serial.print(":");
  Serial.print(MAC_ADDRESS_ACCIDENT[4]);
  Serial.print(":");
  Serial.println(MAC_ADDRESS_ACCIDENT[5]);
  
  for(int i=0;i<10;i++)
  {
    digitalWrite(8,HIGH);
    delay(1000);
    digitalWrite(8,LOW);
    delay(500);
  }
  
}

