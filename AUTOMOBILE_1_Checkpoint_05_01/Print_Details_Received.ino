/*
 * 
 * FUNZIONE CHE SI OCCUPA DI STAMPARE A SCHERMO ALCUNI CAMPI DEL PACCHETTO RICEVUTO
 * 
 */




void Print_Details_Received ( byte* gotPacket)
{
  Serial.print( " Ricevuto un pacchetto da : " );
  Serial.print(gotPacket[12]);
  Serial.print(".");
  Serial.print(gotPacket[13]);
  Serial.print(".");
  Serial.print(gotPacket[14]);
  Serial.print(".");
  Serial.println(gotPacket[15]);
  Serial.print( " Ricevuto un pacchetto destinato a  : " );
  Serial.print(gotPacket[16]);
  Serial.print(".");
  Serial.print(gotPacket[17]);
  Serial.print(".");
  Serial.print(gotPacket[18]);
  Serial.print(".");
  Serial.println(gotPacket[19]);
  Serial.print( " Payload del messaggio : " );
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
