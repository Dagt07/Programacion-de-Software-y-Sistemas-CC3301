#include <stdio.h>

#include "suma.h"

Bcd sumaBcd(Bcd x, Bcd y) {
  // Programe aca la funcion pedida
  // queremos sumar ambos numeros en formato BCD, de acuerdo al enunciado asumamos como ejemplo que Bcd x: unsigned long long 0x60219
  // y Bcd y: unsigned long long 0x1, de forma que el resultado esperado debe ser Bcd 0x60220 esto pues Bcd se usa para representar enteros
  // positivos en base 10, por ello va de 0 a 9 al ser decimal al representarse en Bcd, es decir, no puede representar letras ya que en realidad
  // no estamos trabajando como tal en hexadecimal directamente.


  // Nos definimos una mascara que nos ayude
  unsigned long long mask = -1; // el -1 es 1111111111111111111... hasta 64 bits
  mask <<= 4; // para dejar algo del estilo ...1110000 
  mask = ~ mask; // quedando 000...0001111

  unsigned long long carry = 0; // dejamos y inicializado el carry en 0 

  // for para recorrer bit a bit los numeros Bcd, en el fondo queremos separarlos en paquetes de 4 bits con valores posibles desde 0 a 9
  // ejemplo: Bcd x = 0x60219 = 0b 0110 0000 0010 0001 1001 donde cada paquete de 4 bits es un dígito del Bcd, siendo 6 = 0110, 2 = 0010, 1 = 0001
  // 9 = 1001
  
  unsigned long long sumaparcial = 0;
  unsigned long long suma = 0;

  for (int i = 0; i < (sizeof(long long)*8); i+=4){ // i+= 4 para revisar paquetes de 4 hasta justo el final sin ocasionar desborde
    sumaparcial = (x & mask) + (y & mask) + carry;
    if (sumaparcial > 9){
      // si entramos a este caso, es porque es un numero >= 10, por tanto restamos 10 = 0b1010 para compensar y nuestro carry pasa a ser igual a 1
      sumaparcial -=10;
      carry = 1;
    }
    else{
      carry = 0; // cuando no se de el caso anterior siempre el carry debe ser 0, esto pues o bien 1) veniamos con el carry ya en 0 2) el carry se
                 // pudo sumar antes en sumaparcial y no se desbordo, por lo cual debemos volver a asignarle el valor 0 pues ya fue usado
    }
    suma = ((sumaparcial << i) | suma); 
    x >>= 4; 
    y >>= 4; //se va desplazando los Bcd x e y para no estar comparando siempre lo mismo
  }
 

  if (carry == 1){ // Caso de desbordamiento al final
   return 0xffffffffffffffff;
  }
  return suma;
  
}
