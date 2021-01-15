#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cpu.h"
#include "controlador.h"
<<<<<<< HEAD
=======
#define TAM 5

>>>>>>> origin/main

int main(int argc, char *argv[])
{
    // um vetor de strings contendo um programa exemplo
    if (argc < 1)
    {
        printf("Usage: ./main programa1.txt programa2.txt ...");
        return 1;
    }
<<<<<<< HEAD

    t_jobs job;
    cpu c;
    argc--;
    controlador_inicia(&c, argv, &argc);
}
=======
    FILE *file = fopen(argv[1], "r");
    if (!file)
    {
        printf("Couldn't open txt file");
    }

    t_jobs job; //sistema operacional
    // a variável que representará a CPU
    cpu c;
    //array de entrada
    //as linhas correspondem ao dispositivo de e/s (nesse casao são 4)
    //o primeiro número do array coresponde ao tempo de acesso ao dispositiov.
    int entrada[2][TAM] = {{3, 5, 2, 1, 4}, {4, 3, 5, 1, -4}};
    //array de saida
    //as linhas correspondem ao dispositivo de e/s (nesse casao são 4)
    //o primeiro número do array coresponde ao tempo de acesso ao dispositivo.
    int saida[3][TAM] = {{5}, {2}, {3}};
    argc--;
    //inicia o programa
    controlador_inicia(&c, argv, &argc, entrada, saida);
    //resultados
    printf("\nSaida 1 (resultado programa 1): %i", saida[1][1]);
    for (int i = 1; i < TAM; i++)
    {
        printf("\nSaida 2: (resultado programa 2): %i", saida[2][i]);
    }
    
}
>>>>>>> origin/main
