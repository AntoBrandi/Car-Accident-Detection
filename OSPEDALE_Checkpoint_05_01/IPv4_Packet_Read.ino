/*
 * 
 * FUNZIONE CHE SI OCCUPA DI LEGGERE E DECIFRARE IL PACCHETTO IPv4
 * 
 * Effettuo delle verifiche preeliminari sul pacchetto per valutare se:
 *            - Sia corretto :: valuto il checksum allo stesso modo con cui lo ho creato :: CORRECT = true
 *            - Sia destinato a me :: valuto se il destination address sia compatibile con i miei :: FOR_ME = true
 *                 POSSIBILI DESTINATION ADDRESS RICONOSCIUTI
 *                        - IPv4_Address_Car_1 = 192.168.1.3           Sono in ascolto sul mio personale ip (Ospedale)
 *                        - IPv4_Address_BROADCAST = 255.255.255.255   In ascolto sull'indirizzo di broadcast dove verrò a conoscenza se una auto ha avuto un incidente
 *            - Abbia un payload noto :: leggo il payload per capire cosa vogliono dirmi di fare
 *                 POSSIBILI PAYLOAD RICONOSCIUTI
 *                        - OSPF_Payload = 0xF0                 Payload inviato dalle antenne in discovery (Hello)      TO_ACK=true; ACCIDENT_COMUNICATION=false; ACCIDENT_CONFIRMED=false;
 *                        - ACCIDENT_PAYLOAD = 0xFF             Payload inviato da una auto incidentata                 TO_ACK=false; ACCIDENT_COMUNICATION=true; ACCIDENT_CONFIRMED=false;
 *                        
 * Uso delle variabili booleane per memorizzare lo status di cosa accade durante la decodifica del pacchetto           
 * In questo modo nella funzione loop posso decidere cosa fare interrogando le variabili booleane che funzionano 
 * da indice di cosa sia contenuto nel pacchetto ipv4 appena ricevuto
 *           
 */







void IPv4_Packet_Read( byte* gotPacket)
{
  For_Me = false;
  To_Ack=false;
  int checksum = 0;
  Accident_Comunication = false;
  Correct = false;
  
  // Verifico che sia corretto il checksum
  for( int i=0 ; i<=9 ; i++)                                                                 // Calcolo il checksum dell'header
  {
    checksum = checksum + gotPacket[i];  
  }
    if( (gotPacket[10] == 0x00) && (gotPacket[11] == checksum))
    {
      Correct = true;
    }
    else {
      Correct = false;
    }

    checksum = 0;                                                                             // Riazzero il checksum
// Fine verifica del checksum


// Se non ci sono errori posso leggere il pacchetto
if (Correct == true)                                                                          // Se non ci sono errori nell'header
{

  
  // Valuto se sono io il destinatario IP Accettabili (192.168.1.3  | 255.255.255.255)    // SOLO SU 255
  if ( ((gotPacket[16]== 0xFF) && (gotPacket[17]== 0xFF) && (gotPacket[18]== 0xFF) && (gotPacket[19]== 0xFF)) || ((gotPacket[16]== 192) && (gotPacket[17]== 168) && (gotPacket[18]== 1) && (gotPacket[19]== 1)) ||( (gotPacket[16]== 192) && (gotPacket[17]== 168) && (gotPacket[18]== 1) && (gotPacket[19]== 2))) 
  {
    For_Me = true;
  }
  else 
  {
    For_Me = false;
  }

  
// Se sono effettivamente io il destinatario posso elaborare il pacchetto
  if( For_Me == true )
  { // Valuto se il payload sia un messaggio di INCIDENTE
    if (gotPacket[20] == 0xFF) // MESSAGGIO DI incidente
    {
      if ((MAC_ADDRESS_ACCIDENT[0] == gotPacket[21]) && (MAC_ADDRESS_ACCIDENT[1] == gotPacket[22]) && (MAC_ADDRESS_ACCIDENT[2] = gotPacket[23]) && (MAC_ADDRESS_ACCIDENT[3] = gotPacket[24]) && (MAC_ADDRESS_ACCIDENT[4] = gotPacket[25]) && (MAC_ADDRESS_ACCIDENT[5] = gotPacket[26]))
      {
        To_Ack=false;
        Accident_Comunication=false;
      }
      else
      {
      To_Ack = false;                                                                             // Questo pacchetto è relativo ad un incidente quindi devo confermarlo
      Accident_Comunication = true;
      MAC_ADDRESS_ACCIDENT[0] = gotPacket[21];
      MAC_ADDRESS_ACCIDENT[1] = gotPacket[22];
      MAC_ADDRESS_ACCIDENT[2] = gotPacket[23];
      MAC_ADDRESS_ACCIDENT[3] = gotPacket[24];
      MAC_ADDRESS_ACCIDENT[4] = gotPacket[25];
      MAC_ADDRESS_ACCIDENT[5] = gotPacket[26];
                                                         
      IPv4_Address_Antenna[0] = gotPacket[12];
      IPv4_Address_Antenna[1] = gotPacket[13];
      IPv4_Address_Antenna[2] = gotPacket[14];
      IPv4_Address_Antenna[3] = gotPacket[15];
    }
    }
    
    else if ( gotPacket[20] == 0xF0)                                                                // Se non sto ricevendo nulla tra le cose precedenti significa che è un MAC ADDRESS
    {
      // OSPF 
      // Salvataggio o confronto in tabella di routing statico
      Update_Routing_Table(gotPacket);
      Accident_Comunication = false; 
      To_Ack = false ;                                                                              // sto già ricevendo un ack            
    }
    
    }
  else{
      Serial.println("PACCHETTO NON PER ME");
    }
    
  }
  else {
    Serial.println("CHECKSUM ERRATO");
  }
  
}
