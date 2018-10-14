/*
 * 
 * FUNZIONE CHE SI OCCUPA DI INVIARE UN PACCHETTO E SE NECESSARIO RICEVERE IL RISPETTIVO ACK
 * 
 * Dato che ne loop sono in ascolto, devo prima terminare la mia fase di ascolto per poter ricevere
 * 
 * Tale funzione è chiamata dalla autovettura solo quando ha effettuato un incidente
 * Pertanto manderò un pacchetto in broadcast con accident payload
 * 
 * Misuro anche il tempo che intercorre tra l'inizio della trasmissione e la relativa ricezione dell'ack
 * tale informazione sarà utile per poter scegliere la comunicazione con l'antenna migliore
 * 
 * Dato che il pacchetto che sto inviando è di incidente, mi aspetto che l'antenna me lo confermi,
 * pertanto fino a quando non ricevo l'ack del pacchetto inviato ritrasmetto sempre.
 *
 */





void Send_Packet(void)                                      
{
 
    radio.stopListening();                                                                                // First, stop listening so we can talk.
    radio.openWritingPipe(pipes[1]);
    radio.openReadingPipe(1,pipes[0]);  
    IPv4_Packet_Create ( 5 , ACCIDENT_PAYLOAD , IPv4_Address_Car_1 ,IPv4_Address_Antenna );               // Creo il pacchetto IPv4 da inviare  ( TTL , payload , source , destination )                             
    //Print_Details_Send ( IPv4_Packet );
    byte gotPacket[27];  
    unsigned long time = micros();                                                                        // Take the time, and send it.  This will block until complete   
    
    if (!radio.write( &IPv4_Packet, 27 )){                                                                // Se la trasmissione del contatore non è andata a buon fine allora stmapa la stringa failed
      Serial.println(F("failed."));  
      if (counterSend>0)
      {
        counterSend--;
        Send_Packet();    
        
      }
    }else{
      // ASSUME valore TRUE se riceve un payload da leggere
      if(!radio.available()){ 
        if (counterSend2>0)
        {
          counterSend2--;
        Send_Packet();                                                                                    // Se nessuna connessione radio è disponibile dimmi che hai ricevuto un pacchetto bianco     
        Serial.print(F(""));
         
        }
      }
      else if(radio.available() ){                                                                        // altrimenti se qualcuno ti sta rispondendo memorizzalo in got byte e stampalo a scermo
          unsigned long tim = micros();
          radio.read( &gotPacket, 27);
          IPv4_Packet_Read( gotPacket );
          while (( Accident_Confirmed == false )&& (counter_retransmission>=0))                           // Finchè non ricevi conferma che l'antenna abbia ricevuto il tuo messaggio di incidente continua a trasmettere
          {
             counter_retransmission--;
            Send_Packet();
           
          }
          //Print_Details_Received ( gotPacket );
          
          //Serial.print("Round trip delay : ");
          //Serial.println( tim-time);
        }
                                                          
     }
 
}
