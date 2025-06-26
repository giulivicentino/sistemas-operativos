#include <xinu.h>

void proceso2(pid32);
void proceso3(pid32);

void parcial2024(){ //programa principal
  //creo los procesos
  int pid1= getpid();
  int pid2 = create(proceso2,1024,20,"proc2",1,pid1); //le mando por por parametro el pid1
  int pid3 = create(proceso3,1024,20,"proc3",1,pid1);

  //numero que pido
  int i;
  char buf[10];
  printf("INGRESE UN NRO: ");
  read(CONSOLE, buf, 10);
  i = atoi(buf); //pasa de char a int
  printf("el numero ingresado entero es %d \n",i);

  //inicia el proceso 2
  resume(pid2);
  send(pid2,i); // le mando el numero que ingresaron al proceso 2
  int msg2 = receive();

  //incia el proceso 3
  resume(pid3);
  send(pid3,i); // le mando el numero que ingresaron al proceso 3
  int msg3 = receive();

  if(msg2 == 1){
    printf("El numero SI es primo y "); 
  }else{ //si devolvio 2
    printf("El numero NO es primo y "); 
  }

  if(msg3 == 3){
    printf("el numero SI es bisiesto \n"); 
  }else{// si devolvio 3
    printf("el numero NO es bisiesto \n"); 
  }
  
  exit(); 
}

//proceso 2 verifica numero PRIMO
void proceso2(pid32 pid1){
  int n = receive(); //n es el i que mande en el proceso principal
  int msg = 1; //es primo

  if(n == 0 || n==1){
    msg=2; //no es primo
  }else{
    for(int i = 2 ; i<n ; i++ ){
      if(n % i == 0 ){
	msg = 2; // no es primo
      }
    }
  }
  send(pid1, msg); // le mando la respuesta al proceso principal
  exit();
}

//proceso 3 veridica si es BISIESTO

void proceso3(pid32 pid1){
  int i = receive();
  int msg=4; // no es bisiesto

  if(i%4 ==0 && i%10 !=00){
    msg=3; // es bisiesto
  }
  send(pid1, msg); // le mando la respuesta al proceso principal
  exit();
}
