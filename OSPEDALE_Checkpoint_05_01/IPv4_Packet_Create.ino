/*
 * FUNZIONE CHE SI OCCUPOA DI GENERARE IL PACCHETTO IN FORMATO IPv4
 * 
 * 
 * I parametri da passare alla funzione sono: 
 * Time To Live (TTL) = sarà memorizzato nell'apposito campo ipv4 e decrementato durante il routing del pacchetto al fine di evitarne la permanenza in rete
 * Payload = sarà un vettore di byte che rappresentano il payload del pacchetto a livello 3
 *       POSSIBILI PAYLOAD UTILIZZATI NELLA ANTENNA
 *              - OSPF_Packet = 0xF0 così setto il primo byte del payload
 * Source Address = vettore di 4 byte che contiene l'indirizzo ip del mittente del pacchetto
 *       POSSIBILI SOURCE ADDRESS UTILIZZATI NELLE ANTENNE
 *              - IPv4_Address_Ospedale = 192.128.1.3          
 * Destination Address = vettore di 4 byte che contiene l'indirizzo ip del destinatario
 *       POSSIBILI DESTINATION ADDRESS UTILIZZATI DALLE ANTENNE
 *              - IPv4_Address_BROADCAST = 255.255.255.255       invio il mio messaggio di OSPF in broadcast
 *              
 * 
 * Tale funzione andrà a modificare un array di 27 byte chiamato IPv4_Packet
 * che sarà trasmesso come payload del pacchetto a livello 2.
 * 
 * Si è utilizzato il protocollo IPv4 piuttosto che IPv6 data l'incompatibilita nella dimensione del pacchetto da trasmettere con l'hardware scelto
 * 
 * Ogni volta che creo un nuovo pacchetto IPv4 azzero il checksum precedente
 * ricalcolo il checksum con una somma di tutti i campi precedenti e la salvo nel campo checksum
 * 
 * 
 * Assegno infine source address, destination address e payload ricevuti in ingresso
 *
 */




void IPv4_Packet_Create(byte TTL, byte payload[],byte source_address[], byte destination_address[])                              // Assegno a ciascun byte dell'indirizzo ipv6 il suo valore binario corrispondente secondo la convenzione
{
  int checksum=0;
  
  IPv4_Packet[0] = 0x45;                              // 0100 0101 Version + Header Length
  IPv4_Packet[1] = 0x00;                              // 0000 0000 ToS Deprecato non usato
  IPv4_Packet[2] = 0x00;                              // 0000 0000 Lunghezza del pacchetto pt.1
  IPv4_Packet[3] = 0x1B;                              // 0001 1011 Lunghezza del pacchetto pt.2  TOT payload=7byte
  IPv4_Packet[4] = 0x00;                              // 0000 0000 Inizio Riga di Fragmentation, not used
  IPv4_Packet[5] = 0x00;                              // 0000 0000 
  IPv4_Packet[6] = 0x00;                              // 0000 0000
  IPv4_Packet[7] = 0x00;                              // 0000 0000
  IPv4_Packet[8] = TTL;                               // 0000 0101 Impongo il massimo numero di salti pari a 5 generalmente
  IPv4_Packet[9] = 0x06;                              // Campo Protocol settato a 6 = TCP

  // Creo il campo checksum fatto da 16 bit
  for( int i=0 ; i<=9 ; i++ )
  {
    checksum = checksum + IPv4_Packet[i]; 
  }
  
  IPv4_Packet[10] = 0x00;                            // Header Checksum pt.1
  IPv4_Packet[11] = checksum;                        // Header Checsum pt.2

  checksum = 0;                                      // Riazzero il checksum

  
  // SOURCE ADDRESS
  IPv4_Packet[12] = source_address[0];
  IPv4_Packet[13] = source_address[1]; 
  IPv4_Packet[14] = source_address[2];
  IPv4_Packet[15] = source_address[3]; 

  // DESTINATION ADDRESS
  IPv4_Packet[16] = destination_address[0];
  IPv4_Packet[17] = destination_address[1];
  IPv4_Packet[18] = destination_address[2];
  IPv4_Packet[19] = destination_address[3];

  // PAYLOAD
  IPv4_Packet[20] = payload[0];
  IPv4_Packet[21] = payload[1];
  IPv4_Packet[22] = payload[2];
  IPv4_Packet[23] = payload[3];
  IPv4_Packet[24] = payload[4];
  IPv4_Packet[25] = payload[5];
  IPv4_Packet[26] = payload[6];
}
