
// Variables

// - Estado global

int estado = 0;

// - Generación PWM

struct GeneracionPwm {
  int pin = 12;
  long millisUltimaEjecucion = 0;
  long millisEspera = 1;
  bool estado = false;
} generacionPwm;

// - Selección de canal

struct SeleccionDeCanal {
  int  pin = 10;
  long millisUltimaEjecucion = 0;
  long millisEspera = 6000;
  bool estado = false;
} seleccionDeCanal;

// - Iniciar conversión

struct InicioDeConversion {
  int pin = 11;
  int millisUltimaEjecucion = 0;
  int millisEspera = 5;
  bool estado = false;
} inicioDeConversion;


// Implementación

void setup() {
  // Configurar entradas y salidas
  pinMode(generacionPwm.pin, OUTPUT);
  pinMode(seleccionDeCanal.pin, OUTPUT);
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
    seleccionDeCanal.millisUltimaEjecucion = tActual;
    seleccionDeCanal.estado = !seleccionDeCanal.estado;
    digitalWrite(seleccionDeCanal.pin, seleccionDeCanal.estado ? HIGH : LOW);
  }
}

void iniciar() {

}

void esperarEOC() {
  
}

void leer() {

}

void mostrar() {

}

