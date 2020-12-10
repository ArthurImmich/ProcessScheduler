void controlador_inicia(cpu *c, cpu_estado_t *e, char **programa, int tam_programa, int *dados, int tam_dados, int *entrada, int *saida){
    sistema_operacional_inicia(c, e, programa, tam_programa, dados, tam_dados, entrada, saida);
}

void controlador_executa(cpu *c, int size, cpu_estado_t *e, int *entrada, int *saida){
    bool para = false
    while(para == false){
        if(c->reg.status == 0) cpu_executa(c, size);
        else if (c->reg.status == 3)
        else sistema_operacional_executa(c, e, timer, para, entrada, saida);
    }
}
