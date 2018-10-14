/*
 * 
 * FUNZIONE CHE SI OCCUPA DI INVIARE UN PACCHETTO DI TIPO OSPF
 * 
 * Dato che ne loop sono in ascolto, devo prima terminare la mia fase di ascolto per poter trasmettere
 * 
 * Tale funzione è chiamata dall'ospedale ogni 60 secondi, ogni 60 secondi infatti l'ospedale aggiorna le altre antenne della sua presente
 * 
 * Misuro anche il tempo che intercorre tra l'inizio della trasmissione e la relativa ricezione dell'ack
 * tale informazione sarà utile per poter scegliere la comunicazione con l'antenna migliore
 * ed eventualmente a stabilire se un ospedale è più o meno vicino di un altro
 * 

 *
 */






void Send_Packet (void)
{
  radio.openWritingPipe(pipes[0]);
  radio.openReadingPipe(1,pipes[1]);
  radio.stopListening();                                                                            // First, stop listening so we can talk.  
  Get_Routing_Table();   
  IPv4_Packet_Create(5,IPv4_OSPF_Payload ,IPv4_Address_Ospedale,IPv4_Address_BROADCAST);
  Print_Details_Send ( IPv4_Packet );
  byte gotPacket[27];  
  unsigned long time = micros();                                                                    // Take the time, and send it.  This will block until complete        
  if (!radio.write( &IPv4_Packet, 27 )){                                                            // se la trasmissione del contatore non è andata a buon fine allora stmapa la stringa failed
      Serial.println(F("failed."));      
    }else{

      if(!radio.available()){                                                                       //Se nessuna connessione radio è disponibile dimmi che hai ricevuto un pacchetto bianco
        Serial.println(F("Blank Payload Received.")); 
      }else if (radio.available() )                                                                 //altrimenti se qualcuno ti sta rispondendo memorizzalo in got byte e stampalo a scermo
        {
          unsigned long tim = micros();
          radio.read( &gotPacket, 27 );
         Print_Details_Received ( gotPacket );
         Serial.print("Round trip delay : ");
         Serial.println(tim-time);
        }
      }
  }
