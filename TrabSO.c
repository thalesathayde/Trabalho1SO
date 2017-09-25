#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <math.h>

// semaforo de clientes
sem_t clienteP;
sem_t clienteB;
sem_t clienteC;

// semaforo dos barbeiros
sem_t barbeiroP;
sem_t barbeiroB;
sem_t barbeiroC;
// semaforo do mutex 
sem_t mutex;

//numero de cadeiras = 10
int clientesEsperando = 0;

// thread dos 3 barbeiros
void* barbeiroPinta(void *coisa); 
void* barbeiroBarbeia(void *coisa);
void* barbeiroCorta(void *coisa);
//thread do cliente
void* clientePinta(void* coisa);
void* clienteBarba(void* coisa);
void* clienteCorte(void* coisa);
// coisas que barbeiros fazem
void executando(int opcao);
void clienteChegou(int opcao, int id);
void recebendo_Servico(int opcao, int id);
void desistiu(int opcao, int id);

int idCliente = 1;


int main(){

	sem_init(&clienteP, 1, 0);
	sem_init(&clienteB, 1, 0);
	sem_init(&clienteC, 1, 0);

	sem_init(&barbeiroP, 1, 0);
	sem_init(&barbeiroB, 1, 0);
	sem_init(&barbeiroC, 1, 0);

	sem_init(&mutex, 1, 1);

	pthread_t b1, b2, b3, c; //tem q confirmar se vai ser uma thread pra todos barbeiros e uma pra todos clientes, ou se vai ter q ter uma thread pra cada tipo de barbeiro e cada tipo de cliente

 	// criando os barbeiros
	pthread_create(&b1, NULL, (void *) barbeiroPinta, NULL);
	pthread_create(&b2, NULL, (void *) barbeiroBarbeia, NULL);
	pthread_create(&b3, NULL, (void *) barbeiroCorta, NULL);


	//criando clientes
	while(1) {
		switch(rand()%3){
			case 0:
				pthread_create(&c, NULL, (void *) clientePinta, NULL);
				sleep(1);
				break;
			case 1:
				pthread_create(&c, NULL, (void *) clienteBarba, NULL);
				sleep(1);
				break;
			case 2:
				pthread_create(&c, NULL, (void *) clienteCorte, NULL);
				sleep(1);
				break;
			default:
				printf("Deu erro no valor randomico \n");
		}
	}


}


void* barbeiroPinta(void *coisa) {
	while(1) {
		sem_wait(&clienteP); 
		sem_wait(&mutex);     
		clientesEsperando--;  
		sem_post(&barbeiroP);     
		sem_post(&mutex);       
		executando(0);             
	}

	pthread_exit(NULL);
}
void* barbeiroBarbeia(void *coisa) { 
	while(1) {
		sem_wait(&clienteB); 
		sem_wait(&mutex);     
		clientesEsperando--;  
		sem_post(&barbeiroB);     
		sem_post(&mutex);       
		executando(1);             
	}

	pthread_exit(NULL);
}
void* barbeiroCorta(void *coisa) { 
	while(1) {
		sem_wait(&clienteC); 
		sem_wait(&mutex);     
		clientesEsperando--;  
		sem_post(&barbeiroC);     
		sem_post(&mutex);       
		executando(2);             
	}

	pthread_exit(NULL);
}




void* clientePinta(void* coisa){
	sem_wait(&mutex);
	
	int id = idCliente++;	
	
	if(clientesEsperando<10){
		clienteChegou(0,id);
		clientesEsperando++;
		sem_post(&clienteP);
		sem_post(&mutex);
		sem_wait(&barbeiroP);
		recebendo_Servico(0,id);
	}else{
		sem_post(&mutex);
		desistiu(0,id); 

	}
	pthread_exit(NULL);

}
void* clienteBarba(void* coisa){ 
	sem_wait(&mutex);

	int id = idCliente++;

	if(clientesEsperando<10){
		clienteChegou(1,id);
		clientesEsperando++;
		sem_post(&clienteB);
		sem_post(&mutex);
		sem_wait(&barbeiroB);
		recebendo_Servico(1,id);
	}else{
		sem_post(&mutex);
		desistiu(1,id); 

	}
	pthread_exit(NULL);

}
void* clienteCorte(void* coisa){ 
	sem_wait(&mutex);

	int id = idCliente++;

	if(clientesEsperando<10){
		clienteChegou(2,id);
		clientesEsperando++;
		sem_post(&clienteC);
		sem_post(&mutex);
		sem_wait(&barbeiroC);
		recebendo_Servico(2,id);
	}else{
		sem_post(&mutex);
		desistiu(2,id); 

	}
	pthread_exit(NULL);

}


void executando(int opcao) {
	//pinta=0, barba=1, corte=2
	switch(opcao){
		case 0:
			printf("Barbeiro estah pintando o cabelo de alguem!\n");
			break;
		case 1:
			printf("Barbeiro estah fazendo a barba de alguem!\n");
			break;
		case 2:
			printf("Barbeiro estah cortando o cabelo de alguem!\n");
			break;
		default:
			printf("What?\n");
	}
	sleep(10);
}


void clienteChegou(int opcao, int id){
	//pinta=0, barba=1, corte=2
	switch(opcao){
		case 0:
			printf("O Cliente %d chegou para pintar o cabelo \n", id);
			break;
		case 1:
			printf("O Cliente %d chegou para fazer a barba \n", id);
			break;
		case 2:
			printf("O Cliente %d chegou para cortar o cabelo \n", id);
			break;
		default:
			printf("What?\n");
	}
}

void recebendo_Servico(int opcao, int id){
	//pinta=0, barba=1, corte=2
	switch(opcao){
		case 0:
			printf("O Cliente %d estah tendo o cabelo pintado \n", id);
			break;
		case 1:
			printf("O Cliente %d estah tendo a barba feita \n", id);
			break;
		case 2:
			printf("O Cliente %d estah tendo o cabelo cortado \n", id);
			break;
		default:
			printf("What?\n");
	}
}

void desistiu(int opcao, int id){
	//pinta=0, barba=1, corte=2
	switch(opcao){
		case 0:
			printf("O Cliente %d desistiu de pintar o cabelo \n", id);
			break;
		case 1:
			printf("O Cliente %d desistiu de fazer a barba \n", id);
			break;
		case 2:
			printf("O Cliente %d desistiu de cortar o cabelo \n", id);
			break;
		default:
			printf("What?\n");
	}
}
