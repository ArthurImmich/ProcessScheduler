#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void sistema_operacional_inicia(cpu *c, cpu_estado_t *e, char **programa, int tam_programa, int *dados, int tam_dados){
    // inicializa o estado da CPU
    cpu_estado_inicializa(e);
    // inicializa a CPU, com o estado interno ...
    cpu_altera_estado(c, e);
    // ... a memória de programa ...
    cpu_altera_programa(c, tam_programa, programa);
    // ... e a memória de dados
    cpu_altera_dados(c, tam_dados, dados);
    // faz a CPU executar cada instrução do programa, 
    // até que cause uma interrupção (que deve ser por instrução ilegal em PARA)
    controlador_executa(c, tam_dados);
    cpu_salva_dados(c, tam_dados, dados); // se for o caso
}

void sistema_operacional_executa(cpu *c){
    if(c->reg.status == 1){
        char *instr = strtok(c->m._pm[c->reg._pc], " ");
        int aux;
        if (strcmp(instr, "PARA") == 0){
            exit(EXIT_SUCCESS);
        }
        if (strcmp(instr, "LE") == 0){
            instr = strtok(NULL, " ");
            aux = atoi(instr);
            cpu_estado_altera_acumulador(&c->reg, aux);
            c->reg._pc++;
        }
        if (strcmp(instr, "GRAVA") == 0){
            instr = strtok(NULL, " ");
            aux = atoi(instr);
            int dispositivoes = cpu_estado_acumulador(&c->reg); //por resolver na proxima aula
            c->reg._pc++;
        }
    }
    if(c->reg.status == 2) printf("Violação de memória");
}