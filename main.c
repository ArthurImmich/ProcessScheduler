#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cpu.h"
#include "controlador.h"


int main(int argc, char *argv[])
{
    // um vetor de strings contendo um programa exemplo
    if (argc < 1)
    {
        printf("Usage: ./main programa1.txt programa2.txt ...");
        return 1;
    }
    FILE *file = fopen(argv[1], "r");
    if (!file)
    {
        printf("Couldn't open txt file");
    }

    t_jobs job; //sistema operacional
    // a variável que representará a CPU
    cpu c;
    int entrada[4][2] = {{1, 3}, {1, 1}, {1, 1}, {1, 4}};
    int saida[4][2] = {{1}, {1}, {1}, {1}};
    argc--;
    controlador_inicia(&c, argv, &argc, entrada, saida);
    printf("\nEntrada 0: %i", entrada[0][1]);
    printf("\nSaida 1: %i", entrada[1][1]);
}