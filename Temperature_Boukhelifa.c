/***************************************************************************
***************Acquisition de données capteur température DS18S20 **********
*******************BOUKHELIFA Ali----L3 MIME----2020-2021*******************
****************************************************************************/

unsigned int temp;
int nombreaffiche;
int val;
char tempStr[7];    //Tableau de caractère pr convertir la température en chaine de caractères

//Prototype de la fonction 
void afficheChiffre(char chiffre,char position);
void afficheNombre(int var);


void main(void) {
        TRISA = 0x00;
        TRISD = 0x00;
        TRISE.B2 = 1;
        ANSELA = 0;                		// Configure PORTA pins as digital
        ANSELB = 0x00;                 // Configure PORTB pins as digital
        ANSELC = 0;
        ANSELE = 0;

        I2C1_Init(100000);
        UART1_Init(9600);                // Initialize UART module at 9600 bps
        UART1_Write(13);
        UART1_Write(10);
                     
        do {
   				//récupération de la temperature
                Ow_Reset(&PORTE, 2);
                Ow_Write(&PORTE, 2, 0xCC);              // Issue command SKIP_ROM
                Ow_Write(&PORTE, 2, 0x44);              // Issue command CONVERT_T
                Ow_Reset(&PORTE, 2);
                Ow_Write(&PORTE, 2, 0xCC);
                Ow_Write(&PORTE, 2, 0xBE);

                temp =  Ow_Read(&PORTE, 2)/1.8;
                temp = ((Ow_Read(&PORTE, 2) << 8) + temp)/1.8;
         

        /* A décommenter pour utiliser l'I2C !!!

                I2C1_Start();              // issue I2C start signal
                I2C1_Wr(0xA2);             // send byte via I2C  (device address + W)
                I2C1_Wr(2);                // send byte (address of EEPROM location)
                I2C1_Wr(temp);             // send data (data to be written)
                I2C1_Stop();               // issue I2C stop signal
                Delay_100ms();
                //Lecture
                I2C1_Start();              // issue I2C start signal
                I2C1_Wr(0xA2);             // send byte via I2C  (device address + W)
                I2C1_Wr(2);                // send byte (data address)
                I2C1_Repeated_Start();     // issue I2C signal repeated start
                I2C1_Wr(0xA3);             // send byte (device address + R)
                temp = I2C1_Rd(0);        // Read the data (NO acknowledge)
                I2C1_Stop();               // issue I2C stop signal*

                */

                afficheNombre(temp);
                //UART1_Write(temp);

                IntToStr(temp, tempStr);    //Converstion de l'entier en chaine de caractères 
            
              	UART1_Write_Text(tempStr);         // send data via UART
              	Delay_ms(1000);
            
        } while (1);
}

//Fonction affichage chiffre sur l'afficheur 7 ségments
void afficheChiffre(char chiffre,char position){
        LATA = 0x01 << position; 
        switch(chiffre){
                case 0:LATD0_bit = 1;LATD1_bit = 1; LATD2_bit = 1;LATD3_bit = 1;
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

//Fonction pour l'affichage d'un nombre
void afficheNombre(int var){
        char chiffre0,chiffre1, chiffre2,chiffre3;
        chiffre3 = var/1000;
        var -= chiffre3*1000;
        chiffre2 = var/100;
        var -= chiffre2*100;
        chiffre1 = var/10;
        var -= chiffre1*10;   //Chiffre apres la virgule
        chiffre0 = (char)var;

        afficheChiffre(chiffre0,0);// met un chiffrex sur la 1ere 7 segment
        Delay_ms(1); // Tempo
        afficheChiffre(chiffre1,1);  // met un chiffre sur la 2eme 7 segment
        Delay_ms(1); 
        afficheChiffre(chiffre2,2);  // met un chiffre sur la 3eme 7 segment
        Delay_ms(1); 
        afficheChiffre(0,3); // met 0 a la derniere position sur 7 segment car le DS18S20 ne capte pas les temp?ratures de plus de 257?C (la nous somme ? mille sur le dernier 7 segment)
        Delay_ms(1); 
}