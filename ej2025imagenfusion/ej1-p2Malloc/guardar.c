#include "imagen1.h"
#include "imagen2.h"
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <stdlib.h> /* For mode constants */
   
/*Desarrollar un segundo programa guardar.c que solicita un nombre de archivo al usuario y
guarda la imagen fusionada, que se encuentra en la memoria compartida, en ese archivo*/

int main(){
  const int SIZE = 270015;
  const char *name = "OS";
  int shm_fd;
  char *ptr;
 //ABRO LA MEMORIA COMPARTIDA
  shm_fd = shm_open(name,O_RDONLY,0666);
  if (shm_fd == -1) {
    printf("shared memory failed\n");
    exit(-1);
  }

  // puntero apunta al mapeo de la memoria compartida al espacio de direcciones del proceso
   ptr = mmap(0,SIZE,PROT_READ,MAP_SHARED,shm_fd,0);
  if (ptr == MAP_FAILED) {
    printf("Map failed\n");
    exit(-1);
  }

 //abre el archivo solo modo escritura
  int f = open("archivo.ppm",O_RDWR|O_CREAT,0666);
  write(f,ptr,SIZE); //escribo lo que esta en el puntero prt(la memoria compartida con nuestro arreglo fusionaod) hacia f (que es el archivo.ppm"
  close(f);

  //cierro la memoria compartida
  if (shm_unlink(name) == -1) {
    printf("Error removing %s\n",name);
    exit(-1);
  }
  
  close(shm_fd);
  return 0;
}
