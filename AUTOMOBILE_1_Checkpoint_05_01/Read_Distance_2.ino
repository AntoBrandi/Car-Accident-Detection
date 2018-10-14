/*
 * 
 * FUNZIONE CHE SI OCCUPA DI LEGGERE LA DISTANZA DAL SENSORE AD ULTRASUONI POSTERIORE
 *  
 * Mi accerto che siano trascorsi almeno 30 ms dall'ultimo utilizzo degli ultrasuoni per garantire il corretto funzionamento 
 * 
 * Memorizzo la distanza misurata nella variabile ValueDist2 
 * 
 * Restituisco questa lettura al loop che si occuperà di valurate se sono in pericolo o meno
 * 
 */





int Read_Distance_2(void)
{
  // Aspetto 50ms prima di inviare un nuovo ping (aspettare almeno 29ms)
 unsigned int uS = sonar2.ping();                          // Send ping, get ping time in microseconds (uS).
 ValueDist2 = uS / US_ROUNDTRIP_CM;                        // In ValueDist è contenuta la distanza in cm

 //INUTILI NELLA PROVA SU STRADA
 Serial.print("Distanza2: ");
 Serial.print(ValueDist2);
 Serial.println(" cm");
 
 return ValueDist2;
}
