#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int archivo_obtener_tamanio(const char nombre_de_archivo[]);

int main(int argc, char *argv[]){
  if (argc < 3) {
    printf("error: al menos ejecutar con 2 argumentos\n");
    exit(1);
  }
  
  printf("argumento1: %s argumento2: %s \n", argv[1], argv[2]);
  /* etc */

  int f1 = open(argv[1], O_RDONLY);
  printf("open: %d",f1);
    /* abrir aqui el archivo argv[1] como solo lectura */

  /* creamos el nuevo archivo */
 int  f2 = open(argv[2], O_RDWR | O_TRUNC | O_CREAT, S_IRWXU);

 /*obtengo el tamaño para usarlo en el malloc y despues para leer el archivo*/
 int tamanio = archivo_obtener_tamanio(argv[1]);
if (tamanio < 0) {
        printf("No se pudo obtener el tamaño del archivo\n");
        return 1;
    }

/* reservo la memoria del puntero*/
 char *puntero = malloc(tamanio);
  if (puntero == NULL) {
        printf("Error al asignar memoria\n");
        close(f1);
        return 1;
    }

 /*lee todos los bytes */
  int estado_read = read(f1,puntero,tamanio); // guardo la data del archivo 1 en el puntero
  if (estado_read < 0) {
        printf("%d\n",estado_read);
        printf("Error al leer el archivo completo\n");
        free(puntero);
        close(f1);
        return 1;
    }

  int w1=  write( f2, puntero ,tamanio);
  //int w1=  write( f2, f1 ,tamanio);
  printf("que writeoooo :     %d",w1);
}



int archivo_obtener_tamanio(const char nombre_de_archivo[]) {
    int tamanio = 0;
    int estado_read = 1;
    char buffer;

    int fd = open(nombre_de_archivo, O_RDONLY);

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
