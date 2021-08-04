#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <sys/types.h>
#include <csignal>

/*
 * Interações com o sistema pelo terminal:
 * kill -s USR1 <pid>     => Chama a task1
 * kill -s USR2 <pid>     => Chama a task2
 * kill -s TERM <pid>     => Finaliza a execução
*/

using namespace std; 

/* Facilitação de mensagens printadas na tela */
void print(string msg){
    cout << msg;
    cout.flush();
}
void println(string msg){
    cout << msg << endl;
}

void showPID(){
    pid_t pid = getpid();
    println("---------------------------------");
    print("-> PID desse executavel: ");
    cout << pid << endl; // Possui um tipo diferente => mais facil imprimir sem converter pra string
    println("---------------------------------");
}

void task1 (int sigNumber){
    if (sigNumber == SIGUSR1){
        println("\nSIGUSR1");
        print("\nEsperando comando");
    }
}

void task2 (int sigNumber){
    if (sigNumber == SIGUSR2) {
        println("\nSIGUSR2");
        print("\nEsperando comando");
    }
}

void finishTask (int sigNumber){
    if (sigNumber == SIGTERM) {
        println("\nFinalizando o disparador");
        sleep(1);
        exit(0);
    }
}

int main(){
    //int comandoParaExecutar = 0;
    
    showPID();

    if (signal(SIGUSR1, task1) == SIG_ERR)
        println("-> Erro de tratamento do SIGUSR1");

    if (signal(SIGUSR2, task2) == SIG_ERR)
        println("-> Erro de tratamento do SIGUSR2");
        
    if (signal(SIGTERM, finishTask) == SIG_ERR)
        println("-> Erro de tratamento do SIGTERM");
    
    print("Esperando comando");
    while(true){
        print(".");
        sleep(1);
    }
    
    println("-> Codigo finalizado com sucesso");

    return 0;
}