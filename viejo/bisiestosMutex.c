/* bisiestos.c en XINU */

#include <xinu.h>
#include <mutex.h> //el mutex esta implementado en el include

#define ASCII_ESC 27

long int calculo[2];
sid32 mutex;
//estados de los procesos
int aSuspendido, bSuspendido, aVivo, bVivo;

void bisiesto(int, int, int);
void muestra_bisiestos();

/* programa principal */

void bisiestos() {
    pid32 pid1 = create(bisiesto, 4096, 20, "Proceso A", 3, 1, 2000, 0);
    pid32 pid2 = create(bisiesto, 4096, 20, "Proceso B", 3, 2001, 4000, 1);
    pid32 pidMuestra = create(muestra_bisiestos, 4096, 20, "Proceso Muesta", 0);
    
    //inicializamos todo para que quede correctamente
    mutex = mutex_init();
    aSuspendido = 0;
    bSuspendido = 0;
    aVivo = 1;
    bVivo = 1;
        
    //inicializar procesos hijos
    resume(pidMuestra);
    resume(pid1);
    resume(pid2);
    
    //vamos a gestionar la entrada del usuario   
    
    char opcion;
    //imput raw
    control(CONSOLE, TC_MODER, 0, 0);
    
	while(1) {
		/* gestionar las pulsaciones del teclado, procesar, informar al usuario */
        opcion = getchar();
        
        switch(opcion) {
            case '1':
                mutex_lock(mutex);
                if(aVivo) {
                    if(aSuspendido) {
                        aSuspendido = 0;
                        resume(pid1);
                    } else {
                        aSuspendido = 1;
                        //suspend(pid1); esto no es necesario xq se suspende solo
                    }
                }
                mutex_unlock(mutex);
                break;
            case '3':
                mutex_lock(mutex);
                if(aVivo) {
                    aVivo = 0;
                    kill(pid1);
                }
                mutex_unlock(mutex);
                break;
            case '7':
                mutex_lock(mutex);
                if(bVivo) {
                    if(bSuspendido) {
                        bSuspendido = 0;
                        resume(pid2);
                    } else {
                        bSuspendido = 1;
                        //suspend(pid2); esto no es necesario xq se suspende solo
                    }
                }
                mutex_unlock(mutex);
                break;
            case '9':
                mutex_lock(mutex);
                if(bVivo) {
                    bVivo = 0;
                    kill(pid2);
                }
                mutex_unlock(mutex);
                break;
            case 'q':
                //aqui no es necesario el mutex pues finalizaremos el porgrama
                kill(pidMuestra);
                if(aVivo) {
                    kill(pid1);
                }
                if(bVivo) {
                    kill(pid2);
                }
                
                //volvemos a input cooked
                control(CONSOLE, TC_MODEC, 0, 0);
                exit();
            default:
                break;
        }
		sleepms(5);
	}
}

/* completar el codigo del programa bisiesto */
void bisiesto(int base, int limite, int posArreglo) {
	int i;

	/* base, limite y posArreglo vienen como argumentos */
	for (i=base; i<=limite; i++) {
		if( (i%4 == 0) && ((i%100 != 0) || (i%400 == 0)) ) {
			calculo[posArreglo] = i;
			sleepms(300);
			
			mutex_lock(mutex);
			if((posArreglo == 0 && aSuspendido) || (posArreglo == 1 && bSuspendido)) {
			    mutex_unlock(mutex);
			    suspend(getpid());
			} else {
			    mutex_unlock(mutex);
			}
		}
	}

}

void muestra_bisiestos() {
	/* limpiar toda la pantalla */
	printf( "%c[2J", ASCII_ESC );

	while(1) {
		/* Nos posicionamos en la fila 5 columna 10 */
    		printf( "%c[5;10f", ASCII_ESC );
    		printf( "Proceso A: %d \n", calculo[0]);

		/* Nos posicionamos en la fila 6 columna 10 */
    		printf( "%c[6;10f", ASCII_ESC );
    		printf( "Proceso B: %d \n", calculo[1]);
        
        /* Nos posicionamos en la fila 7 columna 10 */
    		printf( "%c[7;10f", ASCII_ESC );
    		mutex_lock(mutex);
    		if(aVivo) {
    		    if(aSuspendido) {
        		    printf( "Proceso A suspendido      \n" );
        		} else {
        		    printf( "Proceso A ejecutandose    \n" );
        		}
    		} else {
    		    printf( "Proceso A muerto       \n" );
    		}
    		mutex_unlock(mutex);
    		
    	/* Nos posicionamos en la fila 8 columna 10 */
    		printf( "%c[8;10f", ASCII_ESC );
    		mutex_lock(mutex);
    		if(bVivo) {
    		    if(bSuspendido) {
        		    printf( "Proceso B suspendido     \n" );
        		} else {
        		    printf( "Proceso B ejecutandose     \n" );
        		}
    		} else {
    		    printf( "Proceso B muerto      \n" );
    		}
    		mutex_unlock(mutex);
    	
    	/* Nos posicionamos en la fila 9 columna 10 */
    	printf( "%c[9;10f", ASCII_ESC );
    	printf( "Ingresos: 1- suspender/resumir A \n");
    	/* Nos posicionamos en la fila 10 columna 10 */
    	printf( "%c[10;10f", ASCII_ESC );
    	printf( "          3- matar A \n");
    	/* Nos posicionamos en la fila 11 columna 10 */
    	printf( "%c[11;10f", ASCII_ESC );
    	printf( "          7- suspender/resumir B \n");
    	/* Nos posicionamos en la fila 12 columna 10 */
    	printf( "%c[12;10f", ASCII_ESC );
    	printf( "          9- matar B \n");

		sleepms(50);
	}
}


