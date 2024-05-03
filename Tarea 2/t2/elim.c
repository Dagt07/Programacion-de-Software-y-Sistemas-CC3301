#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "elim.h"

void eliminar(char *str, char *pat) {

  // *str es el string original que vamos modificando, eliminando las apariciones del patron
  // *pat es el string del patron

  //Punteros adicionales a usar
  char *straux = str; //puntero auxiliar para recorrer el string en busqueda del patron
  char *strinicio = str;
  char *patinicio = pat; //puntero referencia para volver a resetear el patron luego de cada busqueda
  int cont = 0; //contador de ocurrencias
  int largopatron = strlen(pat); //largo del patron
  
  while (*straux != 0){

    if (*straux == *pat){ //Si hay primera coincidencia
      
      while(((*straux) == (*pat)) && *pat != 0){ //busca patron en el str mientras no se acabe el pat
        cont+=1;
        straux++;
        pat++;
      }
      //El while anterior pudo terminar por 2 razones, por eso ahora es necesario ver si se encontro o no el patron
      if(cont == largopatron){ //Encontro el patron, debe ser eliminado
        if(*straux == 0){ //antes de avanzar str debo confirmar si straux tiene sentido aún, es decir, si se acabo el string
          break;
        }
      }
      else{ //no encontro el patron, debe conservarse
        straux -= cont;        
        *str = *straux;
        straux++;
        str++;
      }    
    }
    else{ //No hubo coincidencia inmediata 
      *str = *straux;
      straux++; 
      str++;
    }

    pat = patinicio; //en cualquier caso ya sea, encontramos el patron o no, debemos volver a su inicio el puntero del patron y
    cont = 0;        //resetear el contador de ocurrencias
  }

  *str = 0; //se agrega el caracter nulo al final del string para indicar que se acabo
  str = strinicio; //vuelvo a resetear el puntero al inicio del string
}


char *eliminados(char *str, char *pat) {
  
  //----------------------VERSION MODIFICADA DE ELIMINAR QUE CUENTA EL PATRON----------------------
  //Punteros adicionales a usar
  char *straux = str; 
  char *patinicio = pat; 
  int cont = 0; 
  int largopatron = strlen(pat); 

  while (*straux != 0){

    if (*straux == *pat){ //Si hay primera coincidencia
      while(((*straux) == (*pat)) && *pat != 0){ //busca patron en el str mientras no se acabe el pat
        straux++;
        pat++;
      }
      //El while anterior pudo terminar por 2 razones, por eso ahora es necesario ver si se encontro o no el patron
      if(cont == largopatron){ //Encontro el patron
        cont += 1; 
        if(*straux == 0){ //antes de avanzar str debo confirmar si straux tiene sentido aún, es decir, si se acabo el string
          break;
        }
      }
      pat = patinicio; 
         
    }
    else{ //No hubo coincidencia inmediata
      straux++; 
      pat=patinicio;
    }

  }
  //Para este punto con cont sabemos cuantas apariciones totales tuvimos del patron, por ende ahora podemos pedir la memoria necesaria
  int lenstr = strlen(str);
  char *strfinal = malloc(sizeof(char)*(lenstr-(cont*largopatron)+1)); //memoria necesaria para el string final
  char *strfinalinicio = strfinal;
  
  //-------------------------Version modificada de eliminar usando la memoria pedida, reemplazando solo lo que no es patron---------------------

  //Punteros adicionales a usar
  char *straux2 = str; //puntero auxiliar para recorrer el string en busqueda del patron
  int cont2 = 0; //contador de ocurrencias
  pat = patinicio; //vuelvo a resetear el patron antes de usarlo

  while (*straux2 != 0){

    if (*straux2 == *pat){ //Si hay primera coincidencia
      
      while(((*straux2) == (*pat)) && *pat != 0){ //busca patron en el str mientras no se acabe el pat
        cont2+=1;
        straux2++;
        pat++;
      }
      //El while anterior pudo terminar por 2 razones, por eso ahora es necesario ver si se encontro o no el patron
      if(cont2 == largopatron){ //Encontro el patron, debe ser eliminado
        if(*straux2 == 0){ //antes de avanzar str debo confirmar si straux2 tiene sentido aún, es decir, si se acabo el string
          break;
        }
      }
      else{ //no encontro el patron, debe conservarse
        straux2 -= cont2;        
        *strfinal = *straux2;
        straux2++;
        strfinal++;
      }       
    }
    else{ //No hubo coincidencia inmediata
      *strfinal = *straux2;
      straux2++; 
      strfinal++;
    }

    pat = patinicio; //en cualquier caso ya sea, encontramos el patron o no, debemos volver a su inicio el puntero del patron y
    cont2 = 0;        //resetear el contador de ocurrencias
  }
  
  *strfinal = 0; //se agrega el caracter nulo al final del string para indicar que se acabo
  strfinal = strfinalinicio; //vuelvo a resetear el puntero al inicio del string final

  return strfinal; //retorna el string final
}


