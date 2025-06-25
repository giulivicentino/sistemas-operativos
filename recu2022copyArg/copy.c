#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int file_get_size(const char *filename);

int main(int argc, char *argv[])
{
    //char archivoOrigen[] = argv[1];
    if (argc < 3) {
        printf("error: al menos ejecutar con 2 argumentos\n");
        exit(1);
    }

    printf("argumento1: %s     argumento2: %s \n", argv[1], argv[2]);

    int tamano = file_get_size(argv[1]);
    if (tamano == -1)
    {
        return 1; 
    }
    

    int f1 = open(argv[1], O_RDONLY);
    if (f1 == -1)
    {
        perror("Error al abrir el archivo de origen");
        return 1;
    }

    // Reservamos memoria para guardar el contenido
    char *contenido = malloc(tamano);
    if (contenido == NULL)
    {
        printf("ERROR malloc\n");
        close(f1);
        exit(1);
    }
    // Leemos el contenido completo
    int totalLeido = 0;
    while (totalLeido < tamano)
    {
        int leido = read(f1, contenido + totalLeido, tamano - totalLeido);
        if (leido == -1)
        {
            perror("Error al leer el archivo");
            free(contenido);
            close(f1);
            return 1;
        }
        totalLeido += leido;
    }


    // Creamos el nuevo archivo
    int f2 = open(argv[2], O_RDWR | O_TRUNC | O_CREAT, S_IRWXU);
    if (f2 == -1)
    {
        perror("Error al crear el archivo de destino");
        free(contenido);
        return 1;
    }

    // Escribimos el contenido en el nuevo archivo
    int totalEscrito = 0;
    while (totalEscrito < tamano)
    {
        int escrito = write(f2, contenido + totalEscrito, tamano - totalEscrito);
        if (escrito == -1)
        {
            perror("Error al escribir en el archivo destino");
            free(contenido);
            close(f2);
            return 1;
        }
        totalEscrito += escrito;
    }

    // Liberamos memoria y cerramos el archivo destino
    free(contenido);
    close(f2);

    return 0;
}
int file_get_size(const char *filename)
{
    int fd = open(filename, O_RDONLY);
    if (fd == -1)
    {
        perror("Error al abrir archivo en file_get_size");
        return -1;
    }

    int tamano = 0;
    char byte;
    int estado = 1;

    while ((estado = read(fd, &byte, 1)) > 0)
    {
        tamano += 1;
    }

    if (estado == -1)
    {
        perror("Error al leer en file_get_size");
        close(fd);
        return -1;
    }

    close(fd);
    return tamano;
}
