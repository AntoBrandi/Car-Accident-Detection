/*
 * 
 * FUNZIONE CHE SI OCCUPA DI LEGGERE E DECIFRARE IL PACCHETTO IPv4
 * 
 * Effettuo delle verifiche preeliminari sul pacchetto per valutare se:
 *            - Sia corretto :: valuto il checksum allo stesso modo con cui lo ho creato :: CORRECT = true
 *            - Sia destinato a me :: valuto se il destination address sia compatibile con i miei :: FOR_ME = true
 *                 POSSIBILI DESTINATION ADDRESS RICONOSCIUTI
 *                        - IPv4_Address_Car_1 = 192.168.1.11   Sono in ascolto sul mio personale ip (Auto1)
 *                        - IPv4_Address_Car_2 = 192.168.1.12   Sono in ascolto sul mio personale ip (Auto2)
 *                        - IPv4_Address_Discovery = 0.0.0.0    Quano la antenna non mi conosce invia beacon con questo ip
 *                        - IPv4_Address_BROADCAST = 255.255.255.255   In ascolto sull'indirizzo di broadcast dove verrò a conoscenza se altre auto nella zona hanno fatto incidente
 *            - Abbia un payload noto :: leggo il payload per capire cosa vogliono dirmi di fare
 *                 POSSIBILI PAYLOAD RICONOSCIUTI
 *                        - Hello_Message = 0x000000000000F0      Payload inviato dalle antenne in discovery (Hello)      TO_ACK=true; ACCIDENT_COMUNICATION=false; ACCIDENT_CONFIRMED=false;
 *                        - ACCIDENT_PAYLOAD = 0xFF   Payload inviato da una auto incidentata                 TO_ACK=false; ACCIDENT_COMUNICATION=true; ACCIDENT_CONFIRMED=false;
 *                        - Accident_Confirmation = 0x000000000000 Payload inviato dalla antenna alla auto per confermare di avr ricevuto il suo incidente TO_ACK=false; ACCIDENT_COMUNICATION=false; ACCIDENT_CONFIRMED=true;
 *             
 * Uso delle variabili booleane per memorizzare lo status di cosa accade durante la decodifica del pacchetto           
 * In questo modo nella funzione loop posso decidere cosa fare interrogando le variabili booleane che funzionano 
 * da indice di cosa sia contenuto nel pacchetto ipv4 appena ricevuto
 *           
 */





void IPv4_Packet_Read ( byte* gotPacket )
{ 
  // SETUP DELLA FUNZIONE
  For_Me = false;
  Correct = false;
  To_Ack = false;
  Accident_Comunication = false;
  Accident_Confirmed = false; 
  int checksum = 0 ;                                         // Lo reinizializzo ogni volta a 0

  
  // Verifico che sia corretto il checksum
  for ( int i=0 ; i<=9 ; i++ )                               // Calcolo il checksum dell'header
  {
    checksum = checksum + gotPacket[i];  
  }
  if( ( gotPacket[10] == 0x00 ) && ( gotPacket[11] == checksum ) )
    {
      Correct = true;
    }
  else {
      Correct = false;
    }
    checksum = 0;                                              // Riazzero il checksum ogni volta
// Fine verifica del checksum


// Se non ci sono errori posso leggere il pacchetto
if (Correct == true)             // Se non ci sono errori nell'header
{

  
  // Valuto se sono io il destinatario IP Accettabili (192.168.1.11 | 0.0.0.10 | 255.255.255 )
  if( ((gotPacket[16] ==  IPv4_Address_Car_1[0]) && (gotPacket[17] == IPv4_Address_Car_1[1]) && (gotPacket[18] == IPv4_Address_Car_1[2]) && (gotPacket[19] == IPv4_Address_Antenna[3])) || ((gotPacket[16] == 0x00) && (gotPacket[17] == 0x00) && (gotPacket[18] == 0x00) & (gotPacket[19] == 0x0A)) || ((gotPacket[16] == 0xFF) && (gotPacket[17] == 0xFF) && (gotPacket[18] == 0xFF) & (gotPacket[19] == 0xFF)) )
  {
    For_Me = true;
  }
  else 
  {
    For_Me = false;
  }

  
// Se sono effettivamente io il destinatario posso elaborare il pacchetto
  if( For_Me == true )
  { // Valuto se il payload sia un messaggio di HELLO ( 0x0000000000F0 )
    if ( (gotPacket[20] == 0x00) && (gotPacket[21] == 0x00) && (gotPacket[22] == 0x00) && (gotPacket[23] == 0x00) && (gotPacket[24] == 0x00) && (gotPacket[25] == 0x00) && (gotPacket[26] == 0xF0))   // MESSAGGIO DI HELLO dalla antenna
    {
      // Messaggio di HELLO lo ha mandato l'antenna
      IPv4_Address_Antenna[0] = gotPacket[12];
      IPv4_Address_Antenna[1] = gotPacket[13];
      IPv4_Address_Antenna[2] = gotPacket[14];
      IPv4_Address_Antenna[3] = gotPacket[15];
      To_Ack = true;                                    // Questo pacchetto mi è stato inviato da una antenna quindi devo confermarlo
      Accident_Comunication = false;
      Accident_Confirmed = false; 
    }
    // Valuto se il messaggio sia di avvenuto incidente ( 0xFF primo byte )
    else if (gotPacket[20] == 0xFF)
    {
      Accident_Comunication = true;                    // Qualcuno mi sta comunicando che c'è stato un incidente
      To_Ack = false;                                  // Non mando l0'ack in questa circostanza
      Accident_Confirmed = false; 
    }
    // Valuto se il messaggio ricevuto è la conferma che l'antenna ha ricevuto l'incidente
    else if ((gotPacket[20] == 0x00) && (gotPacket[21] == 0x00) && (gotPacket[22] == 0x00) && (gotPacket[23] == 0x00) && (gotPacket[24] == 0x00) && (gotPacket[25] == 0x00) && (gotPacket[26] == 0x00))
    {
      Accident_Comunication = false;                   // Qualcuno mi sta comunicando che c'è stato un incidente
      Accident_Confirmed = true;
      To_Ack = false;                                  // Non mando l'ack in questa circostanza
    }
    else if ((gotPacket[20] == 0xF0) && (gotPacket[16] == 0xFF) && (gotPacket[17] == 0xFF) && (gotPacket[18] == 0xFF) & (gotPacket[19] == 0xFF))
    {
      Accident_Comunication = false;  
      Accident_Confirmed = false; 
      To_Ack = false; 
      //Serial.println(" Pacchetto OSPF non per me"); 
    }
    else             // Se non sto ricevendo nulla tra le cose precedenti significa che è un MAC ADDRESS
    {
      MAC_ADDRESS_RECEIVED[0] = gotPacket[21];                
      MAC_ADDRESS_RECEIVED[1] = gotPacket[22];
      MAC_ADDRESS_RECEIVED[2] = gotPacket[23];                
      MAC_ADDRESS_RECEIVED[3] = gotPacket[24];
      MAC_ADDRESS_RECEIVED[4] = gotPacket[25];  
      MAC_ADDRESS_RECEIVED[5] = gotPacket[26];
      Accident_Comunication = false;  
      Accident_Confirmed = false; 
      To_Ack = false;          
    }
    
    }
  else{
     // Serial.println("PACCHETTO NON PER ME");
    }
    
  }
  else {
   // Serial.println("CHECKSUM ERRATO");
  }
  
}

