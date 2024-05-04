#include <stddef.h>

#include "prev.h"

void asignarPrev(Nodo *t, Nodo **pprev) {
  
  //caso base
  if (t == NULL){
    return;
  }
  //subarbol izq
  asignarPrev(t->izq,pprev);
  //condicion centro
  t->prev = *pprev;
  t->prox = NULL;
  if(t->prev != NULL){
      (t->prev)->prox = t; 
  }
  *pprev = t;
  //subarbol der
  asignarPrev(t->der,pprev);
}
