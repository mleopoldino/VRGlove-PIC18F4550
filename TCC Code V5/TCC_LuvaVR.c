/*
 * File:   TCC_Luva V4.c
 * Author: MLS
 * Version: 4.0
 * Created on 27 de Abril de 2016, 02:17
 */
 
#include<18F4550.h>        
#device ADC = 10
#fuses hs,nowdt,noprotect,noput,nodebug,usbdiv,pll2,vregen,mclr
#use delay(clock=8000000)
//#fuses HSPLL,NOWDT,NOPROTECT,NOLVP,NODEBUG,USBDIV,PLL2,CPUDIV1,VREGEN,MCLR,PUT
//#use delay(clock=48000000)

#use rs232(baud=9600,xmit=pin_c6,rcv=PIN_C7,bits=8,parity=N,stream=PC)
#include "LCD8B.c"
#use standard_io(c)

void ini_app(void);

void main() {

  //Configura o bit RB0 como entrada 
  set_tris_b(0b00000001); 
  //Configura o bit RA0, RA1, RA2 como entrada
  set_tris_a(0b00000111);
  
  //Declara��o de Variaveis
  
  int16 x=0;     //Variavel x para determinar o sinal digital X
  int16 y=0;     //Variavel u para determinar o sinal digital Y
  int16 z=0;     //Variavel z para determinar o sinal digital Z
  int16 bnt=0;   //Variavel bnt para determinar que o bot�o foi ativado (0=desativado 1=ativado) 
  
  int i=0;        //Contador
  int16 unX[5];  //Vetor para leitura X
  int16 unY[5];  //Vetor para leitura Y
  int16 unZ[5];  //Vetor para leitira Z
  
  //Configura��o do ADCON
  
  setup_adc_ports(AN0_TO_AN3);      //Inicializa ADC nas portas dos registrador an0(RA0), an1(RA1), an2(RA2) e an3(RA3)
  setup_adc(adc_clock_internal);    //Determina o uso do clock interno para a convers�o
  
  lcd_ini();      //Inicializa o LCD
  delay_ms(200);  //Gera um atraso de 200 ms
  
  ini_app();      //Mensagem inicial do LCD

  while(true){
                  
      delay_us(50);
                 
      for(i=0;i<5;i++){
      
         set_adc_channel(0);  //Configura o canal 0
         delay_us(50);        //Gera atraso de 30 us
         unX[i]=read_adc();   //Leitura ADC X no vetor para a posi��o i
         
         set_adc_channel(1);  //Configura o canal 1
         delay_us(50);        //Gera atraso de 30 us
         unY[i]=read_adc();   //Leitura ADC Y no vetor para a posi��o i
         
         set_adc_channel(2);  //Configura o canal 2
         delay_us(50);        //Gera atraso de 30 us
         unZ[i]=read_adc();   //Leitura ADC Z no vetor para a posi��o i
         
         delay_us(100);
      
      }
      
      for (i=0;i<5;i++){
         //Soma dos valores no vetor
         x = x + unX[i];
         y = y + unY[i];
         z = z + unZ[i];
         
         if(i==4){
            //Calculo da m�dia
            x = x/5;
            y = y/5;
            z = z/5;
            //Atualiza o LCD
            printf(lcd_escreve,"   TCC Luva RV");//   TCC Luva RV
            printf(lcd_escreve,"\n X%3Ld Y%3Ld Z%3Ld",x,y,z);  //Envia os caracteres para o LCD
            fprintf(PC,"X%3Ld Y%3Ld Z%3Ld BntOff\r\n",x,y,z);  //Envia caracteres para a Serial
            delay_ms(100);
         }
      }
            
      if(input(pin_b0)==1){
        do{
              //Atualiza o LCD
              printf(lcd_escreve,"   TCC Luva RV");//   TCC Luva RV
              printf(lcd_escreve,"\n    Botao On    ");          //Envia mensagem para o LCD
              fprintf(PC,"X%3Ld Y%3Ld Z%3Ld BntOn\r\n",x,y,z);   //Envia caracteres para a Serial
              
              //Variavel Flag bot�o pressionado
              bnt=1;
        } while(input(pin_b0)==1);
      
      }
      
      //Retorna os valore x, y, z e bnt para 0   
      x=y=z=bnt=0;
      
  }
}

void ini_app(void){
  //Apresenta��o
  printf(lcd_escreve,"\f   TCC Luva RV");//   TCC Luva RV
  printf(lcd_escreve,"\n   PIC 18F4550");
  delay_ms(2000);
  
  //Imprime pos X, Y, Z inicial em 00
  delay_us(1000);
  printf(lcd_escreve,"\f"); //Limpa Display
  printf(lcd_escreve,"\f   TCC Luva RV");//   TCC Luva RV
  printf(lcd_escreve,"\n X=00 Y=00 Z=00 ");
  delay_ms(2000);
  }

