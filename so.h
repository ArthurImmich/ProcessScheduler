void sistema_operacional_inicia(cpu *c, cpu_estado_t *e, char **programa, int tam_programa, int *dados, int tam_dados, int *entrada, int *saida){
    // inicializa o estado da CPU
    cpu_estado_inicializa(e);
    // inicializa a CPU, com o estado interno ...
    cpu_altera_estado(c, e);
    // ... a memória de programa ...
    cpu_altera_programa(c, tam_programa, programa);
    // ... e a memória de dados
    cpu_altera_dados(c, tam_dados, dados);
    // faz a CPU executar cada instrução do programa, 
    // até que cause uma interrupção
    controlador_executa(c, tam_dados, e);
    cpu_salva_dados(c, tam_dados, dados); // se for o caso
}

void sistema_operacional_executa(cpu *c, cpu_estado_t *e, timer_t *timer, bool *para, int *entrada, int *saida){
    if(c->reg.status == 1){
        char *instr = strtok(c->m._pm[c->reg._pc], " ");
        int aux;
        if (strcmp(instr, "PARA") == 0){
            printf("Execucao encerrada atraves da instrucao PARA");
            *para = true;
        }
        else if (strcmp(instr, "LE") == 0){
            instr = strtok(NULL, " ");
            aux = atoi(instr);
            cpu_dorme(c);
            timer_int_de(&timer, 3, "ENTRADA");
            //Executa o que é pra executar
            cpu_salva_estado(c, e);
            cpu_estado_altera_acumulador(&e, entrada[aux]);
            cpu_altera_estado(c, e);
            cpu_retorna_interrupcao(c);
        }
        else if (strcmp(instr, "GRAVA") == 0){
            instr = strtok(NULL, " ");
            aux = atoi(instr);
            cpu_dorme(c);
            timer_int_de(&timer, 3, "SAIDA");
            //Executa o que é pra executar
            cpu_salva_estado(c, e); 
            saida[aux] = cpu_estado_acumulador(&e);
            cpu_altera_estado(c, e);
            cpu_retorna_interrupcao(c);
        }
        else
        {
            printf("Intrucao Ilegal Detectada.");
            exit(EXIT_FAILURE);
        }
        
    }
    if(c->reg.status == 2) printf("Violação de memória");
}