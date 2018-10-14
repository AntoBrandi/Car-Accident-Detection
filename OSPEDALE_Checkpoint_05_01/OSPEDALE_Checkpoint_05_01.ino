/*
 * Scketch relativo all'antenna disposta in un OSpedale
 * 
 * Ciascun ospedale avrà una antenna ed una tabella di routing statico.
 * L'aggiornamento della tabella è effettuato mediante il protocollo OSPF 
 * E' disposto l'invio di un messaggio di tipo OSPF ogni 60 secondi .
 * L'ospedale in questo modo comunica alle antenne che lo circondano di essere sempre presente ed attivo.
 * Di seguito ciascuna antenna conoscerà la posizione dell'ospedale presente nella propria tabella di routing
 * e sarà in grado di inoltrare un messaggio di incidente.
 * L'ospedale sarà in ascolto al suo indirizzo IP e sull'indirizzo broadcast
 * 
 * Se ricevo dalle antenne vicine la comunicazione di un incidente , allora faccio lampeggiare un LED
 * 
 */


#include <nRF24L01.h>
#include <printf.h>
#include <RF24.h>
#include <RF24_config.h>

RF24 radio(9,10); // CE , CSN

// Topology
const uint64_t pipes[2] = { 0xABCDABCD71LL, 0x544d52687CLL };              // Radio indirizzi per la comuniazione tra due antenne
// PIPES ( Antenna, Auto)
//Gestione dei ruoli tra antenne, una trasmette e l'altra riceve, uso lo stesso codice per entrame le antenne. 
//ciascuna antenna potrà quindi sia trasmettere che ricevere

unsigned int OSPF_Counter = 60000;                                        // LO FA OGNI MINUTO


// IPv4 implementation
const byte MAC_ADDRESS[6]={0xAB,0xCD,0xAB,0xCD,0xEF,0xFF};                // MAC dell'ospedale
const byte ACK_After_Accident[7] = {0x00, 0x00 , 0x00,0x00,0x00,0x00,0x00};
const byte IPv4_Address_Ospedale[4] = {192,168,1,3};                      // Indirizzo Ip dell'ospedale
byte MAC_ADDRESS_ACCIDENT[6];                                             // Indirizzo MAC della auto che ha avuto un incidente
byte IPv4_Packet[27];                                                     // Indirizzo in formato IPV4 che userò per creare il pacchetto di livello 3
byte IPv4_Packet_Received[27];                                            // Pacchetto che ricevo da UNA ANTENNA Alivello 3
byte IPv4_Address_Antenna[4] = {0,0,0,0};                                 // 0.0.0.0 indirizzo ip della antenna che ha visto un incidente
byte IPv4_OSPF_Payload[7];                                                // Payload che invio quando faccio OSPF
bool To_Ack = false;                                                      // Se settato a true, allora devo confermare il paccjetto ricevuto.
bool Accident_Comunication = false;                                       // Comunicazione che ricevo da altre antenne se ci è stato un incidente
byte IPv4_Address_BROADCAST[4] = {255,255,255,255};                       // Indirizzo Broadcast dove invio il messaggio quano ho un incidente
bool For_Me = false;
bool Correct = false;
int counter =1;

// tabella che userò per effettuare del routing statico
// ciascuna antenna coinoscera tutta la topografia della rete
struct routing_table
{
  byte IP_Address [4][4]={ {192,168,1,1} ,  {192,168,1,2} ,{192,168,1,3} ,{192,168,1,254}};  // (antenna 2 || ospedale || unità centrale )   // IP ANTENNA 1????
};

routing_table r_tab;



// FUNZIONI
void Send_Packet(void);
void IPv4_Packet_Create(byte,byte*,byte*,byte*);                                          // ( Time to Live; PAyload ; Source Address ;  Destination Address ) 
void IPv4_Packet_Read(byte*);
void Print_Details_Received( byte* );                                                     // Funzioni che mostrano i pacchetti ipv4 ricevuti                                
void Print_Details_Send( byte* );
void LED_Alarm(void);
void Get_Routing_Table(void);                                                             // Mi crrao il payload da inviare nel messaggio OSPF
void Update_Routing_Table (byte*);






void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  printf_begin();
  Serial.print(F("\n\rRF24/examples/pingpair_ack/\n\rROLE: "));
  Serial.println("Ospedale");                                                             // stampo il tipo di ruolo che attualmente ha l'antenna
  
  // Setup and configure rf radio
  radio.begin();
  radio.setRetries(3,15);                                                                 // (minor tempo tra le ritrasmissioni,massimo numero di tentativi)
  radio.setPayloadSize(27);                                                               // Si sta testando la comunicazione per pacchetti con un solo byte di payload   
  pinMode(8,OUTPUT);                                                                      // Led sul pin 8 che accendo quando ho un incidente
}







void loop() {
  radio.openWritingPipe(pipes[1]);
  radio.openReadingPipe(1,pipes[0]);
  radio.startListening();
  byte pipeNo;
  byte gotPacket[27];
  
  
  
  for (OSPF_Counter=60000;OSPF_Counter>0;OSPF_Counter--)                                   // Posso pensare di restare in ascolto per il tempo necessario a ricevere SICURO un pacchetto dall'antenna                                
  {  
    Accident_Comunication= false;
    counter=1;
    radio.openWritingPipe(pipes[1]);
    radio.openReadingPipe(1,pipes[0]);
    radio.startListening();
    if(radio.available(&pipeNo))
    {
    radio.read( &gotPacket, 27 );
    IPv4_Packet_Read(gotPacket);
    
    while (( Accident_Comunication == true )  && ( counter>0))                               // invio dell'incidente all'ospedale
    {
      Print_Details_Received ( gotPacket );
      counter--;
      LED_Alarm();    
    }

   
    
    }
      MAC_ADDRESS_ACCIDENT[0] = 0x00;
      MAC_ADDRESS_ACCIDENT[1] = 0x00;
      MAC_ADDRESS_ACCIDENT[2] = 0x00;
      MAC_ADDRESS_ACCIDENT[3] = 0x00;
      MAC_ADDRESS_ACCIDENT[4] = 0x00;
      MAC_ADDRESS_ACCIDENT[5] = 0x00;
    delay(1);                                              
} 
    Send_Packet();                                                                          // Chiamo questa funzione solo quansdo faccio OSPF
                                                                                       
}
