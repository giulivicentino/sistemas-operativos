#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h> //para el memcpy
#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
/*Implemente en Linux dos programas:
  1. Un programa crea memoria compartida, y carga en la memoria compartida la foto cat.pgm, usando
  funciones para acceder a archivos en el sistema de archivos.*/
int main(){
  int SIZE = 50261;

  //leo la imagen t guardo el descriptor de archivo en f1
  int f1 = open("/home/giulivicentino/Escritorio/recuSO/recu2023copyMemComp/cat.pgm", O_RDONLY);
  if (f1 == -1)
    {
      perror("Error al abrir el archivo de origen");
      return 1;
    }

  unsigned char imagen[50261];
  read(f1,imagen,SIZE);// lee todo el contendido del archivo que indica f1 y lo guarda en el arreglo imagen

     const char *name = "OS";

  int shm_fd;
  //CREO LA MEMORIA COMPARTIDA
  shm_fd = shm_open(name,O_CREAT | O_RDWR, 0666); 

  ftruncate(shm_fd,SIZE);//ajusto el tamaño de la memoria

  // puntero apunta al mapeo de la memoria compartida al espacio de direcciones del proceso
  char *ptr = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

  if (ptr == MAP_FAILED) {
    printf("Map failed\n");
    return -1;
  }
  // pongo tooodo el arreglo en la memoria compartida
  memcpy(ptr, imagen, 50621);
}
