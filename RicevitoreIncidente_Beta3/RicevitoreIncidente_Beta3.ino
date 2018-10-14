#include <SPI.h>
#include <nRF24L01.h>
#include <printf.h>
#include <RF24.h>
#include <RF24_config.h>

RF24 radio(9,10); //CE, CSN

// Topology
const uint64_t pipes[2] = { 0xABCDABCD71LL, 0x544d52687CLL };              // Radio indirizzi per la comuniazione tra due antenne
int RTT = 1500;
int counter = 2;

//Gestione dei ruoli tra antenne, una trasmette e l'altra riceve, uso lo stesso codice per entrame le antenne. 
//ciascuna antenna potrà quindi sia trasmettere che ricevere
   
void Send_Packet(void);
void CSMA_CA(void);




void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  printf_begin();
  Serial.print(F("\n\rRF24/examples/pingpair_ack/\n\rROLE: "));
  Serial.println("Antenna bordo Strada");                      //stampo il tipo di ruolo che attualmente ha l'antenna posso scegliere tra invalid, ping out, ping back
  
  // Setup and configure rf radio
  radio.begin();
  radio.setAutoAck(1);                    // Assicura che sia attivo l'AutoAck
  radio.enableAckPayload();               // Attivo l'Ack per il payload
  radio.setRetries(0,15);                 // (minor tempo tra le ritrasmissioni,massimo numero di tentativi)
  radio.setPayloadSize(1);                // Si sta testando la comunicazione per pacchetti con un solo byte di payload   
}




void loop() {
  // put your main code here, to run repeatedly:
  radio.openWritingPipe(pipes[1]);
  radio.openReadingPipe(1,pipes[0]);
  radio.startListening();
  byte pipeNo;
  byte gotByte;
  while (RTT>=0)    // Se esiste una radio pronta a trasmetterti resta in ascolto
  {  // Posso pensare di restare in ascolto per il tempo necessario a ricevere SICURO un pacchetto dall'antenna
    if(radio.available(&pipeNo))
    {
    radio.read( &gotByte, 1 );
    radio.writeAckPayload(pipeNo,&gotByte, 1 );           // Genera un pacchetto di ack con lo stesso pauìyload che hai ricevuto
    Serial.print("GotByte: ");
    Serial.println(gotByte);
    }
    RTT--;
    delay(1);
                                 // Resetto il valore del round trip time  
} 
    Send_Packet();
    RTT = 1500; 
}


void Send_Packet (void)
{
  radio.openWritingPipe(pipes[0]);
  radio.openReadingPipe(1,pipes[1]);
  radio.stopListening();                                  // First, stop listening so we can talk.     
  printf("Now sending %d as payload. ",counter);          // sto trasmettendo un indice da 1 byte crescente ad ogni iterazione
  byte gotByte;  
  unsigned long time = micros();                          // Take the time, and send it.  This will block until complete                                                            //Called when STANDBY-I mode is engaged (User is finished sending)
  if (!radio.write( &counter, 1 )){                       // se la trasmissione del contatore non è andata a buon fine allora stmapa la stringa failed
      Serial.println(F("failed."));
      CSMA_CA();      
    }else{

      if(!radio.available()){  
        CSMA_CA();                                          //Se nessuna connessione radio è disponibile dimmi che hai ricevuto un pacchetto bianco
        Serial.println(F("Blank Payload Received.")); 
      }else if (radio.available() )                                              //altrimenti se qualcuno ti sta rispondendo memorizzalo in got byte e stampalo a scermo
        {
          unsigned long tim = micros();
          radio.read( &gotByte, 1 );
          printf("Got response %d, round-trip delay: %lu microseconds\n\r",gotByte,tim-time);
        }
      }
  }


void CSMA_CA (void)
{
  int backoff;                                               // Variabile nella quale sarà settato il numero casuale da attendere prima di provare a ritrasmettere
  randomSeed(millis());                                      // Uso come seme per la generazione di numeri casuali il tempo
  backoff = random( 0 , 50 );                                // Setto un numero casuale di attesa 0-30 millisecondi
  delay(backoff);
  Send_Packet();
}
