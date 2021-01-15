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

    t_jobs job;
    cpu c;
    argc--;
    controlador_inicia(&c, argv, &argc);
}