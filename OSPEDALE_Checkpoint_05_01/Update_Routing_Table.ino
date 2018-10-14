/*
 * 
 * FUNZIONE CHE SI OCCUPA DI CONFRONTARE IL PAYLOAD DEL PACCHETTO OSPF CHE HO APPENA RICEVUTO CON LA TABELLA DI ROUTING STATICO
 * 
 * Byte per byte confronto se l'indirizzo presente nel payload sia già presente in uno qualsiasi dei campi della tabella
 * se è così stampo a schermo una stringa di conferma
 * in alternativa, se l'indirizzo non è presente stampo una stringa negativa
 * possibilità di ampliarlo usando una scrittura in tabella di nuovi elementi precedentemente non presenti
 * 
 */



void Update_Routing_Table(byte* gotPacket)
{
  if ((gotPacket[21]==r_tab.IP_Address[0][3]) || (gotPacket[21]==r_tab.IP_Address[1][3]) || (gotPacket[21]==r_tab.IP_Address[2][3]) || (gotPacket[21]==r_tab.IP_Address[3][3]) )
  {
    Serial.println("Indirizzo 1 Presente");
  }
  else
  {
    Serial.println("Indirizzo 1 NON Presente");
  }
  if ((gotPacket[22]==r_tab.IP_Address[0][3]) || (gotPacket[22]==r_tab.IP_Address[1][3]) || (gotPacket[22]==r_tab.IP_Address[2][3]) || (gotPacket[22]==r_tab.IP_Address[3][3]) ) 
  {
    Serial.println("Indirizzo 2 Presente");
  }
  else
  {
    Serial.println("Indirizzo 2 NON Presente");
  }
  if ((gotPacket[23]==r_tab.IP_Address[0][3]) || (gotPacket[23]==r_tab.IP_Address[1][3]) || (gotPacket[23]==r_tab.IP_Address[2][3]) || (gotPacket[23]==r_tab.IP_Address[3][3]) )
  {
    Serial.println("Indirizzo 3 Presente");
  }
  else
  {
    Serial.println("Indirizzo 3 NON Presente");
  }
  if ((gotPacket[24]==r_tab.IP_Address[0][3]) || (gotPacket[24]==r_tab.IP_Address[1][3]) || (gotPacket[24]==r_tab.IP_Address[2][3]) || (gotPacket[24]==r_tab.IP_Address[3][3]) )
  {
    Serial.println("Indirizzo 4 Presente");
  }
  else
  {
    Serial.println("Indirizzo 4 NON Presente");
  }
}

