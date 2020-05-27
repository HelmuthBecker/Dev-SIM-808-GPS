 /*
 
                                                          SoftSerial (Saídas Digitais)      Objeto EBYTE (Saídas Digitais)    LEDs Indicativos (Saídas Analógicas)
                                                                                            ** 7, 8, 9 - M0, M1, AUX          ** A1 - LED Mensagem Recebida
                                                          ** 5,6 - LORA Rx, Tx                                                ** A2 - LED GPS
  
 */
 
//#include <SPI.h>
#include <SoftwareSerial.h>       //Biblioteca SoftwareSerial
#include <EBYTE.h>                //Biblioteca dos módulos LORA

SoftwareSerial serialLORA(5,6);   //Rx-TX

#define DEBUG true

                    // ******* VARIÁVEIS GLOBAIS ******
   
String response;                                      //Contém a mensagem enviada pelo Sender do barco
int    tamanhoString;                                 //Informa a qtd de char da variável response; Caso seja 51 char a mensagem esta completa.

                  // ******* FIM VARIÁVEIS GLOBAIS ******

EBYTE emissor(&serialLORA, 7, 8, 9);                  //Objeto EBYTE - parâmetros PINOS Rx,Tx, M0, M1, AUX

 void setup()
	{
      Serial.begin     (9600);
      serialLORA.begin (9600);
  
 	    iniciarLORA();
	}   //Fim Setup

 void loop()
	{
     serialLORA.listen();
        
     receberMensagem(5000,DEBUG);                        //Abre uma janela por 5 segundos e aguarda o recebimento da mensagem
     //receberMensagem();
     validarMensagem();                                 //Faz o tratamento da mensagem recebida pelo módulo com todos dados fornecidos
     
  } //Fim loop

 void iniciarLORA () 
	{
     serialLORA.listen();                   
       
     emissor.init();                                            //Inicia o módulo

     emissor.SetMode(MODE_NORMAL);                              //Modo de funcionamento do módulo
     emissor.SetAddressH(0);                                    //Endereço H(?)
     emissor.SetAddressL(0);                                    //Endereço L(?)
     emissor.SetAirDataRate(ADR_2400);                          //AirDataRate 2400kbps
     emissor.SetUARTBaudRate(UDR_9600);                         //BAUDRate 9600
     emissor.SetChannel(23);                                    //Canal 23
     emissor.SetParityBit(PB_8N1);                              //Bit Paridade 8N1
     emissor.SetTransmitPower(OPT_TP30);                        //Força de transmissão 30db
     emissor.SetWORTIming(OPT_WAKEUP2000);                      //WakeUP Time(?) 2000
     emissor.SetFECMode(OPT_FECENABLE);                         //FEC(?)
     emissor.SetTransmissionMode(OPT_FMDISABLE);                //Transmission Mode
     emissor.SetPullupMode(OPT_IOPUSHPULL);                     //IO Mode PushPull
     emissor.SaveParameters(PERMANENT);                         //Salva as modificações na memória do módulo
         
     emissor.PrintParameters();                                 //Exibe os parâmetros configurados
  } //Fim iniciarLORA 

    
 void validarMensagem ()                            //Função que faz o tratamento da mensagem com os dados recebidos
	{                                    
     tamanhoString = response.length();             //Informa a qtd de char da variável response;
     Serial.println();
     if (tamanhoString == 51 && response[0] == '[' && response[48] == ']' )      //Verifica se a string está inteira e consistente. Caso tenha 49 caracteres está OK
    	{
          serialLORA.println("1");
          Serial.print("Mensagem Recebida: ");
          Serial.println(response);    	    
    	}  
    else
      {
          Serial.println("Falha no recebimento");
          Serial.println(tamanhoString);
          //Serial.println(response[0])
      }                   
       	 
  }  //Fim criarMensagem

void receberMensagem()
{
  serialLORA.listen();

  response = "";

  while(serialLORA.available())
          {       
             char c = serialLORA.read(); 
             response+=c;
          }  
  Serial.println(response+"---teste");
  }
  
 String receberMensagem(const int timeout, boolean debug)
	{
     serialLORA.listen();
    
     //Efetua a limpeza das variáveis antes de executar a função
     response = "";                                  //Informações completas recebidas pelo módulo
     tamanhoString = "";   
     long int time = millis();   
    
     while( (time+timeout) > millis())               //Enquanto a soma de time(millis()) + timeout for maior do que o valor atual de millis(), lê caracteres da Serial.
    	{
         while(serialLORA.available())
        	{       
             char c = serialLORA.read(); 
             response+=c;
          }  
      }    
     if(debug)                                       //Se debug for true, retorna o valor da variavel response
    	{
         //Serial.print(response);
         return response;
      }
  }  //Fim sendData
