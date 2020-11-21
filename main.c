#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cpu.h"
#include "so.h"
#include "controlador.h"

int main(){
    // um vetor de strings contendo um programa exemplo
    char *programa[] = {
    "CARGI 10",
    "ARMM 2",
    "CARGI 32",
    "SOMA 2",
    "ARMM 0",
    "PARA"
    };
    // um vetor de inteiros que será a memória de dados da CPU
    int dados[4];
    // a variável que representará a CPU
    cpu c;
    // um local para conter e inicializar o estado da CPU
    cpu_estado_t e;
    int tam_programa = sizeof(programa)/sizeof(programa[0]);
    int tam_dados = sizeof(dados)/sizeof(dados[0]);
    controlador_inicia(&c, &e, programa, tam_programa, dados, tam_dados);
}