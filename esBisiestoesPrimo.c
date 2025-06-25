#include <xinu.h>

void proc2(pid32);
void proc3(pid32);

void ej() {
  //principal
  //creo procesos
  int pid1 = getpid();
  int pid2 = create(proc2, 1024, 20, "bisiesto", 1, pid1);
  int pid3 = create(proc3, 1024, 20, "primos", 1, pid1);
  resume(pid2);
  resume(pid3);
  //pido numero
  int i;
  char buf[10];
  printf("INGRESE UN NRO: ");
  read(CONSOLE, buf, 10);
  i = atoi(buf);
  printf ("el numero ingresado entero es %d \n", i);

  send(pid2, i);
  send(pid3, i);

  int msg1;
  int msg2;
  msg1 = receive();
  msg2 = receive();

  printf("1 - bisiesto\n2 - no es bisiesto\n3 - es primo\n4 - no es primo\n\n");
  
  if (msg1 == 1 || msg1== 2) {
    printf("¿es bisiesto?: %d\n", msg1);
    printf("¿es primo?: %d\n", msg2);
  } else {
    printf("¿es bisiesto?: %d\n", msg2);
    printf("¿es primo?: %d\n", msg1);
  }
}

void proc2(pid32 pid1) { 
  //bisiesto
  int i = receive();
  int msg;
  if(i%4 == 0 && i % 100 != 0){
    msg = 1; // es bisiesto
  } else {
    msg = 2; // no es bisiesto
  }
  send(pid1, msg);
  exit();
}

void proc3(pid32 pid1) {
  //primos
  int n = receive();
  int msg = 3; // es primo
  if(n == 0 || n == 1) {
    msg = 4; // no es primo
  }
  for(int i = 2; i < n; i++) {
    if(n % i == 0) {
      msg = 4; // no es primo
    }
  }
  sleepms(100);
  send(pid1, msg);
  exit();
}
