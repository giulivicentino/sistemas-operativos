#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

// consumidor
int main() {
  const int SIZE = 4096;
  const char *name = "OS";

  int shm_fd;
  char *ptr;
  int i;

  shm_fd = shm_open(name,O_RDONLY,0666);
  if (shm_fd == -1) {
    printf("shared memory failed\n");
    exit(-1);
  }

  ptr = mmap(0,SIZE,PROT_READ,MAP_SHARED,shm_fd,0);
  if (ptr == MAP_FAILED) {
    printf("Map failed\n");
    exit(-1);
  }

  printf("%s", ptr);

  if (shm_unlink(name) == -1) { // elimina el objeto de memoria compartida
    printf("Error removing %s\n",name);
    exit(-1);
  }

  close(shm_fd);
  
  return 0;
}

