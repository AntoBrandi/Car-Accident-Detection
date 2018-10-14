/*
 * 
 * FUNZIONE CHE SI OCCUPA DI LEGGERE I VALORI DI UN ACCELEROMETRO E VERIFICARE SE SONO CONTENUTI NEI LIMITI PREVISTI
 * 
 * Instauro una comunicazione tramite I2C con l'accelerometro a 3 assi
 * Memorizzo nelle variabili AcX,AcY,AcZ rispettivamente le accelerazioni misurate lungo l'asse x,y,z
 * 
 * Valuto se in una di queste direzioni l'accelerazione superi il valore massimo calibrato sul modello in scala
 * In tal caso , si presuppone che ci sia stata una condizione anomala di funzionamento ed un probabile incidente
 * 
 */







bool Read_Acceleration()                                   
{
  bool result = false;
  //INIZIO LA COMUNICAZIONE TRAMITE I2C
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU,14,true);
  // COMUNICAZIONE CONCLUSA
  
  AcX=Wire.read()<<8|Wire.read();                          // Contengono i valori in output delle letture dell'accelerometro
  AcY=Wire.read()<<8|Wire.read();
  AcZ=Wire.read()<<8|Wire.read();

  //INUTILI NELLA PROVA SU STRADA
  Serial.print(AcX);                                       // Stampa a schermo dei valori, utile solo in fase di calibrazione del sensore
  Serial.print(" ");
  Serial.print(AcY);
  Serial.print(" ");
   Serial.println(AcZ);

  AcX=abs(AcX);                                            // il confronto viene effettuato sui valori assoluti delle letture dell'accelerometro
  AcY=abs(AcY);
  AcZ=abs(AcZ);

  if ( (AcX>MAX_ACCELERATION_X) || (AcY>MAX_ACCELERATION_Y) || (AcZ>MAX_ACCELERATION_Z) )
  {
    Serial.println("                                        PERICOLO ACCELERAZIONE");
    Serial.println(AcX);
    Serial.println(AcY);
    Serial.println(AcZ);
    result = true;                                         // Valuto la condizione sulla accelerazione con dei valori di riferimento da clibrare in base alla auto
  } 
  return result;
}
