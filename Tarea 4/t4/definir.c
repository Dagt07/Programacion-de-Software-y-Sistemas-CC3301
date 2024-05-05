#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "pss.h"
//... define variables globales o funciones adicionales ...
#define linesize (80+1)

int main(int argc, char *argv[]) {
  
  if (argc != 4) {
    fprintf(stderr,"Uso: ./definir <diccionario> <llave> <definicion>\n");
    return 1;
  }

  char *nomArch = argv[1];
  FILE *file = fopen(nomArch, "r+"); //apertura archivo con modo r+ requerido
  if (file == NULL){                 //manejo de error por apertura de archivo
    perror(nomArch);
    exit(1);
  }

  char buff[linesize];
  char *llave = argv[2];
  int tamllave = strlen(argv[2]);
  char *definicion = argv[3];
  int tamdefinicion = strlen(argv[3]);

  //calculamos el tamaño y el numero de lineas en el archivo usando fseek y ftell
  fseek(file, 0 , SEEK_END); //nos movemos al final
  int nrocaracteres = ftell(file); //retorna valor del cursor al final del archivo, por ende conocemos su cantidad de caracteres
  //ya que un archivo de texto esta compuesto por caracteres donde cada 1 tiene sizeof(char) --> sizeof(char)*nrocaracteres = tamaño en bytes
  int tamanoarch = sizeof(char)*nrocaracteres; 
  int nrolineas = tamanoarch/linesize; //tamaño y cada linea siendo de 81 de largo ----> tamano/lenlinea nos da el numero de lineas

  //Usamos Hashing para saber a que linea del archivo nos interesa ir
  //luego hay que ver si es posible posicionar la llave en el lugar, esto nos da los siguientes casos:
  int hashingresult = hash_string(llave)%nrolineas; 
  fseek(file, linesize*hashingresult, SEEK_SET);
  fread(buff,tamllave,1,file);
  buff[tamllave] = 0;

  //Caso linea libre, se inserta la llave
  if(buff[0] == ' '){
    fseek(file,linesize*hashingresult, SEEK_SET);
    fwrite(llave,tamllave,1,file);
    fputc(':',file);
    fwrite(definicion,tamdefinicion,1,file);
    fclose(file);
    exit (0);
  }
  //Caso linea ocupada, usar metodo de hashing lineal probing
  else if (buff[0] != ' '){
    int flag = 0;
    int cont = 0;
    for(int i=0; i<nrolineas; i++){
      fseek(file, hashingresult*linesize,SEEK_SET);
      fread(buff,tamllave,1,file);
      if(buff[0]==' '){
        flag = 1;
        break;
      }
      //Caso ya esta la llave --> gestionar error
      if(strcmp(llave,buff) == 0){
        fprintf(stderr, "La llave %s ya se encuentra en el diccionario\n", llave);
        fclose(file);
        exit(1);
      }
      hashingresult++;
      hashingresult %= nrolineas;
      cont++;
    }
    if(flag == 1){ //encontramos la linea vacia haciendo lineal probing
      fseek(file,-tamllave,SEEK_CUR);
      fwrite(llave,tamllave,1,file);
      fputc(':',file);
      fwrite(definicion,tamdefinicion,1,file);
      fclose(file);
      exit(0);
    }
    else if(cont == nrolineas){ //Caso archivo lleno
      fprintf(stderr,"dicc-full.txt: el diccionario esta lleno\n");
      exit(1);
    }
  }

  fclose(file);
}
