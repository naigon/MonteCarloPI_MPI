//NAIGON MEDEIROS MARTINS
//CALCULO DE PI PELO METODO DE MONTE CARLO USANDO THREADS (MPI)

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define N_PONTOS 1000000 //numero de pontos utilizados no calculo da aproximacao
#define PI 3.141592653589793 //valor de pi ate a 15a casa depois da virgula, apenas para fins de comparacao com o valor obtido

double gera_coord(); //funcao que gera numeros aleatorios

int main(int argc, char* argv[]){
	
	printf("################### MONTE CARLO PI ###################\n\n");
	
	int myid; 	//id do processo/thread
	int reducedpdentro;  //total de pontos dentro(variavel usada na reducao)
	int reducednptos;     //numero total de prontos(variavel usada na reducao)
   	int i=0;
	double x=0,y=0;
	int pdentro=0; //contador de pontos dentro
	double valor_pi=0,erro=0;
   
	MPI_Init(&argc, &argv);                 //inicia MPI
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);   //pega o rank de cada processo
	
	if(myid != 0){ //para processos nao raiz
	
	for(i=0;i<N_PONTOS;i++){
		x=gera_coord(); //gera coordenada x do ponto
		y=gera_coord(); //gera coordenada y do ponto
		//printf("P(%d) x=%.15f  y=%.15f \n",i,x,y); //print auxiliar para verificar gera��o das coordenadas
		if((x*x + y*y) <= 1) //soma os quadrados das coordenadas
		{	++pdentro; //se a soma dos quadrados for menor ou igual a 1, ponto caiu dentro
		}
	}
}
	//reducoes das variaveis de contagem de pontos e numero de pontos
	MPI_Reduce(&pdentro, &reducedpdentro, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(&N_PONTOS, &reducednptos, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    reducednptos -= N_PONTOS;  //compensa os loops fora do processo mestre
 
    if (myid == 0)    //para processo raiz  
    {
        valor_pi = 4.0*((double)reducedpdentro/(double)reducednptos);
		erro = valor_pi - PI; //calcula o erro(diferenca em relacao ao valor ideal de PI)
        printf("\n");
		printf("[NUMERO TOTAL DE PONTOS] = %d \n\n",reducednptos);
		printf("[PONTOS DENTRO DO CIRCULO] = %d \n\n",reducedpdentro);
		printf("[PONTOS FORA DO CIRCULO] = %d \n\n",(reducednptos-reducedpdentro));
		printf("[VALOR APROXIMADO DE PI] = %.15f \n\n",valor_pi);
		printf("[VALOR IDEAL DE PI] = %.15f \n\n",PI);
		printf("[DIFEREN�A EM RELACAO AO PI IDEAL] = %.15f \n\n",erro);
       
    }
 
    MPI_Finalize(); //termina instancia do MPI

return 0;
}

/*	- Corpo da funcao geradora de numeros aleatorios,
ela gera um double aletorio, e retorna so a parte
apos a virgula, pois o objetivo eh obter um valor real 
entre 0 e 1 nas coordenadas do ponto */

double gera_coord(){
	double aux=100.0*((double)(rand())/RAND_MAX);
	aux=aux - (int)(aux);
	return aux;
}
