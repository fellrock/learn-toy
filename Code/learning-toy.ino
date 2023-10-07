/*
      Teste de função em C para PFC
      
      Conversor AD para célula de carga c/ interrupçao


      Felipe Moreira
      19 de Novembro de 2017
      
      Lendo valores em duas balança e calibrando ambas com a tecla C.
      Sem uso de funções auxiliares.
*/

// Biblioteca usada para o uso da contagem de tempo
#include "TimerOne.h"

// --- Mapeamento de Hardware ---
#define  ADDO1  3    //Data Out
#define  ADSK1  2    //SCK
#define  ADDO2  5    //Data Out
#define  ADSK2  6    //SCK
#define  ADDO3  8    //Data Out
#define  ADSK3  9    //SCK


// --- Protótipo das Funções Auxiliares ---
unsigned long ReadCount1(); //conversão AD do HX711
unsigned long ReadCount2(); //conversão AD do HX711
//unsigned long ReadCount3(); //conversão AD do HX711

// --- Variáveis Globais ---
unsigned long convert1=0, zera1=0, calib1=0, atual1=0, check1=0;
unsigned long convert2=0, zera2=0, calib2=0, atual2=0, check2=0;
//unsigned long convert3=0, zera3=0, calib3=0, atual3=0, check3=0;
unsigned int refer2=1, refer1=1, refresh1=0, refresh2=0;
unsigned int refer3=1, refresh3=1;
unsigned int estado=0;
unsigned int a=1,b=1,p1=0,p2=0,p3=0;
const int ledPin =  LED_BUILTIN;// o número do pino do LED
int ledState = LOW;             // ledState é usado para alterar o estado do LED para OFF

char rx_byte = 0;

// Configurações dos recursos que serão usados.
void setup()
{
  Serial.begin(9600);      // Abertura da porta serial com 9600 de baund rate:
  // set the digital pin as output:
  pinMode(ADDO1, INPUT_PULLUP);   //entrada para receber os dados do 1º sensor
  pinMode(ADSK1, OUTPUT);         //saída para SCK do 1º sensor
  pinMode(ADDO2, INPUT_PULLUP);   //entrada para receber os dados do 2º sensor
  pinMode(ADSK2, OUTPUT);         //saída para SCK do 2º sensor
  pinMode(ADDO3, INPUT_PULLUP);   //entrada para receber os dados do 3º sensor
  pinMode(ADSK3, OUTPUT);         //saída para SCK do 3º sensor
  pinMode(ledPin, OUTPUT); // saída para o pino do LED.
  Timer1.initialize(1000000);         // Inicializa o timer com 1 segundo
  Timer1.attachInterrupt(callback);  // aciona a função callback() quando o timer ultrapassar a interrupção
}
 // Início da função de interrupção
void callback()
{

  if (Serial.available() > 0) {    // is a character available?
    rx_byte = Serial.read();       // get the character 
    // check if a letter T was received
    if (rx_byte == 'a'){
      Serial.println("Tarando balança 1...");
      for (int i=check1; i > 395; i++){ 
        refresh1 = refresh1 + 1;
      }
      for (int b=check1; b < 415; b++){
        refresh1 = refresh1 - 1;
      }     
      refer1 = refresh1;
      check1 = atual1/refer1;
      Serial.println(check1); 
     }
      if (rx_byte == 's'){
      Serial.println("Tarando balança 2...");
      for (int i=check2; i > 395; i++){ 
        refresh2 = refresh2 + 1;
      }
      for (int b=check2; b < 415; b++){
        refresh2 = refresh2 - 1;
      }     
      refer2 = refresh2;
      check2 = atual2/refer2;
      Serial.println(check2); 
     }
      if (rx_byte == 'd'){
      Serial.println("Tarando balança 3...");
      for (int i=check3; i > 395; i++){ 
        refresh3 = refresh3 + 1;
      }
      for (int b=check3; b < 415; b++){
        refresh3 = refresh3 - 1;
      }     
      refer3 = refresh3;
      check3 = atual3/refer3;
      Serial.println(check3); 
      }
      if (rx_byte == 'c'){
      check1=0; check2=0;     
      zera1 = convert1;
      calib1 = zera1;
      zera2 = convert2;
      calib2 = zera2;       
      refer1=550;
      refer2=630;
      zera3 = convert3;
      calib3 = zera3;        
      refer3=600;
      }
      if (rx_byte == '1'){
      p3=0;
      } 
      if (rx_byte == '2'){
      p3=1;
      }  
      if (rx_byte == '3'){
      p3=2;
      }  
      if (rx_byte == '4'){
      p3=3;
      }  
      if (rx_byte == '5'){
      p3=4;
      }  
      if (rx_byte == '6'){
      p3=5;
      }  
      if (rx_byte == '7'){
      p3=6;
      }  
      if (rx_byte == '8'){
      p3=7;
      }        
  } // end: if (Serial.available() > 0)   
  else {   
 
  // obtenção dos valores dos sensores
  convert1 = ReadCount1();
  convert2 = ReadCount2();
  convert3 = ReadCount3();
  
  // contas para atualizar corretamente os valores
    atual1 = convert1 - calib1;
    check1 = atual1/refer1; 
    refresh1 = refer1;
    atual2 = convert2 - calib2;
    check2 = atual2/refer2; 
    refresh2 = refer2;
    atual3 = convert3 - calib3;
    check3 = atual3/refer3; 
    refresh3 = refer3;    
    
 // programação de posições de cada sensor
 
 // Posições possíveis
//
//              G     M     P
// posição 0    0     0     0    (Nenhuma peça)
// posição 1    0     0     1 
// posição 2    0     1     0 
// posição 3    0     1     1 
// posição 4    1     0     0  
// posição 5    1     0     1 
// posição 6    1     1     0
// posição 7    1     1     1
 
 if (check1<95) {
  p1 = 0;
 } else if ((check1>=95)&&(check1<=190)){
   p1 = 1;
   } else if ((check1>190)&&(check1<=290)){
     p1 = 2;   
     } else if ((check1>290)&&(check1<=385)){
       p1 = 3; 
       } else if ((check1>385)&&(check1<=450)){
         p1 = 4;
         } else if ((check1>450)&&(check1<=590)){
           p1 = 5;
           } else if ((check1>590)&&(check1<=695)){
             p1 = 6;
             } else if ((check1>695)&&(check1<=810)){
               p1 = 7;
               } else {
                 p1 = 0;}

 if (check2<95) {
  p2 = 0;
 } else if ((check2>=95)&&(check2<=190)){
   p2 = 1;
   } else if ((check2>190)&&(check2<=290)){
     p2 = 2;   
     } else if ((check2>290)&&(check2<=385)){
       p2 = 3; 
       } else if ((check2>385)&&(check2<=450)){
         p2 = 4;
         } else if ((check2>450)&&(check2<=590)){
           p2 = 5;
           } else if ((check2>590)&&(check2<=695)){
             p2 = 6;
             } else if ((check2>695)&&(check2<=810)){
               p2 = 7;
               } else {
                 p2 = 0;}                 
                 
 if (check3<95) {
  p3 = 0;
 } else if ((check3>=95)&&(check3<=190)){
   p3 = 1;
   } else if ((check3>190)&&(check3<=290)){
     p3 = 2;   
     } else if ((check3>290)&&(check3<=385)){
       p3 = 3; 
       } else if ((check3>385)&&(check3<=450)){
         p3 = 4;
         } else if ((check3>450)&&(check3<=590)){
           p3 = 5;
           } else if ((check3>590)&&(check3<=695)){
             p3 = 6;
             } else if ((check3>695)&&(check3<=810)){
               p3 = 7;
               } else {
                 p3 = 0;}                   

 
Serial.print("Estado dos medidores: ");
Serial.print("P1: ");
Serial.print(p1);
Serial.print(", ");
Serial.print("P2: ");
Serial.print(p2);
Serial.print(", ");
Serial.print("P3: ");
Serial.print(p3);
Serial.println(",");

//printando o estado do jogo

// Estados possíveis
//
// 0 - nenhum
// 1 - pequeno(P) / 2 - médio(M) / 3 - PM
// 4 - grande(G) / 5 - PG / 6 - MG / 7 - todos
//
// Nunca poderá ter o mesmo estado em cada posição (p1,p2 ou p3)
//


//            P1    P2    P3
//estado00    0     0     0    (Nenhuma peça)

//estado01    0     0     7 
//estado02    0     7     0    (Todas juntas)
//estado03    7     0     0 

//estado04    1     6     0 
//estado05    1     0     6  
//estado06    6     1     0 
//estado07    6     0     1    (1 + 6)
//estado08    0     6     1 
//estado09    0     1     6

//estado10    4     0     3 
//estado11    4     3     0  
//estado12    3     0     4 
//estado13    3     4     0    (4 + 3)
//estado14    0     4     3 
//estado15    0     3     4

//estado16    2     0     5 
//estado17    2     5     0  
//estado18    5     0     2 
//estado19    5     2     0    (2 + 5)
//estado20    0     2     5 
//estado21    0     5     2

//estado22    1     2     4 
//estado23    1     4     2  
//estado24    2     1     4 
//estado25    2     4     1    (1 + 2 + 4)
//estado26    4     1     2 
//estado27    4     2     1

b=0;

if ((p1==0)&&(p2==0)&&(p3==0)) {
 estado=0;
 } else if ((p1==0)&&(p2==0)&&(p3==7)) {
   estado = 1;
 } else if ((p1==0)&&(p2==7)&&(p3==0)) {
   estado = 2;
 } else if ((p1==7)&&(p2==0)&&(p3==0)) {
   estado = 3;
 } else if ((p1==0)&&(p2==1)&&(p3==6)) {
   estado = 4;
 } else if ((p1==0)&&(p2==6)&&(p3==1)) {
   estado = 5;
 } else if ((p1==1)&&(p2==0)&&(p3==6)) {
   estado = 6;
 } else if ((p1==1)&&(p2==6)&&(p3==0)) {
   estado = 7;
 } else if ((p1==6)&&(p2==0)&&(p3==1)) {
   estado = 8;
 } else if ((p1==6)&&(p2==1)&&(p3==0)) {
   estado = 9;
 } else if ((p1==0)&&(p2==3)&&(p3==4)) {
   estado = 10;
 } else if ((p1==0)&&(p2==4)&&(p3==3)) {
   estado = 11;
 } else if ((p1==3)&&(p2==0)&&(p3==4)) {
   estado = 12;
 } else if ((p1==3)&&(p2==4)&&(p3==0)) {
   estado = 13;
 } else if ((p1==4)&&(p2==0)&&(p3==3)) {
   estado = 14;
 } else if ((p1==4)&&(p2==3)&&(p3==0)) {
   estado = 15;
 } else if ((p1==0)&&(p2==2)&&(p3==5)) {
   estado = 16;
 } else if ((p1==0)&&(p2==5)&&(p3==2)) {
   estado = 17;
 } else if ((p1==2)&&(p2==0)&&(p3==5)) {
   estado = 18;
 } else if ((p1==2)&&(p2==5)&&(p3==0)) {
   estado = 19;   
 } else if ((p1==5)&&(p2==0)&&(p3==2)) {
   estado = 20;
 } else if ((p1==5)&&(p2==2)&&(p3==0)) {
   estado = 21;
 } else if ((p1==1)&&(p2==2)&&(p3==4)) {
   estado = 22;
 } else if ((p1==1)&&(p2==4)&&(p3==2)) {
   estado = 23;
 } else if ((p1==2)&&(p2==1)&&(p3==4)) {
   estado = 24;
 } else if ((p1==2)&&(p2==4)&&(p3==1)) {
   estado = 25;
 } else if ((p1==4)&&(p2==1)&&(p3==2)) {
   estado = 26;   
 } else if ((p1==4)&&(p2==2)&&(p3==1)) {
   estado = 27;
 } else {b=1;}
 
if (b==0){
Serial.print("Estado: ");
Serial.print(estado);
Serial.println(",");
} else {
  Serial.println("Estado: INVALIDO");
}
estado = 0;


  } //end if      
      
      
    // Faz com que o estado do LED mude para ALTO, ou seja, liga o LED:
    ledState = HIGH;
    digitalWrite(ledPin, ledState);    

}

// Aqui começa as funções responsáveis pela leitura dos sensores
unsigned long ReadCount1()
{
  unsigned long Count1 = 0;
  unsigned char i1;
  
  digitalWrite(ADSK1, LOW);
  
  while(digitalRead(ADDO1));
  
  for(i1=0;i1<24;i1++)
  {
     digitalWrite(ADSK1, HIGH);
     Count1 = Count1 << 1;
     digitalWrite(ADSK1, LOW);
     if(digitalRead(ADDO1)) Count1++;
  } //end for
  
  digitalWrite(ADSK1, HIGH);
  Count1 = Count1^0x800000;
  digitalWrite(ADSK1, LOW);
  
  return(Count1);

} //end ReadCount1

unsigned long ReadCount2()
{
  unsigned long Count2 = 0;
  unsigned char i2;
  
  digitalWrite(ADSK2, LOW);
  
  while(digitalRead(ADDO2));
  
  for(i2=0;i2<24;i2++)
  {
     digitalWrite(ADSK2, HIGH);
     Count2 = Count2 << 1;
     digitalWrite(ADSK2, LOW);
     if(digitalRead(ADDO2)) Count2++;
  
  } //end for
  
  digitalWrite(ADSK2, HIGH);
  Count2 = Count2^0x800000;
  digitalWrite(ADSK2, LOW);
  
  return(Count2);

} //end ReadCount2

unsigned long ReadCount3()
{
  unsigned long Count3 = 0;
  unsigned char i3;
  
  digitalWrite(ADSK3, LOW);
  
  while(digitalRead(ADDO3));
  
  for(i3=0;i3<24;i3++)
  {
     digitalWrite(ADSK3, HIGH);
     Count3 = Count3 << 1;
     digitalWrite(ADSK3, LOW);
     if(digitalRead(ADDO3)) Count3++;
  
  } //end for
  
  digitalWrite(ADSK3, HIGH);
  Count3 = Count3^0x800000;
  digitalWrite(ADSK3, LOW);
  
  return(Count3);

} //end ReadCount3
