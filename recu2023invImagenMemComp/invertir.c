#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h> //para el memcpy
#include <sys/mman.h>
#include <sys/stat.h>
#include <stdlib.h>
/* For mode constants */
/*2. Un segundo programa lee de la memoria compartida la foto, y la guarda en un nuevo archivo de manera
  invertida.*/
int main(){
  const int SIZE =  50261;
  const char *name = "OS";
  int shm_fd;
  char *ptr;
  printf("DIRECCION VIRTUAL DEL MAIN: %p\n ",main);
  // DIRECCION VIRTUAL DEL MAIN: 0x5e9253d73269
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
  printf("DIRECCION MEMORIA COMPARTIDA: %p\n ",ptr);
   // DIRECCION MEMORIA COMPARTIDA: 0x70a04b36b000

  //abre el archivo
  int f = open("invertida.ppm",O_RDWR|O_CREAT,0666);

  int *imagenOriginal = (int*) ptr;
  // read(f,imagen,SIZE);// lee todo el contendido del archivo que indica f1 y lo guarda en el arreglo imagen
  unsigned char imagenInvertida[50261];

  int i;
  write(f,ptr,15); //escribo tal cual los primeros 15 bytes de la cabecera

  
  int j;
  for(j=SIZE;j>= 15 ;j--){
    write(f,&ptr[j],1); //asi va escribiendo desde el ultimo hasta la cabecera
  }// el & agarra la direccion de ese puntero, por eso puede guardarlo

  close(f);

  //cierro la memoria compartida
  if (shm_unlink(name) == -1) {
    printf("Error removing %s\n",name);
    exit(-1);
  }
  
  close(shm_fd);
  return 0;
}
/* *NOTA 2: Si se quiere utilizar la memoria compartida como si fuese un arreglo, se puede hacer así:
   int * arreglo = (int) ptr;
   arreglo[1] = 3; / se modifica el segundo entero en la memoria compartida */
