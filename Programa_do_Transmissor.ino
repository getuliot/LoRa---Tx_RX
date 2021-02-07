#include <LoRa.h>
#include <SPI.h>
#include <Wire.h>
 
/* Definicoes para comunicação com radio LoRa */
#define SCK_LORA           5
#define MISO_LORA          19
#define MOSI_LORA          27
#define RESET_PIN_LORA     14
#define SS_PIN_LORA        18
 
#define HIGH_GAIN_LORA     20  /* dBm */
#define BAND               915E6  /* 915MHz de frequencia */
/* Definicoes gerais */
#define DEBUG_SERIAL_BAUDRATE    115200
int Resultado;
int SensorBaixo = 34;
int SensorMedio = 35;
//int SensorAlto  = 32;
int SensorAlto  = 25;
/* Variaveis globais */
long informacao_a_ser_enviada = 0;
 
/* Local prototypes */
bool init_comunicacao_lora(void);
 
/* Funcao: inicia comunicação com chip LoRa
 * Parametros: nenhum
 * Retorno: true: comunicacao ok
 *          false: falha na comunicacao
*/
bool init_comunicacao_lora(void)
{
    bool status_init = false;
    Serial.println("[LoRa Sender] Tentando iniciar comunicacao com o radio LoRa...");
    SPI.begin(SCK_LORA, MISO_LORA, MOSI_LORA, SS_PIN_LORA);
    LoRa.setPins(SS_PIN_LORA, RESET_PIN_LORA, LORA_DEFAULT_DIO0_PIN);
     
    if (!LoRa.begin(BAND)) 
    {
        Serial.println("[LoRa Sender] Comunicacao com o radio LoRa falhou. Nova tentativa em 1 segundo...");        
        delay(1000);
        status_init = false;
    }
    else
    {
        /* Configura o ganho do receptor LoRa para 20dBm, o maior ganho possível (visando maior alcance possível) */
        LoRa.setTxPower(HIGH_GAIN_LORA); 
        Serial.println("[LoRa Sender] Comunicacao com o radio LoRa ok");
        status_init = true;
    }
 
    return status_init;
}
 
/* Funcao de setup */
void setup() 
{

  pinMode(SensorBaixo, INPUT);
  pinMode(SensorMedio, INPUT);
  pinMode(SensorAlto, INPUT);    
    Serial.begin(DEBUG_SERIAL_BAUDRATE);
    while (!Serial);
 
    /* Tenta, até obter sucesso, comunicacao com o chip LoRa */
    while(init_comunicacao_lora() == false);       
}

 
/* Programa principal */
void loop() 
{
int SensorBaixostates = digitalRead(SensorBaixo);
int SensorMediostates = digitalRead(SensorMedio);
int SensorAltostates = digitalRead(SensorAlto);
          if(SensorAltostates==0&&SensorMediostates==0&&SensorBaixostates==0){
             Serial.println("Nivel alto");
                               delay(250);
                               Resultado = 32;                             
                       }else
                       
                       if(SensorAltostates==1&&SensorMediostates==0&&SensorBaixostates==0){
                              Serial.println("Nivel Medio");
                               delay(250);
                               Resultado = 35;
                               
                              }else
                              
                              if(SensorAltostates==1&&SensorMediostates==1&&SensorBaixostates==0){
                                      Serial.println("Nivel Baixo");
                                      delay(250);
                                     Resultado = 34;     
                                                        
                                                        }else
                                                        
                                if(SensorAltostates==1&&SensorMediostates==1&&SensorBaixostates==1){                        
                                                        Serial.println("Vazio");
                                                        delay(250);
                                                        Resultado = 0;
                                                        }

                                                                             
                          
                              

    LoRa.beginPacket();
    LoRa.write((unsigned char*)&Resultado,sizeof(Resultado));
    LoRa.endPacket();
    delay(1000);
                       
                       
     }  //fecha o loop
    
