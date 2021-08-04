////////////////////////////////////////////////////////////////////////////
//  Universidade Federal do Rio de Janeiro
//  Disciplina: Introdução ao Aprendizado de Máquina - EEL770
//  Professor: Rodrigo Souza Couto
//  Desenvolvedor: Chritian Marques de Oliveira Silva
//  DRE: 117.214.742
//  Trabalho 1: Inicialização de programas e comunicação entre processos
////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////
//  IMPORTAÇÃO DE BIBLIOTECAS E ONFIGURAÇÕES DE AMBIENTE
////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <string>
#include <unistd.h>
#include <cstdlib>
#include <csignal>
#include <sys/types.h>
#include <sys/wait.h>
using namespace std; 

// Interações com o sistema pelo terminal:
// kill -SIGUSR1 <pid>     => Chama a task1
// kill -SIGUSR2 <pid>     => Chama a task2
// kill -SIGTERM <pid>     => Finaliza a execução

////////////////////////////////////////////////////////////////////////////
//  VARIAVEL GLOBAL DE COMANDOS PARA EXECUÇÃO
////////////////////////////////////////////////////////////////////////////
int commandToRun;

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
    print("-> PID desse executavel: ");
    printPID();
    println("---------------------------------");
}

////////////////////////////////////////////////////////////////////////////
//  TASK 1
////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////
//  TASK 2
////////////////////////////////////////////////////////////////////////////
void task2Child(){
    print("Sou o filho, meu pid: ");
    printPID();

    // Comando executado caso a variavel contenha 0
    if (commandToRun == 0){
        println("Nao ha comando a executar");
        exit(0);
    }

    // Comando executado caso a variavel contenha um valor par
    else if (commandToRun % 2 == 0)
        execlp("/bin/ping","ping","8.8.8.8","-c","5",NULL);

    // Comando executado caso a variavel contenha um valor impar
    else if (commandToRun % 2 != 0) 
        execlp("/bin/ping", "ping", "paris.testdebit.info", "-c", "5", "-i", "2", NULL);
}

void task2Father(int childPID){
     // Espera especificamente o processo do filho finalizar
    waitpid(childPID, NULL, 0);
    
    println("Sou o pai, pid do filho: " + to_string(childPID));
}

void task2(){
    commandToRun = 3;
    int childPID = fork();

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
        println("\nSIGUSR1");
        print("\nEsperando comando");
    }
}

void handlerTask2 (int sigNumber){
    if (sigNumber == SIGUSR2) {
        println("\nSIGUSR2");
        task2();
        print("\nEsperando comando");
    }
}

void handlerFinishTask (int sigNumber){
    if (sigNumber == SIGTERM) {
        println("\nFinalizando o disparador");
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
        println("-> Erro de tratamento do SIGUSR1");

    // Configura o handler da task 2 (verificando erros)
    if (signal(SIGUSR2, handlerTask2) == SIG_ERR)
        println("-> Erro de tratamento do SIGUSR2");
        
    // Configura o handler de finalização do processo (verificando erros)
    if (signal(SIGTERM, handlerFinishTask) == SIG_ERR)
        println("-> Erro de tratamento do SIGTERM");
    
    // Loop infinito
    print("Esperando comando");
    while(true){
        print(".");
        sleep(1);
    }

    return 0;
}