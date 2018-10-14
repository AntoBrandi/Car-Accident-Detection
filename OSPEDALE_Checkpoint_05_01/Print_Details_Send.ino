/*
 * 
 * FUNZIONE CHE SI OCCUPA DI STAMPARE A SCHERMO ALCUNI CAMPI DEL PACCHETTO INVIATO
 * 
 */





void Print_Details_Send( byte* gotPacket )
{
   Serial.print( " Inviato un pacchetto da: " );
  Serial.print(gotPacket[12]);
  Serial.print(".");
  Serial.print(gotPacket[13]);
  Serial.print(".");
  Serial.print(gotPacket[14]);
  Serial.print(".");
  Serial.println(gotPacket[15]);
  Serial.print( " Inviato un pacchetto verso : " );
  Serial.print(gotPacket[16]);
  Serial.print(".");
  Serial.print(gotPacket[17]);
  Serial.print(".");
  Serial.print(gotPacket[18]);
  Serial.print(".");
  Serial.println(gotPacket[19]);
  Serial.print( " Message Payload contains : " );
  Serial.print(gotPacket[20]);
  Serial.print(" ");
  Serial.print(gotPacket[21]);
  Serial.print(" ");
  Serial.print(gotPacket[22]);
  Serial.print(" ");
  Serial.print(gotPacket[23]);
  Serial.print(" ");
  Serial.print(gotPacket[24]);
  Serial.print(" ");
  Serial.print(gotPacket[25]);
  Serial.print(" ");
  Serial.println(gotPacket[26]);
}
