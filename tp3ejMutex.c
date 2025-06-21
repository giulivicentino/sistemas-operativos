/* mut.c - mut, operar, incrementar */
#include <xinu.h>
//variables globales
void operar(void), incrementar(void);
unsigned char x = 0;
sid32 sem;

void tp3ejMutex(void){
  int i;
  sem = semcreate(1);
  resume( create(operar, 1024, 20, "process 1", 0) );
  resume( create(incrementar, 1024, 20, "process 2", 0) );
  sleep(10);
}

void operar(void){
  int y = 0;
  printf("Si no existen mensajes de ERROR entonces todo va OK! \n");
  while (1) {
    /* si x es multiplo de 10 */
    wait(sem);
    if ((x % 10) == 0) {
      y = x * 2; /* como y es el doble de x entonces
		  * y es multiplo de 10 tambien
		  */
      /* si y no es multiplo de 10 entonces hubo un error */
      if ((y % 10) != 0){
	printf("\r ERROR!! y=%d, x=%d \r", y, x);
      }
    }
    signal(sem);
  }
}

void incrementar(void){
  while (1) {
    wait(sem);
    x = x + 1;
    signal(sem);
  }
}
