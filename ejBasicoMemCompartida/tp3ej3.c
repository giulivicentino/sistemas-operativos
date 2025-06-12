#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>


// productor
int main() {
  const int SIZE = 4096;
  const char *name = "OS";
  
  int shm_fd; // id de la memoria compartida
  char *ptr; // puntero para acceder a la memoria

  //crea un objeto de memoria compartida(nombre, permisos, modo de permisos)
  shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);

  ftruncate(shm_fd,SIZE); //ajusta el tamaño de memoria (por que al principio es 0)

  // puntero apunta al mapeo de la memoria compartida al espacio de direcciones del proceso
  ptr = (char*)mmap(0,SIZE,PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd,0);
  if (ptr == MAP_FAILED) {
    printf("Map failed\n");
    return -1;
  }

  //abre el archivo solo modo lectura
 
  int fd = open("/home/giulivicentino/Escritorio/recuSO/ejBasicoMemCompartida/superbbtrickz.txt",O_RDONLY);

  if (fd < 0) { // funcion open devuelve un descriptor
    printf("error de abrir");
    return -1;
  }
  
  char contenido[SIZE]; //arreglo de caracteres de tamaño 4096, funciona como buffer para guardar lo que lee del descriptor de archivo

  // lee SIZE bytes de fd y los guarda en el buffer contenido
  int bytesLeidos = read(fd, contenido, SIZE);
  
  if (bytesLeidos < 0) {
    printf("error de lectura");
    return -1;
  }

  close(fd); //cierra el descriptor del archivo
  
  sprintf(ptr, "%s", contenido);
  return 0;
}
