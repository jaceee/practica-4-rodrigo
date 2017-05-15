
// Variables

// - Estado global

int estado = 0;

// - Generación PWM

struct GeneracionPwm {
  int pin = 12;
  int valor = 127;
} generacionPwm;

// - Selección de canal

struct SeleccionDeCanal {
  int  pin = 10;
  long millisUltimaEjecucion = 0;
  long millisEspera = 6000;
  bool estado = false;
} seleccionDeCanal;


// Implementación

void setup() {
  // Configurar entradas y salidas
  pinMode(generacionPwm.pin, OUTPUT);
  pinMode(seleccionCanal.pin, OUTPUT);

  // Iniciar Pwm
  analogWrite(generacionPwm.pin, generacionPwm.valor);
}

void loop() {
  seleccionarCanal();

  switch (estado) {

  case 0:
    iniciar();
    break;

  case 1:
    break;

  case 2:
    leer();
    break;

  }
}

void seleccionarCanal() {

}

void iniciar() {

}

void leer() {

}

void mostrar() {

}
