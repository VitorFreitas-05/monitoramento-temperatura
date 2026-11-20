/*
- Projeto: Sistema de monitoramento de temperatura

- Descrição: Simula a leitura de sensores utilizando processos, comunicação entre processos (pipe),
threads, mutex e semáforo.

- Funcionamento: O processo filho gera temperaturas aleatórias e as envia ao processo pai por meio
de um pipe.O processo pai cria threads para analisar as temperaturas e contabiliza os alertas.
*/


#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>

//Contabiliza a quantidade total de alertas identificados.
int contadorAlertas = 0;

//Proteger o acesso simultâneo ao contador de alertas
pthread_mutex_t trava = PTHREAD_MUTEX_INITIALIZER;

//Limita a quantidade de threads executando ao mesmo tempo
sem_t vagas;

//Armazenar os dados de cada sensor
typedef struct{
    int numeroSensor;
    float temperatura;
}DadosSensor;

//Analisa as temperaturas e executa as threads
void* verificarTemperatura(void* argumento){
    DadosSensor*dados = (DadosSensor*)argumento;
    int sensor = dados->numeroSensor;
    float temperatura = dados->temperatura;

    sem_wait(&vagas);

    printf("Sensor %d iniciou a analise.\n", sensor);

    if(temperatura > 35.0){
	pthread_mutex_lock(&trava);
	contadorAlertas++;
	pthread_mutex_unlock(&trava);
	printf("ALERTA: Sensor %d com temperatura elevada - %.2f C!\n", sensor, temperatura);
    }else{
	printf("Sensor %d: Temperatura normal - %2.f C\n", sensor, temperatura);
    }

    sleep(1);
    printf("Sensor %d finalizou a analise.\n", sensor);
    sem_post(&vagas);

    return NULL;
}

int main(){
    int fd[2];
    DadosSensor sensores[3];
    int threadsCriadas[3] = {0};

    if(pipe(fd) == -1){
	printf("Erro ao criar pipe!\n");
	return 1;
    }

    pthread_t thread[3];
    pid_t processo = fork(); //Cria novo processo

    //Processo filho que gera as temperaturas aleatórias
    if(processo == 0){
	close(fd[0]);

	srand(time(NULL)^getpid());

	for(int i = 0;i < 3;i++){
	    float temperatura = 20.0 + (rand() % 3000) / 100.0;

	    printf("Filho - Sensor %d: %.2f\n", i+1, temperatura);
	    write(fd[1], &temperatura, sizeof(temperatura));
	}

	printf("\n");

	close(fd[1]);
	exit(0);
    }else if(processo > 0){ //Processo pai que recebe as temperaturas, analisa e executa as threads
	close(fd[1]);
	sem_init(&vagas, 0, 2);

	for(int i = 0;i < 3;i++){
	    float temperaturaRecebida;

	    read(fd[0], &temperaturaRecebida, sizeof(temperaturaRecebida));
	    printf("Pai - Sensor %d: %.2f\n", i+1, temperaturaRecebida);
	    sensores[i].numeroSensor = i + 1;
	    sensores[i].temperatura = temperaturaRecebida;

	    int resultado = pthread_create(&thread[i], NULL, verificarTemperatura, &sensores[i]);

	    if(resultado != 0){
		printf("Erro ao criar thread do sensor %d!\n", i + 1);
	    }else{
		threadsCriadas[i] = 1;
	    }
	}
	for(int i = 0;i < 3;i++){
	    if(threadsCriadas[i]){
		pthread_join(thread[i], NULL);
	    }
	}

	printf("Total de alertas: %d\n", contadorAlertas);
	pthread_mutex_destroy(&trava);
	sem_destroy(&vagas);

	close(fd[0]);
	wait(NULL);
    }else{
	printf("Erro ao criar processo!\n");
    }

    return 0;
}
