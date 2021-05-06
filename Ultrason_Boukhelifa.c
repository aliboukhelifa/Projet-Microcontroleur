/*******************************************************************************
-------------------Acquisition de données capteur Ultrason SR04-----------------
----------------------BOUKHELIFA Ali----L3 MIME----2020-2021--------------------
********************************************************************************
Fonctionnement : 
Un etat haut de 10 microsecondes est mis sur la broche "Trig"
On remet l'etat bas la broche Trig
On lit la duree de l'etat haut sur la broche "Echo"
On divise cette duree par deux pour n'avoir qu'un trajet
On calcule la distance avec la formule D = V*T/2
D=340*T/2 ; D = 170*T = T/(1/170) = T/58.82
On affiche la distance
*******************************************************************************/

//Prototype de la fonction affichage chiffre sur l'afficheur 7 ségments
void afficheChiffre(char chiffre,int position);		
//Prototype de la fonction affichage d'un nombre
void afficheNombre(int var);

void main(void) {

	int etape = 0;
	char distanceStr[3];
	int compte = 0;
	float nombreaffiche = 0;
	// Configurer routes les entrées en numeriques 
	ANSELA = 0x00;
	ANSELB = 0x00; 
	ANSELC = 0;
	ANSELD = 0x00;
	TRISA = 0x00;
	TRISB = 0x01;
	TRISD = 0x00;
	LATA = 0x00;
	LATB = 0x00;         //TRIG sur rb1
	LATD = 0x00;
	PORTB = 0x00;

	// Initialize UART module at 9600 bps
	UART1_Init(9600);            
	UART1_Write(13);
	UART1_Write(10);
	//Delay_ms(100);

    while(1)
    {
        switch(etape) 
        {
            case 0:
                    //Génération de l'impulsion 
                     LATB1_bit = 1;
                     compte = 0;
                     etape++ ;
                     break;
            case 1:
                 Delay_us(1);
                 LATB1_bit = 0;
                 etape++ ;
                 break;
            case 2:
            //Test s'il y a un echo
            if(PORTB & 0x01)      
            {
                while(PORTB & 0x01)
                {
                        compte++;
                        delay_us(10);
                }
                etape++;
            }
            break;
            case 3:
            	//D = V*T/2  ; D=340*T/2 ; D = 170*T = T/(1/170) = T/58.82
                nombreaffiche = (float)compte*10/58.82;                
                Delay_ms(100);
                //Affichage de la distance
                afficheNombre((int)nombreaffiche);
                //Envoi des données via l'UART
                //UART1_Write(nombreaffiche);
                IntToStr(nombreaffiche, distanceStr); 	//Conversion de la distance en chaine de caractères
                UART1_Write_Text(distanceStr);
                Delay_ms(100);

                etape=0;
                break;
    	}
   }
}

void afficheChiffre(char chiffre,int position){
    LATA = 0x01 << position;
    switch(chiffre)
    {
           case 0:LATD0_bit = 1;LATD1_bit = 1;LATD2_bit = 1;LATD3_bit = 1;
                  LATD4_bit = 1;LATD5_bit = 1;LATD6_bit = 0;LATD7_bit = 0;
                           break;

           case 1:LATD0_bit = 0;LATD1_bit = 1;LATD2_bit = 1;LATD3_bit = 0;
                  LATD4_bit = 0;LATD5_bit = 0;LATD6_bit = 0;LATD7_bit = 0;
                           break;

           case 2:LATD0_bit = 1;LATD1_bit = 1;LATD2_bit = 0;LATD3_bit = 1;
                  LATD4_bit = 1;LATD5_bit = 0;LATD6_bit = 1;LATD7_bit = 0;
                          break;

           case 3:LATD0_bit = 1;LATD1_bit = 1;LATD2_bit = 1;LATD3_bit = 1;
                  LATD4_bit = 0;LATD5_bit = 0;LATD6_bit = 1;LATD7_bit = 0;
                           break;

           case 4:LATD0_bit = 0;LATD1_bit = 1;LATD2_bit = 1;LATD3_bit = 0;
                  LATD4_bit = 0;LATD5_bit = 1;LATD6_bit = 1;LATD7_bit = 0;
                           break;

           case 5:LATD0_bit = 1;LATD1_bit = 0;LATD2_bit = 1;LATD3_bit = 1;
                  LATD4_bit = 0;LATD5_bit = 1;LATD6_bit = 1;LATD7_bit = 0;
                           break;

           case 6:LATD0_bit = 1;LATD1_bit = 0;LATD2_bit = 1;LATD3_bit = 1;
                  LATD4_bit = 1;LATD5_bit = 1;LATD6_bit = 1;LATD7_bit = 0;
                           break;

           case 7:LATD0_bit = 1;LATD1_bit = 1;LATD2_bit = 1;LATD3_bit = 0;
                  LATD4_bit = 0;LATD5_bit = 0;LATD6_bit = 0;LATD7_bit = 0;
                           break;

           case 8:LATD0_bit = 1;LATD1_bit = 1;LATD2_bit = 1;LATD3_bit = 1;
                  LATD4_bit = 1;LATD5_bit = 1;LATD6_bit = 1;LATD7_bit = 0;
                           break;

           case 9:LATD0_bit = 1;LATD1_bit = 1;LATD2_bit = 1;LATD3_bit = 1;
                  LATD4_bit = 0;LATD5_bit = 1;LATD6_bit = 1;LATD7_bit = 0;
                           break; 

           default:
                           break;
    }
}

void afficheNombre(int var){

        char chiffre0,chiffre1,chiffre2,chiffre3;
        chiffre3 = var/1000;
        var -= chiffre3*1000;
        chiffre2 = var/100;
        var -= chiffre2*100;
        chiffre1 = var/10;
        var -= chiffre1*10;
        chiffre0 = var;
        afficheChiffre(chiffre0,0);
        Delay_ms(1);
        afficheChiffre(chiffre1,1);
        Delay_ms(1);
        afficheChiffre(chiffre2,2);
        Delay_ms(1);
        afficheChiffre(chiffre3,3);
        Delay_ms(1);
}
