/*
 * FUNZIONE CHE SI OCCUPOA DI GENERARE IL PACCHETTO IN FORMATO IPv4
 * 
 * 
 * I parametri da passare alla funzione sono: 
 * Time To Live (TTL) = sarà memorizzato nell'apposito campo ipv4 e decrementato durante il routing del pacchetto al fine di evitarne la permanenza in rete
 * Payload = sarà un byte che rappresenta il primo byte del payload del pacchetto a livello 3
 *       POSSIBILI PAYLOAD UTILIZZATI NELLE AUTOMOBILI
 *              - MAC_ADDRESS = 0x00 = auto risponde al pacchetto di hello (beacon) della antenna con il suo MAC
 *              - ACCIDENT_PAYLOAD = 0xFF = automobile invia questo pacchetto alla antenna quando HA FATTO un incidente
 * Source Address = vettore di 4 byte che contiene l'indirizzo ip del mittente del pacchetto
 *       POSSIBILI SOURCE ADDRESS UTILIZZATI NELLE AUTOMOBILI
 *              - IPv4_Address_Car_1 = 192.128.1.11
 *              - IPv4_Address_Car_2 = 192.128.1.12
 * Destination Address = vettore di 4 byte che contiene l'indirizzo ip del destinatario
 *       POSSIBILI DESTINATION ADDRESS UTILIZZATI DALLE AUTOMOBILI
 *              - IPv4_Address_Antenna = 0.0.0.0         Quando non conosco ancora la antenna
 *              - IPv4_Address_Antenna = 192.128.1.1     Parlo con l'antenna 1
 *              - IPv4_Address_Antenna = 192.128.1.2     Parlo con l'antenna 2
 *              
 * 
 * Tale funzione andrà a modificare un array di 26 byte chiamato IPv4_Packet
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






void IPv4_Packet_Create(byte TTL, byte payload,byte* source_address , byte* destination_address)                              // Assegno a ciascun byte dell'indirizzo ipv6 il suo valore binario corrispondente secondo la convenzione
{
  int checksum=0;
  
  IPv4_Packet[0] = 0x45;                              // 0100 0101 Version + Header Length
  IPv4_Packet[1] = 0x00;                              // 0000 0000 ToS Deprecato non usato
  IPv4_Packet[2] = 0x00;                              // 0000 0000 Lunghezza del pacchetto pt.1
  IPv4_Packet[3] = 0x1B;                              // 0001 1101 Lunghezza del pacchetto pt.2  TOT=7byte
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
  
  checksum = 0;                                      // Reiniziallizo il checksum
  
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
  
  IPv4_Packet[20] = payload;                        // In ogni caso gli ultimi 6 byte del payload inviato dalla macchina sono il suo MAC
  IPv4_Packet[21] = MAC_ADDRESS[0];
  IPv4_Packet[22] = MAC_ADDRESS[1];
  IPv4_Packet[23] = MAC_ADDRESS[2];
  IPv4_Packet[24] = MAC_ADDRESS[3];
  IPv4_Packet[25] = MAC_ADDRESS[4];
  IPv4_Packet[26] = MAC_ADDRESS[5];
}
