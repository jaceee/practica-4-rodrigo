
// Includes

// SPI - Version: Latest 
#include <SPI.h>

#include "MAX7219.h"


// Variables

// - Estado global

int estado = 0;

// - Generación PWM

struct GeneracionPwm {
  int  pin = A2;
  long millisUltimaEjecucion = 0;
  long millisEspera = 1;
  bool estado = false;
} generacionPwm;

// - Selección de canal

struct SeleccionDeCanal {
  int  pin = A1;
  long millisUltimaEjecucion = 0;
  long millisEspera = 6000;
  bool estado = false;
} seleccionDeCanal;

// - Iniciar conversión

struct InicioDeConversion {
  int  pin = A0;
  int  millisUltimaEjecucion = 0;
  int  millisEspera = 1;
  bool estado = false;
} inicioDeConversion;

// - Lectura EOC

struct LecturaEOC {
  int pin = A3;
} lecturaEOC;

// - Datos

struct Datos {
  int  pines[8] = { 9, 8, 7, 6, 5, 4, 3, 2 };
  byte valor = 0;
} datos;

// - MAX7219

struct ConfPantalla {
  int pin = 10;
  int digitos = 8;
} confPantalla;

MAX7219 pantalla(confPantalla.pin, confPantalla.digitos);


// Implementación

void setup() {
  // Configurar entradas y salidas
  pinMode(generacionPwm.pin, OUTPUT);
  pinMode(seleccionDeCanal.pin, OUTPUT);
  pinMode(inicioDeConversion.pin, OUTPUT);
  pinMode(lecturaEOC.pin, INPUT);

  for (int i = 0; i < 8; i++) {
    pinMode(datos.pines[i], INPUT);
  }

  Serial.begin(9600);
  
  // MAX7219
  SPI.begin();
  pantalla.inicializar(); // SPI debe estar inicializado antes de inicializar el MAX7219 (que usa SPI)
  pantalla.apagar();
  pantalla.desactivar_modo_prueba();
  pantalla.brillo(15); // Brillo medio (va desde cero hasta 15)
  pantalla.borrar();
  pantalla.encender();
}

void loop() {
  generarPwm();
  seleccionarCanal();

  switch (estado) {

  case 0:
    iniciar();
    break;

  case 1:
    esperarEOC();
    break;

  case 2:
    leer();
    break;

  }
}

void generarPwm() {
  long tActual = millis();
  if (tActual - generacionPwm.millisUltimaEjecucion > generacionPwm.millisEspera) {
    generacionPwm.millisUltimaEjecucion = tActual;
    generacionPwm.estado = !generacionPwm.estado;
    digitalWrite(generacionPwm.pin, generacionPwm.estado ? HIGH : LOW);
  }
}

void seleccionarCanal() {
  long tActual = millis();
  if (tActual - seleccionDeCanal.millisUltimaEjecucion > seleccionDeCanal.millisEspera) {
    Serial.println("seleccionando canal");
    
    seleccionDeCanal.millisUltimaEjecucion = tActual;
    seleccionDeCanal.estado = !seleccionDeCanal.estado;
    digitalWrite(seleccionDeCanal.pin, seleccionDeCanal.estado ? HIGH : LOW);
  }
}

void iniciar() {
  long tActual = millis();
  
  if (!inicioDeConversion.estado) {
    inicioDeConversion.millisUltimaEjecucion = tActual;
    inicioDeConversion.estado = true;
    digitalWrite(inicioDeConversion.pin, HIGH);
  } else if (tActual - inicioDeConversion.millisUltimaEjecucion > inicioDeConversion.millisEspera) {
    inicioDeConversion.estado = false;
    digitalWrite(inicioDeConversion.pin, LOW);

    estado = 1;
  }
}

void esperarEOC() {
  if (digitalRead(lecturaEOC.pin) == HIGH) {
    estado = 2;
  }
}

void leer() {
  byte valor = 0;
  for (int i = 0; i < 8; i++) {
    if (digitalRead(datos.pines[i]) == HIGH) {
      valor += (byte)(1 << i);
    }
  }
  datos.valor = valor;

  char buff[5];

  itoa(valor, buff, 10);

  Serial.write("Valor: ");
  Serial.write(buff);
  Serial.println();

  mostrar();
  
  estado = 0;
}

void mostrar() {
  pantalla.borrar();
  pantalla.mostrar_cifra(datos.valor, 0, false);
}
