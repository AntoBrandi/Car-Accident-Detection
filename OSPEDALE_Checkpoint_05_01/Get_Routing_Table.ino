/*
 * 
 * FUNZIONE CHE CREA UN PACCHETTO DI TIPO OSPF BASANDOLO SULLA ROUTING TABLE ATTUALE
 * 
 * Creo il payload del pacchetto IPv4 che sarà trasmesso con il messaggio OSPF 
 * tale payload conterrà gli indirizzi IP dei nodi direttamente collegati con me, quelli che conosco nella mia tabella di routing statico
 * 
 */





void Get_Routing_Table ( void) 
{
  IPv4_OSPF_Payload[0] = 0xF0;
  IPv4_OSPF_Payload[1] = r_tab.IP_Address[0][3];
  IPv4_OSPF_Payload[2] = r_tab.IP_Address[1][3];
  IPv4_OSPF_Payload[3] = r_tab.IP_Address[2][3];
  IPv4_OSPF_Payload[4] = r_tab.IP_Address[3][3];
  IPv4_OSPF_Payload[5] = 0x00;
  IPv4_OSPF_Payload[6] = 0x00;
}

