#include <xinu.h>

sid32 sema;
int key;
void mutex_init();
void mutex_lock();
void mutex_unlock();


void mutex_init() {
  key = 0;
  sema = semcreate(1);
}

void mutex_lock() {
  wait(sema);
  key = getpid();
}

void mutex_unlock() {
  if (key == getpid()) {
    key = 0;
    signal(sema);
  }
}

//variables GLOBALES
//sid32 sem_print;
int sintonia;


const char * mensajes[] = {
  " Mensaje de la primer radio \n",
  " Segunda radio transmitiendo \n",
  " Tercera emisora enviando \n",
};

int emisora(int numEmisora, int retardo, int pid_radio){ //nesesito el pid_radio para poder mandarle el mensaje a la radio
  //cuento que tan largos son los mensajes
  int i;
  int largoMsj;
  while(mensajes[numEmisora-1][i]!=0){ //i es el iterador en el msj 
    largoMsj++;
    i++;
  }

  int msg; //lo que manda despues a la radio
  while(1){
    for(i=0; i<largoMsj; i++){
      msg = (numEmisora<<8)| mensajes[numEmisora-1][i]; //numero emisora + letra del msj
      // printf("\n\r emisora: %d antes de mandar %d \n\r",numEmisora, msg);
      send(pid_radio,msg); //le envia numero emisora + letra del msj
      // printf("\n\r emisora: %d desdpues de mandar %d \n\r",numEmisora, msg);
      sleepms(retardo);
    }
  }
}

int radio_play(){ //emula la radio y recibe los msj de las radios
  int msg;
  int numEmisora;
  char c;
    
  while(1){ //queda infinito por que siempre se repiten los msj
  
    msg = receive();
    //decodificar el mensaje que le llega
    numEmisora = (msg>>8);
    c = (char)(msg & 0xFF);
    // printf("%lea",letra);
    
    if(sintonia == numEmisora){
      mutex_lock();//wait(sem_print); // se queda esperando aca si es que no tiene el permiso
      printf("%c",c); //imprime la letra solo si es la sintonia que le toca
      mutex_unlock();//signal(sem_print);
    }
  }
}

int mutexCasero(){

  //consola
  control(CONSOLE, TC_MODER,0,0);

  //crea el semaforo de la terminal
  // sem_print = semcreate(1);
  mutex_init();
    sintonia=1;
    
    int pid_play = create(radio_play, 8192, 20, "play", 0);
    int pid_emi1 = create(emisora, 8129, 20, "emi1", 3, 1, 600, pid_play);
    int pid_emi2 = create(emisora, 8129, 20, "emi2", 3, 2, 500, pid_play);
    int pid_emi3 = create(emisora, 8129, 20, "emi3", 3, 3, 888, pid_play);

    resume(pid_play);
    resume(pid_emi1);
    resume(pid_emi2);
    resume(pid_emi3);
       
    char c; 
    while(1) {
      c = getchar();
      if (c == 'q') {
	kill(pid_play);
	kill(pid_emi1);
	kill(pid_emi2);
	kill(pid_emi3);
	break;
      } else if (c == '1') {
	sintonia = 1;
	mutex_lock();//wait(sem_print);
	printf("\n\r La emisora es %d \n\r", sintonia);
	mutex_unlock();//signal(sem_print);

      } else if (c == '2') {
	sintonia = 2;
	mutex_lock();//wait(sem_print);
	printf("\n\r La emisora es %d \n\r", sintonia);
	mutex_unlock();//signal(sem_print);

      } else if (c == '3') {
	sintonia = 3;
	mutex_lock();//wait(sem_print);
	printf("\n\r La emisora es %d \n\r", sintonia);
	mutex_unlock();//signal(sem_print);
      }

    }
    return 0;
    control(CONSOLE, TC_MODEC, 0, 0);

}
