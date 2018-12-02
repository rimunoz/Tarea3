#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "Tarea3.h"


//MACRO
#define IMPRIMIR 1
#define VERBOSE  2



int main(int argc, char *argv[] )
{
//change seed
srand((unsigned int)time(NULL));
//execution time
clock_t start, end;
double total_time;
start = clock();

//Declare structures to use
Particle  P;
Container C = {0,0,0,0};

leerArgumentosEntrada(&C,  argc ,  argv);
printContainer(C); //no necesario

//Inicializar Particulas
P = generarParticulas(C);

printParticula(P,C.p); //no necesario
checkDuplicates(C, P); //no necesario

imprimirContainer(C,P,0);

for(int s = 0; s < C.s ; s++){
	Particle P_next = obtenerDireccion(C, P);


	if(VERBOSE > 1){
		printf("\nPARTICULA obtenerDireccion\n");
		printParticula(P,C.p); //no necesario
		printf("\nPARTICULA NEXT\n");
		printParticula(P_next,C.p);
	}

	moverParticulas(C,P,P_next);

	if(VERBOSE > 1){
		printf("\nPARTICULA moverParticulas\n");
		printParticula(P,C.p); //no necesario
	}
	
	comprobarColisiones(C,P);
	if(VERBOSE > 1){
		printf("\nPARTICULA comprobarColisiones\n");
		printParticula(P,C.p); //no necesario
	}

	imprimirContainer(C,P, s+1);
	desbloquearParticulas(C,P);
	printf("Fin Iteracion \n-----------------------\n");

}

end = clock();
total_time = ((double) (end - start)) / CLOCKS_PER_SEC;
printf("Total time: %f \n", total_time);
  return 0;
} //end main

void desbloquearParticulas(struct container C, struct particle P){
	for(int i = 0; i< C.p; i++){
		//desbloquear particulas
		if(P.s[i] == 'b'){
			if(VERBOSE > 0){ printf("Particula %02d desbloquada\n",i+1); }

			
			P.s[i] = 'm';
		}
	}
}// end function


void comprobarColisiones(struct container C, struct particle P){
	 for(int i = 0; i < C.p; i++){
		for(int j = 0; j < C.p; j++){
			//cambio de estado por particula precipitando encima
			if((P.s[i] == 'm' || P.s[i] == 'b') && P.s[j] == 'p'){
				if(P.x[i] == P.x[j]  && P.y[i]  == P.y[j] + 1){
					if(VERBOSE > 0){
					printf("Cambio de estado por particula precipitando encima %02d y %02d = (%02d,%02d) (%02d,%02d) \n",i +1, j+1, P.x[i], P.y[i], P.x[j],P.y[j]);}
					P.s[i] = 'p';
				}
			}
			//cambio a precipitando por cercania
			if((P.x[i] == P.x[j] -1 && P.y[i] == P.y[j]) ||(P.x[i] == P.x[j] + 1 &&  P.y[i]==P.y[j])){
				if((P.s[i] == 'm' || P.s[i] == 'b')){
					if(VERBOSE > 0){
					printf("Cambio de estado a precipitando por cercania %02d y %02d = (%02d,%02d) (%02d,%02d) \n",i +1, j+1, P.x[i], P.y[i], P.x[j],P.y[j]);}
					P.s[i] = 'p';
				}
				if((P.s[j] == 'm' || P.s[j] == 'b')){
					if(VERBOSE > 0){
					printf("Cambio de estado a precipitando por cercania %02d y %02d = (%02d,%02d) (%02d,%02d) \n",i +1, j+1, P.x[i], P.y[i], P.x[j],P.y[j]);}
					P.s[j] = 'p';
				}
			}
			//particulas precipitando a Precipitado
			if(P.s[i] == 'p' && P.x[i] == P.x[j] && P.y[i] + 1 == P.y[j] && P.s[j] == 'D' ){
				if(VERBOSE > 0){printf("Cambio de estado a D por caer encima de particula D %02d y %02d = (%02d,%02d) (%02d,%02d) \n",i +1, j+1, P.x[i], P.y[i], P.x[j],P.y[j]);}
				P.s[i] = 'D';
			}

		}//end for
		//particulas precipitando a Precipitado
		if(P.s[i] == 'p' && P.y[i]== C.h-1){
				if(VERBOSE > 0){printf("Cambio de estado a precipitado por tocar fondo - Particula: %02d (%02d,%02d)\n", i+1, P.x[i], P.y[i]);}
				P.s[i] = 'D';
			}

	} //end for



}// end function




void moverParticulas(struct container C, struct particle P, struct particle P_next){
	//actualizar direcciones
	for(int i = 0; i< C.p; i++){
		//particulas bloqueadas
		if(P.s[i] == 'b'){
			P.x[i] = P_next.x[i];
			P.y[i] = P_next.y[i];
		}
		//particulas precipitando
		if(P.s[i] == 'p'){
			P.x[i] = P_next.x[i];
			P.y[i] = P_next.y[i];
		}
		//particulas precipitadas
		if(P.s[i] == 'D'){
			P.x[i] = P_next.x[i];
			P.y[i] = P_next.y[i];
		}
		//particulas en movimiento
		if(P.s[i] == 'm'){
			P.x[i] = P_next.x[i];
			P.y[i] = P_next.y[i];
		}
		//particulas precipitando a precipitadas por llegar al fondo
		if(P.s[i] == 'p' && P.y[i] == C.h -1){
			if(VERBOSE > 0){printf("Cambio de estado a precipitado - Particula: %02d (%02d,%02d)\n", i+1, P.x[i], P.y[i]);}
			P.s[i] = 'D';
		}
		//Condicion de precipitando a precitadas por particula encima se mueve a la siguiente funcion para no replicar el doble for
	 }	

}// end function


struct particle obtenerDireccion(struct container C, struct particle P){
	struct particle P_next;

	P_next.x = malloc(C.p*sizeof(int));
	P_next.y = malloc(C.p*sizeof(int));
	P_next.s = malloc(C.p*sizeof(char));
	P_next.d = malloc(C.p*sizeof(int));

	//update address
	
	for(int i = 0; i< C.p; i++){
		int d = rand() % 4;
		P.d[i] = d;
		P_next.x[i] = P.x[i];
		P_next.y[i] = P.y[i];
		if(P.s[i] == 'b' || P.s[i]=='m'){
			switch(d){
			case 0:
				P_next.x[i] = P.x[i] - 1 ;
				break;
			case 1:
				P_next.x[i] = P.x[i] + 1;
				break;
			case 2:
				P_next.y[i] = P.y[i] - 1 ;
				break;
			case 3:
				P_next.y[i] = P.y[i] + 1 ;
				break;
		}}
		if(P.s[i] == 'p'){
			P_next.y[i] = P.y[i] + 1;
		}
		if(P.s[i] == 'D'){
			P_next.x[i] = P.x[i];
			P_next.y[i] = P.y[i];
		}
		

	}
	//Comprobar estados
	//bloqueos por salida del container
	for(int i = 0; i< C.p; i++){
		if(P_next.x[i] == -1 || P_next.x[i] == C.w || P_next.y[i] == -1 || P_next.y[i] == C.h ){
			if(VERBOSE > 0){printf("bloqueo por salida de container %d \n",i+1);}
			P.s[i] = 'b';
			P_next.x[i] = P.x[i];
			P_next.y[i] = P.y[i];
		}
	 }
	 //bloqueos por posible uso de la misma posicion
	 for(int i = 0; i < C.p; i++){
		for(int j = 0; j < i; j++){
			if(P_next.x[i] == P_next.x[j] && P_next.y[i] == P_next.y[j]){
				if(VERBOSE > 0){
				printf("bloqueo por misma posicion %02d y %02d = (%02d,%02d) (%02d,%02d) \n",i +1, j+1, P_next.x[i], P_next.y[i], P_next.x[j],P_next.y[j]);
				}
				if(P.s[i]=='m'){
					P.s[i] = 'b';
					P_next.x[i] = P.x[i];
					P_next.y[i] = P.y[i];
				}
				if(P.s[j]=='m'){
					P.s[j] = 'b';
					P_next.x[j] = P.x[j];
					P_next.y[j] = P.y[j];
				}
			}
		}
	}
	return P_next;


}//end function



void leerArgumentosEntrada(struct container *C, int argc , char *argv[] ){
	//Declare parameters
	int w, h, p, s;

	//CHECK ARGUMENTS
		if( argc == 5 ){
	      printf("argumentos %s %s %s %s \n\n" , argv[1] , argv[2] , argv[3], argv[4]);
	      w = atoi(argv[1]);
	      h = atoi(argv[2]);
	      p = atoi(argv[3]);
	      s = atoi(argv[4]);
	      printf("ancho: %d \n alto: %d \n #particulas: %d \n #pasos: %d \n\n", w,h,p,s);

	      //Init Container
			C->w=w;
			C->h=h;
			C->p=p;
			C->s=s;

	   } 
	   else{
	   	fprintf(stderr, "ParameterError: Se esperan 4 argumentos (w t h p): \n\tw: ancho del intervalo\n\th: alto del intervalo\n\tp: num de particulas \n\ts: num de pasos \n");
	   	exit(0);
	   }

}//end function


struct particle  generarParticulas(struct container C){
	struct particle P;

	P.x = malloc(C.p*sizeof(int));
	P.y = malloc(C.p*sizeof(int));
	P.s = malloc(C.p*sizeof(char));
	P.d = malloc(C.p*sizeof(int));

	int x, y, d;
	for(int i = 0; i< C.p; i++){
		x = rand() % C.w;
		y = rand() % C.h;
		d = rand() % 4;
		//printf("Particula %d : x,y,d = (%d,%d,%d) \n",i, x, y,d);
		P.x[i] = x;
		P.y[i] = y;
		P.s[i] = 'm';
		P.d[i] = d;
			}
	while(moveDuplicates(C, P) > 0){
		printf("duplicados\n");
	}
	return P;
}//end function


int moveDuplicates(struct container C, struct particle P){
	int dup = 0;
	printf("--------------------------------------\n");
	for(int i = 0; i < C.p; i++){
		for(int j = 0; j < i; j++){
			if(P.x[i] == P.x[j] && P.y[i] == P.y[j]){
				printf("Duplicados %02d y %02d = (%02d,%02d) (%02d,%02d) \n",i +1, j+1, P.x[i], P.y[i], P.x[j],P.y[j]);
				P.x[j] = rand() % C.w;
				P.y[j] = rand() % C.h;
				dup++;
			}
		}
	}
	printf("--------------------------------------\n");
	return dup;
}//end function


void checkDuplicates(struct container C, struct particle P){
	int dup = 0;
	printf("--------------------------------------\n");
	for(int i = 0; i < C.p; i++){
		for(int j = 0; j < i; j++){
			if(P.x[i] == P.x[j] && P.y[i] == P.y[j]){
				printf("Duplicados %02d y %02d = (%02d,%02d) (%02d,%02d) \n",i +1, j+1, P.x[i], P.y[i], P.x[j],P.y[j]);
				dup++;
			}
		}
	}
	printf("--------------------------------------\n");
	printf("DUPLICADOS: %d", dup);
}//end function


void printContainer(struct container C){
	printf("Container: \n\tancho: %d \n\tlargo: %d \n\t#particulas: %d \n\t#pasos:%d \n\n", C.w, C.h, C.p, C.s);
} //end function



void printParticula(struct particle P, int size){
 	//int size = sizeof(P.x)/sizeof(P.x[0]);

	for(int i = 0; i < size; i++){
	printf("Particula %02d de %02d, (x,y,s,d) = (%d,%d,<%c>,%d) \n", i+1 ,size ,P.x[i] , P.y[i], P.s[i], P.d[i]);
	}
	printf("\n");
}//end function



void imprimirContainer(struct container C, struct particle P, int iter){
	
	if(IMPRIMIR != 0){
		printf("\n");
	 	for(int j = -1; j < C.h + 1; j++){
	 		printf("%03d ",j);
	 		for(int i = -1; i < C.w + 1; i++){
	 			char c = getElement(i,j,P,C);
	 			printf("%c",c);
	 		}
	 		printf("\n");
	 	}
	}
 	if(IMPRIMIR == 0){
	   FILE *fp;
	   if(iter == 0){ 	fp = fopen("salida.txt", "w");   }
	   if(iter > 0) { 	fp = fopen("salida.txt", "a");   }
	   
	   fp = fopen("salida.txt", "a");
		fprintf(fp,"\n");
	 	for(int j = -1; j < C.h + 1; j++){
	 		fprintf(fp,"%03d ",j);
	 		for(int i = -1; i < C.w + 1; i++){
	 			char c = getElement(i,j,P,C);
	 			fprintf(fp,"%c",c);
	 		}
	 		fprintf(fp,"\n");
	 		
	 	}
	 	fprintf(fp,"\n");
	 	fclose(fp);
 	}

} //end function


char getElement(int i, int j, struct particle P, struct container C){

	char c = ' ';
	//Border
	if(i == -1  && j > -1 && j < C.h ){  c = '|'; return c;}
	if(i == C.w && j > -1  && j < C.h){ c = '|'; return c;}
	if(j == -1  ){ c = '_'; return c;}
	if(j == C.h ){ c = '-'; return c;}


	//Check Particles
	for(int k = 0; k < C.p; k++){
		if(P.x[k]==i && P.y[k]==j){ c= P.s[k]; return c;}		
	}
	//return empty cell
	return c;
} //end function




