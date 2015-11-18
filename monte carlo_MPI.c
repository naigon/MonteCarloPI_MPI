//NAIGON MEDEIROS MARTINS
//CALCULO DE PI PELO METODO DE MONTE CARLO USANDO THREADS(MPI)

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mpi.h"




#define N_PONTOS 50000 //numero de pontos utilizados no calculo da aproximação
#define PI 3.141592653589793 //valor de pi até a 15a casa depois da virgula, apenas para fins de comparação com o valor obtido

double gera_coord(); //função que gera numeros aleatorios
void calcula_pi(); //função para o calculo do valor de pi

int main(int argc, char* argv[]){
printf("################### MONTE CARLO PI ###################\n\n");


srand(time(NULL)); //inicia gerador de numeros com a semente
clock_t start_time; //variavel para calculo do tempo de execução
start_time = clock();

calcula_pi(); //chamada da função para o calculo de PI

double tempo = (clock() - start_time) / (double)CLOCKS_PER_SEC; //obtem o tempo de execução depois do retorno de calcula_pi()
printf("[TEMPO TOTAL DE EXECUCAO] = %.2f segundos \n\n",tempo); // imprime tempo na tela

return 0;
}

/*	- Corpo da função geradora de numeros aleatorios,
ela gera um double aletório, e retorna só a parte
após a virgula, pois o objetivo é obter um valor real 
entre 0 e 1 nas coordenadas do ponto */

double gera_coord(){
	double aux=100.0*((double)(rand())/RAND_MAX);
	aux=aux - (int)(aux);
	return aux;
}

//corpo da função que calcula o valor de PI
void calcula_pi(){
int myid;
int reducedcount;                   //total number of "good" points from all nodes
int reducedniter;                   //total number of ALL points from all nodes
    MPI_Init(&argc, &argv);                 //Start MPI
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);           //get rank of node's process
 
	int i=0;
	double x=0,y=0;
	int pdentro=0,pfora=0;
	double valor_pi=0,erro=0;
	
if(myid != 0){
	
	for(i=0;i<N_PONTOS;i++){
		x=gera_coord(); //gera coordenada x do ponto
		y=gera_coord(); //gera coordenada y do ponto
		//printf("P(%d) x=%.15f  y=%.15f \n",i,x,y); //print auxiliar para verificar geração das coordenadas
		if((x*x + y*y) <= 1) //soma os quadrados das coordenadas
		{	++pdentro; //se a soma dos quadrados for menor ou igual a 1, ponto caiu dentro
		}
	}
}
	MPI_Reduce(&count, &reducedcount, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(&niter, &reducedniter, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    reducedniter -= N_PONTOS;                  //to compensate for no loop on master node
 
    if (myid == 0)                      //if root process
    {
        valor_pi = ((double)reducedcount/(double))*4.0;
		erro = valor_pi - PI; //calcula o erro(diferença em relação ao valor ideal de PI)              //p = 4(m/n)
        printf("\n");
		printf("[NUMERO TOTAL DE PONTOS] = %d \n\n",reducedniter);
		printf("[PONTOS DENTRO DO CIRCULO] = %d \n\n",reducedcount);
		printf("[PONTOS FORA DO CIRCULO] = %d \n\n",(reducedniter-reducedcount));
		printf("[VALOR APROXIMADO DE PI] = %.15f \n\n",valor_pi);
		printf("[VALOR IDEAL DE PI] = %.15f \n\n",PI);
		printf("[DIFERENÇA EM RELACAO AO PI IDEAL] = %.15f \n\n",erro);
       
 
    }
 
    MPI_Finalize();


}
