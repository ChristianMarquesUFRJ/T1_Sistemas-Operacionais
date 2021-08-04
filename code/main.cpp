////////////////////////////////////////////////////////////////////////////
//  Universidade Federal do Rio de Janeiro
//  Disciplina: Sistemas Operacionais - EEL770
//  Professor: Rodrigo Souza Couto
//  Desenvolvedor: Chritian Marques de Oliveira Silva
//  DRE: 117.214.742
//  Trabalho 1: Inicialização de programas e comunicação entre processos
////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////
//  COMANDOS DE INTERAÇÃO COM O SISTEMA PELO TERMINAL
////////////////////////////////////////////////////////////////////////////
// |-> Chamar a task1:          kill -SIGUSR1 <pid>
// |-> Chamar a task2:          kill -SIGUSR2 <pid>
// |-> Finalizar a execução:    kill -SIGTERM <pid>

////////////////////////////////////////////////////////////////////////////
//  IMPORTAÇÃO DE BIBLIOTECAS E CONFIGURAÇÕES DE AMBIENTE
////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <ctime>
#include <string>
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <csignal>
#include <sys/types.h>
#include <sys/wait.h>
using namespace std; 

////////////////////////////////////////////////////////////////////////////
//  VARIAVEIS/DEFINIÇÕES GLOBAIS
////////////////////////////////////////////////////////////////////////////
int commandToRun;
const unsigned char PIPE_READ = 0;
const unsigned char PIPE_WRITE = 1;
const unsigned char PIPE_SIZE = 2;
const unsigned char BUFFER_SIZE = 4;
const unsigned char MIN_RANDOM_VALUE = 1;
const unsigned char MAX_RANDOM_VALUE = 100;

////////////////////////////////////////////////////////////////////////////
//  INTERFACE
////////////////////////////////////////////////////////////////////////////
void print(string msg){
    cout << msg;
    cout.flush();
}
void println(string msg){
    cout << msg << endl;
}
void printPID(){
    cout << getpid() << endl;
}
void showPID(){
    println("---------------------------------");
    print(">>> PID desse executavel: ");
    printPID();
    println("---------------------------------");
}

////////////////////////////////////////////////////////////////////////////
//  TASK 1
////////////////////////////////////////////////////////////////////////////

// A lógica dessa função foi baseada no site: https://www.bitdegree.org/learn/random-number-generator-cpp
string getRandomValue(int min, int max){
    int random;
    // Inicializa a semente a cada pedido de novo valor
    srand((unsigned) time(0));
    // Obtém um número randômico entre min e max
    random = (rand() % max) + min;

    return to_string(random);
}

void task1Child(int *pipeDescribers){
    string random_str = getRandomValue(MIN_RANDOM_VALUE, MAX_RANDOM_VALUE);
    char buffer[BUFFER_SIZE];

    // Converte o valor randomico de 'string' para vetor de 'char'
    strcpy(buffer, random_str.c_str());

    // Mostra na tela o valor randômico
    print("\n>>> Valor randomico obtido: ");
    println(buffer);

    // Realiza a escrita correta do valor randômico no pipe
    close(pipeDescribers[PIPE_READ]);
    write(pipeDescribers[PIPE_WRITE], buffer, BUFFER_SIZE);

    // Finaliza o processo do filho
    exit(0);
}

void task1Father(int *pipeDescribers, int childPID){
    char buffer[BUFFER_SIZE];
     // Espera especificamente o processo do filho finalizar
    waitpid(childPID, NULL, 0);

    // Realiza a leitura correta do valor randômico no pipe
    close(pipeDescribers[PIPE_WRITE]);
    read(pipeDescribers[PIPE_READ], buffer, BUFFER_SIZE);

    // Atualiza a variavel com o valor randômico obtido no pipe
    commandToRun = atoi(buffer);
}

void task1(){
    int childPID;
    // Cria os 2 pipes para comunicação entre o pai e o filho
    int pipeDescribers[PIPE_SIZE];
    // Abre os pipes para a conexão
    if (pipe(pipeDescribers)){
        println("\n-> Erro ao criar um pipe entre o pai e o filho");
        return; // Não continua os comandos da task 1
    }

    // Cria um processo cópia (filho) do atual (pai) verificando se houve algum problema (retorno negativo)
    if ((childPID = fork()) < 0)
        return;

    // Fork do filho
    if (childPID == 0) 
        task1Child(pipeDescribers); 
    // Fork do pai 
    else
        task1Father(pipeDescribers, childPID);
}


////////////////////////////////////////////////////////////////////////////
//  TASK 2
////////////////////////////////////////////////////////////////////////////
void task2Child(){
    // Pula duas linhas para que os prints saiam no começo da próxima linha
    println("\n");

    // Comando executado caso a variavel contenha 0
    if (commandToRun == 0){
        println("\n>>> Nao ha comando a executar");
        exit(0);
    }

    // Comando executado caso a variavel contenha um valor par
    else if (commandToRun % 2 == 0)
        execlp("/bin/ping", "ping", "8.8.8.8", "-c", "5",NULL);

    // Comando executado caso a variavel contenha um valor impar
    else if (commandToRun % 2 != 0) 
        execlp("/bin/ping", "ping", "paris.testdebit.info", "-c", "5", "-i", "2", NULL);
}

void task2Father(int childPID){
     // Espera especificamente o processo do filho finalizar
    waitpid(childPID, NULL, 0);
}

void task2(){
    int childPID;
    // Cria um processo cópia (filho) do atual (pai) verificando se houve algum problema (retorno negativo)
    if ((childPID = fork()) < 0)
        return;

    // Fork do filho
    if (childPID == 0) 
        task2Child(); 
    // Fork do pai 
    else 
        task2Father(childPID);
}

////////////////////////////////////////////////////////////////////////////
//  HANDLERS
////////////////////////////////////////////////////////////////////////////
void handlerTask1 (int sigNumber){
    if (sigNumber == SIGUSR1){
        task1();
        print("\nEsperando comando");
    }
}

void handlerTask2 (int sigNumber){
    if (sigNumber == SIGUSR2) {
        task2();
        print("\nEsperando comando");
    }
}

void handlerFinishTask (int sigNumber){
    if (sigNumber == SIGTERM) {
        println("\n>>> Finalizando o disparador");
        sleep(1);
        exit(0);
    }
}

////////////////////////////////////////////////////////////////////////////
//  FUNÇÃO PRINCIPAL
////////////////////////////////////////////////////////////////////////////
int main(){
    // Mostra o valor de pid do processo atual
    showPID();

    // Inicializa zerada a variavel de comandos
    commandToRun = 0;

    // Configura o handler da task 1 (verificando erros)
    if (signal(SIGUSR1, handlerTask1) == SIG_ERR)
        println("\n-> Erro de tratamento do SIGUSR1");

    // Configura o handler da task 2 (verificando erros)
    if (signal(SIGUSR2, handlerTask2) == SIG_ERR)
        println("\n-> Erro de tratamento do SIGUSR2");
        
    // Configura o handler de finalização do processo (verificando erros)
    if (signal(SIGTERM, handlerFinishTask) == SIG_ERR)
        println("\n-> Erro de tratamento do SIGTERM");
    
    // Loop infinito
    print("\nEsperando comando");
    while(true){
        print(".");
        sleep(1);
    }

    return 0;
}