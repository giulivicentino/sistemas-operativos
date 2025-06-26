#include <xinu.h>

sid32 sem_print;


int recolector(int numRobot, int retardo,int pid_analizador){
  int muestra;
  int msg ;
  int i;
  while(1){
    for(i=0;i<numRobot*10;i++){
     muestra = numRobot * i;
     printf("MANDO MUESTRA del robot %d : %d",numRobot,muestra);
     msg=(numRobot<<16)|muestra;
     send(pid_analizador,msg);
     sleepms(retardo);
    }
  }
}

int analizador(int pid_Lab){
  int cantMuestras=0;
  int muestra;
  int robotAnalizando;
  int msg;
  while(cantMuestras<21){
    msg = receive(); //recive numRobot-muestra
    robotAnalizando= (msg>>16);
    muestra = (int)(msg& 0xFFFF);

    wait(sem_print);
    cantMuestras++;
    printf("ANALIZO MUESTRA del robot %d : %d , cantAnalizada: %d",robotAnalizando,muestra,cantMuestras);
    signal(sem_print);

    if(cantMuestras == 20){
      send(pid_Lab,cantMuestras);
    }
  }
}

int laboratorio(){

  //crea el semaforo de la terminal
  sem_print = semcreate(1);

  int pid_Lab = getpid();
  int pid_analizador = create(analizador, 8192, 20, "a1", 1,pid_Lab);
    int pid_robot1 = create(recolector, 8129, 20, "rob1", 3, 1, 600, pid_analizador);
    int pid_robot2 = create(recolector, 8129, 20, "rob2", 3, 2, 500, pid_analizador);
    int pid_robot3 = create(recolector, 8129, 20, "rob3", 3, 3, 888,pid_analizador);

    resume(pid_analizador);
    resume(pid_robot1);
    resume(pid_robot2);
    resume(pid_robot3);

    int cant;
    cant = receive();
    if(cant==20){
      kill(pid_analizador);
      kill(pid_robot1);
      kill(pid_robot2);
      kill(pid_robot3);
      printf("El laboratorio termino luego de analizar 20 muestras");
    }
    
    return 0;





}
