#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void controlador_inicia(cpu *c, cpu_estado_t *e, char **programa, int tam_programa, int *dados, int tam_dados){
    sistema_operacional_inicia(c, e, programa, tam_programa, dados, tam_dados);
}

void controlador_executa(cpu *c, int size){
    while(1){
        if(c->reg.status == 0) cpu_executa(c, size);
        else sistema_operacional_executa(c);
    }
}
