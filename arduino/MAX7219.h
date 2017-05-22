// MAX7219.h

#if defined(ARDUINO) && ARDUINO>=100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

#define MODO_BCD_MAX7219 0x09
#define BRILLO_MAX7219 0x0A
#define LIMITE_MAX7219 0x0B
#define ACTIVAR_MAX7219 0x0C
#define MODO_TEST_MAX7219 0x0F

#define SIGNO_MENOS 0x0A
#define LETRA_E 0x0B
#define LETRA_H 0x0C
#define LETRA_L 0x0D
#define LETRA_P 0x0E
#define BORRAR_SEGMENTOS 0x0F

#define COMA 0B10000000

#define SEGMENTOS_APAGADOS 0B00000000 // Todos los segmentos apagados
#define SEGMENTOS_LETRA_E 0B01001111 // Segmentos ADEFG
#define SEGMENTOS_LETRA_R 0B00000101 // Segmentos EG
#define SEGMENTOS_LETRA_O 0B00011101 // Segmentos CDEG

#define PIN_CS 10
#define NUMERO_DIGITOS 8 // Número de dígitos de 7 segmentos conectados al MAX7219 por defecto

class MAX7219
{
  private:
    unsigned char pin_cs;
    unsigned char numero_digitos;
    long minimo;
    long maximo;
    void digitos_BCD();
  protected:
  public:
    MAX7219(); // Constructor sin parámetros
    MAX7219(unsigned char pin_cs); // El número de dígitos de los módulos suele ser 8
    MAX7219(unsigned char pin_cs, unsigned char numero_digitos);
    ~MAX7219(); // Destructor
    void inicializar();
    void enviar(unsigned char  registro, unsigned char valor);
    void brillo(unsigned char nivel);
    void encender();
    void apagar();
    void activar_modo_prueba();
    void desactivar_modo_prueba();
    void digitos_BCD(unsigned char digitos);
    void cantidad_digitos();
    bool mostrar_cifra(long valor);
    bool mostrar_cifra(long valor, char posicion_coma);
    bool mostrar_cifra(long valor, char posicion_coma, bool rellenar_ceros);
    void borrar();
    void mostrar_error();
};

