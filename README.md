Sistema de Monitoramento de Temperatura

1. Objetivo

O projeto consiste em uma aplicação desenvolvida em linguagem C que simula um sistema de monitoramento de temperatura utilizando conceitos de Sistemas Operacionais, como processos, comunicação entre processos, threads e mecanismos de sincronização.

2. Funcionamento

O sistema simula três sensores que geram valores aleatórios de temperatura.

O processo filho é responsável por gerar as temperaturas e enviá-las ao processo pai através de um pipe.

O processo pai recebe os valores e cria threads para analisar individualmente cada temperatura.

Quando uma temperatura ultrapassa 35 °C, o sistema gera um alerta. Ao final da execução, é apresentado o total de alertas identificados.

3. Conceitos de Sistemas Operacionais utilizados

- fork(): criação de um processo filho.
- pipe(): comunicação entre o processo pai e o processo filho.
- wait(): sincronização do processo pai com o processo filho.
- exit(): encerramento do processo filho.
- pthread_create(): criação das threads responsáveis pela análise.
- pthread_join(): espera pela finalização das threads.
- Mutex: proteção da variável compartilhada que contabiliza os alertas.
- Semáforo: limita a execução simultânea a duas threads.

4. Tecnologias utilizadas

- Linguagem C
- GCC
- Biblioteca POSIX Threads (pthread)
- Semáforos POSIX
- Ambiente Linux / Ubuntu

5. Como executar

Compile o programa:

gcc projeto-monitorar-temperatura.c -o projeto-monitorar-temperatura -pthread

Execute:

./projeto-monitorar-temperatura

6. Resultado esperado

O programa apresenta:

1. Temperaturas geradas pelos sensores.
2. Temperaturas recebidas pelo processo pai.
3. Análise individual realizada pelas threads.
4. Alertas para temperaturas superiores a 35 °C.
5. Total de alertas identificados.

7. Considerações finais

O projeto demonstra a aplicação prática de conceitos de Sistemas Operacionais, permitindo observar a comunicação entre processos, a execução concorrente de threads e o uso de mecanismos de sincronização para controlar o acesso a recursos compartilhados.
