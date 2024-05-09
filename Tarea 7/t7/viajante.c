#define _XOPEN_SOURCE 500

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "viajante.h"

int leer(int fd, void *vbuf, int n) {
  char *buf= vbuf;
  do {
    int rc= read(fd, buf, n);
    if (rc<=0)
      return 1; /* fracaso: error o fin del archivo/pipe/socket */
    n-= rc; /* descontamos los bytes leídos */
    buf+= rc; /* avanzamos el buffer para no reescribir lo leido previamente */
  } while (n>0); /* mientras no leamos todo lo que esperamos */
  return 0; /* exito */
}

double viajante_par(int z[], int n, double **m, int nperm, int p) {
  // Complete esta funcion
  // Puede invocar a su version secuencial: viajante
  // Use la funcion leer para que el padre reciba la ruta del hijo.
  // Use fork() para crear p nuevos procesos.  El padre solo obtiene los
  // resultados de los procesos hijos a traves de pipes, entierra a los
  // los hijos y calcula el resultado final.
  // Despues de invocar fork() agregue: srandom(getUSecsOfDay()*getpid());
  // Esto es para que cada proceso genere secuencias de numeros aleatorios
  // diferentes.

  int pids[p]; //se usa p porque es la cantidad de hijos que queremos crear (p procesos pesados)
  int fds[p][2];

  for(int i=0; i<p; i++){
    pipe(fds[i]);
    pids[i] = fork();

    if (pids[i] == 0){ //HIJO
      close(fds[i][0]); //cerramos el extremo de lectura del pipe
      srandom(getUSecsOfDay()*getpid()); //generamos una semilla diferente para cada hijo
      double min = viajante(z, n, m, nperm/p); //calculamos el resultado
      write (fds[i][1], &min, sizeof(double)); //escribimos el resultado en el buffer, que posteriormente leerá el padre
      write (fds[i][1], &z[0], sizeof(int)*(n+1)); //escribimos la ruta en el buffer, que posteriormente leerá el padre
      exit(0); //terminamos el proceso hijo
    }
    else{ //PADRE
      close(fds[i][1]); //cerramos el extremo de escritura del pipe
    }
  }

  double min = DBL_MAX;
  for(int i=0; i<p; i++){
    double res_hijo;
    leer(fds[i][0], &res_hijo, sizeof(double)); //leemos el resultado de cada hijo
  
    if(res_hijo < min && res_hijo != 0.0){ //si el resultado del hijo es menor que el resultado del padre, lo reemplazamos
      min = res_hijo;
      leer(fds[i][0], &z[0], sizeof(int)*(n+1)); //guardamos la ruta de ese hijo que logro el minimo
    }
    waitpid(pids[i], NULL, 0); //esperamos a que cada hijo termine OBS waitpid luego de cada read para no caer en deadlock
    close(fds[i][0]); //cerramos el extremo de lectura del pipe
  }

  return min;
}
