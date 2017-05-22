// MAX7219.cpp

#include <SPI.h> // La librería MAX7219 necesita que la librería SPI esté operativa y para no interferir con otros usos NO la inicializa
#include "MAX7219.h"

MAX7219::MAX7219() // Constructor sin parámetros (en Arduino suele usarse CS=10, PIN_CS, y el número de dígitos de los módulos suele ser 8, NUMERO_DIGITOS)
{
  pin_cs=PIN_CS;
  numero_digitos=NUMERO_DIGITOS;
  // inicializar(PIN_CS,NUMERO_DIGITOS); // Como utiliza SPI debe inicializarse cuando SPI ya esté inicializado
}

MAX7219::MAX7219(unsigned char numero_pin_cs) // El número de dígitos de los módulos suele ser 8 (NUMERO_DIGITOS)
{
  pin_cs=numero_pin_cs;
  numero_digitos=NUMERO_DIGITOS;
  //inicializar(pin_cs,NUMERO_DIGITOS); // Como utiliza SPI debe inicializarse cuando SPI ya esté inicializado
}

MAX7219::MAX7219(unsigned char numero_pin_cs, unsigned char maximo_digitos)
{
  pin_cs=numero_pin_cs;
  numero_digitos=maximo_digitos;
  //inicializar(pin_cs,numero_digitos); // Como utiliza SPI debe inicializarse cuando SPI ya esté inicializado
}

MAX7219::~MAX7219() // Destructor
{
}

void MAX7219::inicializar()
{
  // SPI.begin(); // Para no interferir con otro código que utilice SPI, hay que gestionarlo desde el programa
  pinMode(pin_cs,OUTPUT); // El pin al que se conecta CS/SS del MAX7219 debe ser de salida para activarlo/desactivarlo a nivel bajo/alto
  digitalWrite(pin_cs,HIGH); // Desactivar el driver
  /* // Descomentar este bloque si hay problemas con CS/SS en SPI. Si persisten, usar el pin 10 para CS/SS de SPI. En algunas versiones, SPI el pin 10 (por defecto) ser de salida no se conecte a CS/SS del dispositivo (y no debe utilizarse para otra cosa
  pinMode(10,OUTPUT);
  */
  cantidad_digitos();
  minimo=-(long)pow(10,numero_digitos-1)+1; // Hay que renunciar a un dígito para el signo (en este tipo de pantalla)
  maximo=(long)pow(10,numero_digitos)-1;
  digitos_BCD(0B11111111); // Todos los dígitos (8) en formato BCD
}

void MAX7219::enviar(unsigned char registro, unsigned char valor)
{
  digitalWrite(pin_cs,LOW);
  SPI.transfer(registro);
  SPI.transfer(valor);
  digitalWrite(pin_cs,HIGH);
}

void MAX7219::brillo(unsigned char nivel)
{
  enviar(BRILLO_MAX7219,nivel);
}

void MAX7219::encender()
{
  enviar(ACTIVAR_MAX7219,1);
}

void MAX7219::apagar()
{
  enviar(ACTIVAR_MAX7219,0);
}

void MAX7219::activar_modo_prueba()
{
  enviar(MODO_TEST_MAX7219,1);
}

void MAX7219::desactivar_modo_prueba()
{
  enviar(MODO_TEST_MAX7219,0);
}

void MAX7219::digitos_BCD()
{
  enviar(MODO_BCD_MAX7219,0B11111111);
}

void MAX7219::digitos_BCD(unsigned char digitos)
{
  enviar(MODO_BCD_MAX7219,digitos);
}

void MAX7219::cantidad_digitos()
{
  enviar(LIMITE_MAX7219,numero_digitos-1);
}

bool MAX7219::mostrar_cifra(long valor)
{
  return mostrar_cifra(valor,0,false); // Por defecto no tiene coma ni se rellena con ceros
}

bool MAX7219::mostrar_cifra(long valor, char posicion_coma)
{
  return mostrar_cifra(valor,posicion_coma,false); // Por defecto no rellena con ceros
}

bool MAX7219::mostrar_cifra(long valor, char posicion_coma, bool rellenar_ceros)
{
  if(valor>maximo||valor<minimo)
  {
    mostrar_error();
    return false;
  }
  else
  {
    digitos_BCD();
    unsigned char contador_digito=0;
    unsigned char digito;
    bool negativo=valor<0;
    posicion_coma=posicion_coma*(posicion_coma>0&&posicion_coma<numero_digitos-negativo); // Corregir posiciones de coma incorrectas
    if(valor==0)
    {
      while(contador_digito<=posicion_coma)
      {
        digito=COMA*(contador_digito==posicion_coma&&posicion_coma>0); // El dígito es cero y si la posición de la coma es mayor que cero y se ha llegado al dígito en el que representarla añadírsela
        enviar(++contador_digito,digito);
      }
    }
    else
    {
      if(negativo)
      {
        valor=abs(valor);
      }
      while(valor>0)
      {
        digito=(valor%10)|(contador_digito==posicion_coma&&posicion_coma>0)*COMA; // Dígito que toque (resto de 10) con coma si es su posición y es mayor que cero
        enviar(++contador_digito,digito); // Mostrar el dígito y la coma si corresponde. Como el primer dígito es 1, cuando la posición de la coma es cero (o un valor mayor que los dígitos) no se muestra
        valor/=10;
      }
      while(contador_digito<=posicion_coma)
      {
        digito=(contador_digito==posicion_coma)*COMA;
        enviar(++contador_digito,digito);
      }
      if(negativo&&!rellenar_ceros)
      {
        enviar(++contador_digito,SIGNO_MENOS);
      }
    }
    while(contador_digito<NUMERO_DIGITOS-rellenar_ceros*negativo)
    {
      enviar(++contador_digito,rellenar_ceros?0:BORRAR_SEGMENTOS);
    }
    if(negativo&&rellenar_ceros)
    {
      enviar(++contador_digito,SIGNO_MENOS);
    }
    return true;
  }
}

void MAX7219::borrar() // Borra toda la pantalla sin apagarla
{
  for(char contador=0;contador<NUMERO_DIGITOS;contador++)
  {
    enviar(contador+1,BORRAR_SEGMENTOS);
  }
}

void MAX7219::mostrar_error()
{
  digitos_BCD(0B00000000);
  char contador_digito=1;
  enviar(contador_digito++,SEGMENTOS_LETRA_R);
  enviar(contador_digito++,SEGMENTOS_LETRA_O);
  enviar(contador_digito++,SEGMENTOS_LETRA_R);
  enviar(contador_digito++,SEGMENTOS_LETRA_R);
  enviar(contador_digito++,SEGMENTOS_LETRA_E);
  while(contador_digito<=numero_digitos)
  {
    enviar(contador_digito++,SEGMENTOS_APAGADOS);
  }
}

