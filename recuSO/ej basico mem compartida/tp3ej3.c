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
  
  int shm_fd;
  char *ptr;

  shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);

  ftruncate(shm_fd,SIZE);

  ptr = (char*)mmap(0,SIZE,PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd,0);
  if (ptr == MAP_FAILED) {
    printf("Map failed\n");
    return -1;
  }

  int fd = open("/usr/share/doc/aufs-dkms/06mmap.txt",O_RDONLY);

  if (fd < 0) {
    printf("error de abrir");
    return -1;
  }
  
  char contenido[SIZE];
  
  int bytesLeidos = read(fd, contenido, 1024);
  if (bytesLeidos < 0) {
    printf("error de lectura");
    return -1;
  }

  close(fd);
  
  sprintf(ptr, "%s", contenido);
  return 0;
}
