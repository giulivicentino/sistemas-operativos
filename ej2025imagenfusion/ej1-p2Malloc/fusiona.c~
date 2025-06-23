#include "imagen1.h"
#include "imagen2.h"
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h> //para el memcpy
 #include <sys/mman.h>
       #include <sys/stat.h>        /* For mode constants */
      
int main(){

  unsigned char fus[270015]; //arreglo de la fusion
  int i;
  for(i=0; i<15;i++){
     fus[i]= cabecera1[i];
  }
  int j = 0;
  for(i=0; i<270000; i++){
      if(i%2==0){
	fus[15+i]=pixels2[i];
      }else{
	fus[15+i]=pixels1[i];
      }
    }


  const int SIZE = 270015;
  const char *name = "OS";

  int shm_fd;
  
  shm_fd = shm_open(name,O_CREAT | O_RDWR, 0666); //CREO LA MEMORIA COMPARTIDA

  ftruncate(shm_fd,SIZE);//ajusto el tamaño de la memoria

  // puntero apunta al mapeo de la memoria compartida al espacio de direcciones del proceso
  char *ptr = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

  if (ptr == MAP_FAILED) {
    printf("Map failed\n");
    return -1;
  }
  // pongo tooodo el arreglo en la memoria compartida
  memcpy(ptr, fus, 270015);

  return 0;
 
}
