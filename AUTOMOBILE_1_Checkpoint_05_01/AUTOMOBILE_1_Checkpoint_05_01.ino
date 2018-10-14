/* 
 *  Sketch relativo alla AUTOMOBILE con IP = 192.168.1.11
 *  
 *  Vengono letti i dati provenienti dalla lettura di alcuni sensori quali : Accelerometro e 2 Distanziometri
 *  Tali dati sono confrontati con dei valori di riferimento che sono stati calibrati sulle auto utilizzate nel modello dimostrativo
 *  Qualora la lettura dell'accelerometro superi un certo valore di guardia & il distanziometro legga un oggetto in contatto
 *  Unitamente ad una informazione sullo scoppio di un Airbag ( soltanto simultao via codice) si giunge alla conclusione che l'auto ha avuto un incidente
 *  
 *  Tale condizione sarà prontamente comunicata dalla auto alla antenna dalla quale è coperta IPv4_Address_Antenna.
 *  
 *  Parallelamente, l'auto resta sempre in ascolto di un eventuale messaggio di hello proveniente dalla antenna
 *  Tale messaggio sarà scambiato quando l'auto si trova sotto la copertura di una antenna, l'antenna comunicherà un messaggio con payload HELLO = 0x 00 00 00 00 00 00 F0
 *  A tale payload risponderò con l'indirizzo MAC della antenna nel payload di livello 3 MAC_ADDRESS
 *  
 */

#include <SPI.h>
#include <Wire.h>
#include <nRF24L01.h>
#include <printf.h>
#include <RF24.h>
#include <RF24_config.h>
#include <NewPing.h>


#define MIN_DISTANCE        6                              // Minima distanza oltre la quale si considera avvenuto il contatto
#define MAX_ACCELERATION_X  18000                          // Massimo valore di accelerazione lungo gli assi oltre il quale si considera ANOMALA la decelerazione o accelerazione
#define MAX_ACCELERATION_Y  18000
#define MAX_ACCELERATION_Z  25000
#define TRIGGER_PIN_1       2                              // Pin di arduino al quale è collegato il trigger del sensore
#define ECHO_PIN_1          3                              // Pin di arduino al quale è collegato l'echo del sensore
#define TRIGGER_PIN_2       4                              // Pin relativi al secondo sensore di distanza
#define ECHO_PIN_2          5                                      
#define MAX_DISTANCE        400                            // Massima distanza leggibile dal sensore                


NewPing sonar1(TRIGGER_PIN_1, ECHO_PIN_1, MAX_DISTANCE);   // Creo l'oggetto sonar1 della libreria NewPing
NewPing sonar2(TRIGGER_PIN_2, ECHO_PIN_2, MAX_DISTANCE);   // Creo l'oggetto per il secondo sensore di distanza
RF24 radio(9,10);                                          // ( CE , CSN )


// IPv4 Implementation
const byte MAC_ADDRESS[6]={0xB3,0xB4,0xB5,0xB6,0xB7,0xF2};
const byte ACCIDENT_PAYLOAD = 0xFF;                                     // Primo byte derl payload ( Liv.3) che invio quando ho un incidente, gli altri 6 saranno il mio MAC 
const byte MAC_PAYLOAD = 0x00;                                          // Primo byte del payload (Liv.3) che invio quando rispondo ad un messaggio di HELLO , gli altri 6 saranno il mio MAC
const byte IPv4_Address_Car_1[4] = {192,128,1,12};                      // 192.168.1.11 indirizzo ip della auto 1
byte MAC_ADDRESS_RECEIVED[6];                                           // Indirizzo MAC della antenna che mi sta parlando
byte IPv4_Packet[27];                                                   // Su questo array andrò a scrivere di volta in volta il pacchetto IPv4 che sono in procinto di inviare
byte IPv4_Packet_Received[27];                                          // Ogni qual volta ricevo un pacchetto IPv4 lo memorizzo in questo array per poi leggerlo nella funzione IPv4_Packet_Read
byte IPv4_Address_Antenna[4] = {0,0,0,0};                               // Indirizzo IP della antenna. Ogni qual volta cambia la antenna che mi copre, aggiorno questo campo
bool To_Ack = false;                                                    // Se settato a true, allora devo confermare il pacchetto ricevuto.
bool Accident_Comunication = false;                                     // Se settato a true allora mi stanno COMUNICANDO che sulla strada c'è stato un incidente
bool Accident_Confirmed = false;                                        // Se settato a true significa che l'antenna alla quale ho comunicato il MIO incidente mi ha confermato di averlo ricevuto
bool For_Me = false;                                                    // Se settato a true significa che il pacchetto che ho ricevuto è destinato a ME o ad un IP broadcast
bool Correct = false;                                                   // Se settato a true significa che il pacchetto IPv4 appena ricevuto ha il checksum corretto

 int counter_retransmission=10;
   int counterSend=10;
   int counterSend2=10;
int SetDistance = 0;                                       // Letture provenienti dal sensore di distanza
int ValueDist1 = 0; 
int ValueDist2 = 0;
int distanza1 = 0;                                         // Distanza ricevuta dalla funzione read
int distanza2 = 0;                                         // Distanza letta per il secondo sensore ad ultrasuoni
bool Accelerometer_Alert = false;                          // Variabile ritornata dalla funzione Read dell'accelerometro                                                                                                                  
const int MPU=0x68;                                        // I2C address dell'accelerometro
int16_t AcX,AcY,AcZ;                                       // Variabili che sono lette dall'accelerometro
                                                           // Sarà verificato direttamente nella funzione se il valore delle accelerazioni superi quello di riferimento
int RTT = 1000;                                            // Contatore che indica per quanto tempo devo restare in ascolto prima di poter avere una sessione di trasmissione
bool AIRBAG = true;                                        // Se settato a true indica che l'airbag è esploso ( SEMPRE TRUE)
bool Accident=false;                                       // Se settato a true significa che ho AVUTo un incidente, NON RILEVATO


// TOPOLOGIA DELLA RETE
const uint64_t pipes[2] = { 0xABCDABCD71LL, 0x544d52687CLL };              // Radio indirizzi per la comuniazione tra due antenne
// PIPES = ( antenna , auto ) uso lo stesso indirizzo per identificare tutte le auto MULTICAST



// DICHIARAZIONE FUNZIONI                                                      
int Read_Distance_1(void);
int Read_Distance_2(void);
bool Read_Acceleration(void);                                                          
void Send_Packet(void);  
void IPv4_Packet_Create(byte,byte,byte*, byte*);                                           // ( Time to Live ; PAyload ; Source Address ; Destination Address ) 
void IPv4_Packet_Read(byte*);                                                              // ( Pacchetto_Ricevuto )
void Print_Details_Received( byte* );                                                      // ( Pacchetto_Ricevuto) Funzioni che mostrano i pacchetti ipv4 ricevuti                                
void Print_Details_Send( byte* );                                                         






void setup() {
  //SETUP della comunicazione con l'accelerometro
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
  // FINE SETUP accelerometro
                                                                     // HO AVUTO un incidente
  pinMode(8,OUTPUT);                                                 // HO RILEVATO un incidente lungo la strada 
  pinMode(6,OUTPUT);  
  digitalWrite(6,HIGH);                                              // Pin dal quale controllo il motore
  Serial.begin(115200);
  //printf_begin();
 // Serial.print(F("\n\rRF24/examples/pingpair_ack/\n\rROLE: "));
  //Serial.println("Trasmettitore"); 
  
// SETUP DELLA COMUNICAZIONE RADIO
  radio.begin();
  radio.setRetries(3,15);                                             // (minor tempo tra le ritrasmissioni,massimo numero di tentativi) Protocollo di ritrasmissione simile a CSMA/CA
  radio.setPayloadSize(27);                                           // Si sta testando la comunicazione per pacchetti con un solo byte di payload  
// FINE SETUP COMUNICAZIONE RADIO

}







void loop() {
  
  /* 
   *  Faccio in modo che l'antenna sia sempre in ascolto su un indirizzo comunemente noto tra tutte le auto
   *  Resto in ascolto dell'eventuale comunicazione di una antenna a bordo strada 
   *  che mi dica il suo indirizzo cosi so dove mandare i pacchetti se ho un incidente
   */ 
  radio.openWritingPipe(pipes[1]);
  radio.openReadingPipe(1,pipes[0]);
  radio.startListening();
  byte pipeNo;
  byte gotPacket[27]; 
  digitalWrite( 6, HIGH);                                                                              // accendo il motore
  while (RTT>=0)                                                                                       // Posso pensare di restare in ascolto per il tempo necessario a ricevere SICURO un pacchetto dall'antenna                                           
  {  
    Accident_Comunication=false;
    Accident=false;
    // Prova a rileggere anche qui i valori
    radio.openWritingPipe(pipes[1]);
    radio.openReadingPipe(1,pipes[0]);
    radio.startListening();
    if(radio.available(&pipeNo))                                                                       // Lo ricevo da un generico pipe
    {
    radio.read( &gotPacket, 27 );
    IPv4_Packet_Read(gotPacket);                                                                       // Leggo  e decodifico il pacchetto IPv4 che ho appena ricevuto
    Print_Details_Received ( gotPacket );
    
    if ( To_Ack == true ) 
    {
    IPv4_Packet_Create( 5 , MAC_PAYLOAD , IPv4_Address_Car_1 , IPv4_Address_Antenna );                 // Rispondo con un pacchetto ipv4 contenete il mio mac ( TTL , payload , source , dest)
    radio.stopListening();
    radio.openWritingPipe(pipes[1]);
    if ( !radio.write(&IPv4_Packet, 27 ))
    {
      Serial.println("failed");
    }
    else
    {
    //Print_Details_Send ( IPv4_Packet );                                                                 // Stampa a schermo del pacchetto appena ricevuto
    }
    }
    }
    
    // Lettura dati dai sensori
  distanza1 = Read_Distance_1();
  distanza2 = Read_Distance_2();
  Accelerometer_Alert = Read_Acceleration();

/*
 * SE      uno dei due distanziometri rileva una condizione di incidente distanza<Min distanza
 * E       uno dei tre accelerometri ha rilevato una accelerazione/decelerazione brusca 
 * E       è stata segnalata una esplosione dell'airbag nella autovettura
 * ALLORA  ho avuto un incidente e lo comunico
 */
  if ( (((distanza1<MIN_DISTANCE) || (distanza2<MIN_DISTANCE)) && (Accelerometer_Alert==true) && (AIRBAG==true)))
  {
    Accident = true;
  }
  else
  {
    Accident = false;
  }
  
  if ( Accident == true )
  {
    // TRASMETTI  ALL'ANTENNA PIU VICINA 
    digitalWrite(8,HIGH);                                  // Accendo il LED rosso
    digitalWrite(6,LOW);                                   // Spengo il motore
    if ( Accident_Comunication == false )                  // Trasmetto solo se sono io a fare l'incidente
    { 
    Send_Packet();                                         // Se NON ho avuto io l'incidente non trasmetto nulla 
    }
    delay(5000);
    digitalWrite(8,LOW);
    digitalWrite(6,HIGH);   
  }
 
  if (Accident_Comunication == true) 
  {
    digitalWrite(6,LOW);                                   // Spengo il motore
    digitalWrite(8,HIGH);                                  // Accendo la luce di emergenza
    delay(5000);
     digitalWrite(6,HIGH);                                   // Spengo il motore
    digitalWrite(8,LOW);                                     // Accendo la luce di emergenza
  }
 
  RTT = RTT - 1;  
       counter_retransmission=10; 
        counterSend=10;
        counterSend2=10; // Faccio in modo di aspettare effettivamente almeno 1sec in ricezione
  }
   
  RTT = 1000;                                              // Azzero i parametri prima di ricominciare il ciclo           
                          


}
