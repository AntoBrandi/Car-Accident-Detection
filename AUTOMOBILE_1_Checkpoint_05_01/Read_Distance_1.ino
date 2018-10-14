/*
 * 
 * FUNZIONE CHE SI OCCUPA DI LEGGERE LA DISTANZA DAL SENSORE AD ULTRASUONI FRONTALE 
 *  
 * Mi accerto che siano trascorsi almeno 30 ms dall'ultimo utilizzo degli ultrasuoni per garantire il corretto funzionamento 
 * 
 * Memorizzo la distanza misurata nella variabile ValueDist1 
 * 
 * Restituisco questa lettura al loop che si occuperà di valurate se sono in pericolo o meno
 * 
 */



int Read_Distance_1(void)                                  
{
 delay(30);                                                // Aspetto 30ms prima di inviare un nuovo ping (aspettare almeno 29ms)
 unsigned int uS = sonar1.ping();                          // Send ping, get ping time in microseconds (uS).
 ValueDist1 = uS / US_ROUNDTRIP_CM;                        // In ValueDist è contenuta la distanza in cm
 
 //INUTILI NELLA PROVA SU STRADA
 Serial.print("Distanza1: ");
Serial.print(ValueDist1);
 Serial.println(" cm");
 return ValueDist1;
}
