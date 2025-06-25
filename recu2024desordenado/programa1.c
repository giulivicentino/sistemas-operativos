#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int calcular_tamanio(char ruta[]);

int main(){

  char *ruta = "/home/giulivicentino/Escritorio/recuSO/recu2024desordenado/desordenado.bin";
  int SIZE = calcular_tamanio(ruta) + 4; // se agregan 4 mas para el primer numero 
  int fd = open(ruta,O_RDONLY);
 printf("AUXILIO QUE TAMAÑO TIENEE        : %d         ", SIZE);
  // crear memoria compartida
  int shm_fd;
  int *ptr;

  shm_fd = shm_open("OS", O_CREAT | O_RDWR, 0666);
  ftruncate(shm_fd,SIZE);
  ptr = (int*)mmap(0,SIZE,PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd,0); //(int*) para poder acceder como un arreglo
  if (ptr == MAP_FAILED) {
    printf("Map failed\n");
    return -1;
  }

  printf("DIRECCION VIRTUAL DE MEMORIA COMPARTIDA: %p", ptr);
  // primer posicion de memoria compartida en 0
  ptr[0] = 0;
  printf("Primer byte de la memoria compartida: %d", ptr[0]);
  // + 4 para que no pise el primer 0
  read(fd, &ptr[1], SIZE - 4); // guarda size-4 bytes del archivo y los guarda en el buffer ptr[1]
  ptr[0] = SIZE;

  
  while(ptr[0] != 1){//espera a que ordene
    usleep(1000);
  }
  printf("Primer byte de la memoria compartida ya listo: %d", ptr[0]);

  //creo el archivo donde lo voy a guardar
  int f2 = open("/home/giulivicentino/Escritorio/recuSO/recu2024desordenado/ordenado.bin", O_RDWR | O_CREAT | O_TRUNC, 0666);
  if (f2 == -1) {
    perror("Error al crear el archivo de destino");
    return 1;
  }

  //escribo todo el contenido de ptr en el nuevo archivo en f2
  write(f2, ptr, SIZE);

  printf("Contenido despues de ordenar:\n");
  int totalInts = (SIZE - 4) / 4; //  -4 para que no imprima el primero, y /4 para que la cantidad de iteraciones sea la cant de numeros(1 num 4 bytes), en vez de la cant total de bytes
  for (int i = 1; i <= totalInts; i++) {
    printf(" %d, ", ptr[i]);
  } 
  printf("\n");
  
  close(f2);
  close(shm_fd);//cierro la memoria compartida
  close(fd);
}
int calcular_tamanio(char ruta[]) {
    int tamanio = 0;
    int estado_read = 1;
    char buffer;
    int fd = open(ruta, O_RDONLY);
    while (estado_read > 0) {
        estado_read = read(fd, &buffer, 1);

        if (estado_read == -1){
            printf("Error al leer archivo, error: %d\n", estado_read);
            return -1;
        }
        tamanio++;
    }
    close(fd);
    return tamanio;
}
