#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//CPU

typedef int cpu_interrupcao_t;

typedef struct{
    //pc = program counter, acc = accumulator
    unsigned int _pc;
    int _acc;
    //cpu status 0 =  'normal', 1 = 'instrução ilegal', 2 = 'violação de memória'
    cpu_interrupcao_t status;
}cpu_estado_t;

typedef struct{
    //_pm = program memory
    char **_pm;
    //_md = data memory
    int *_md;
}memory;

typedef struct{
    //estado
    cpu_estado_t reg;
    //memória
    memory m;
}cpu;

void cpu_altera_programa(cpu *c, int size, char *m[size]){
    c->m._pm = malloc(size * sizeof(char *));
    if(c->m._pm == NULL) return;
    for(int i = 0; i < size; i++){
        c->m._pm[i] = malloc((strlen(m[i]) + 1) * sizeof(char));
        if(c->m._pm[i] == NULL) return;
        strcpy(c->m._pm[i], m[i]);
    }
}

void cpu_altera_dados(cpu *c, int size, int *m){
    c->m._md = malloc(size * sizeof(int));
    if(c->m._md == NULL) return;
    for(int i = 0; i < size; i++){
        c->m._md[i] = m[i];
    }

}

void cpu_salva_dados(cpu *c, int size, int *m){
    for(int i = 0; i < size; i++){
        m[i] = c->m._md[i];
    }
}

cpu_interrupcao_t cpu_interrupcao(cpu *c){
    return c->reg.status;
}

void cpu_retorna_interrupcao(cpu *c){
    if (c->reg.status != 0){
        c->reg.status = 0;
        c->reg._pc++;
    }
}

char *cpu_instrucao(cpu *c, int size){
    if(size > c->reg._pc)
        return c->m._pm[c->reg._pc];
    else
        return "invalida";
}

void cpu_salva_estado(cpu *c, cpu_estado_t *e){
    e->_pc = c->reg._pc;
    e->_acc = c->reg._acc;
    e->status = c->reg.status;
}

void cpu_altera_estado(cpu *c, cpu_estado_t *e){
    c->reg._pc = e->_pc;
    c->reg._acc = e->_acc;
    c->reg.status = e->status;
}

void cpu_estado_inicializa(cpu_estado_t *e){
    e->_pc = 0;
    e->_acc = 0;
    e->status = 0; 
}

void cpu_estado_altera_acumulador(cpu_estado_t *e, int novo_valor_do_acum){
    e->_acc = novo_valor_do_acum;
}

int cpu_estado_acumulador(cpu_estado_t *e){
    return e->_acc;
}

void cpu_executa(cpu *c, int size){
//gets the first part of the isntruction;
    char *instr = strtok(c->m._pm[c->reg._pc], " ");
    int aux;
    if (strcmp(instr, "CARGI") == 0){
        //gets the argument of the instruction;
        instr = strtok(NULL, " ");
        //atoi() transforms a string in a integer
        c->reg._acc = atoi(instr);
        c->reg._pc++;
    }
    else if (strcmp(instr, "CARGM") == 0){
        instr = strtok(NULL, " ");
        aux = atoi(instr);
        if (size > aux && aux >= 0)
        {
            c->reg._acc = c->m._md[aux];
            c->reg._pc++;
        }
        else
        {
            c->reg.status = 2;
        }
    }
    else if (strcmp(instr, "CARGX") == 0){
        instr = strtok(NULL, " ");
        aux = atoi(instr);
        if (size > c->m._md[aux] && c->m._md[aux] >= 0)
        {
            c->reg._acc = c->m._md[c->m._md[aux]];
            c->reg._pc++;
        }
        else
        {
            c->reg.status = 2;
        }
    }
    else if (strcmp(instr, "ARMM") == 0){
        instr = strtok(NULL, " ");
        aux = atoi(instr);
        if (size > aux && aux >= 0)
        {
            c->m._md[aux] = c->reg._acc;
            c->reg._pc++;
        }
        else
        {
            c->reg.status = 2;
        }
    }
    else if (strcmp(instr, "ARMX") == 0){
        instr = strtok(NULL, " ");
        aux = atoi(instr);
        if (size > c->m._md[aux] && c->m._md[aux] >= 0)
        {
            c->m._md[c->m._md[aux]] = c->reg._acc;
            c->reg._pc++;
        }
        else
        {
            c->reg.status = 2;
        }
    }
    else if (strcmp(instr, "SOMA") == 0){
        instr = strtok(NULL, " ");
        aux = atoi(instr);
        if (size > aux && aux >= 0)
        {
            c->reg._acc += c->m._md[aux];
            c->reg._pc++;
        }
        else
        {
            c->reg.status = 2;
        }
    }
    else if (strcmp(instr, "NEG") == 0){
        c->reg._acc = -c->reg._acc;
        c->reg._pc++;
    }
    else if (strcmp(instr, "DESVZ") == 0){
        if (c->reg._acc == 0){
            instr = strtok(NULL, " ");
            c->reg._pc = atoi(instr);
        }
        else{
            c->reg._pc++;
        }
    }
    else c->reg.status = 1;
}

// instrução	argumentos	descrição
// CARGI	n	coloca o valor n no acumulador (A=n)
// CARGM	n	coloca no acumulador o valor na posição n da memória de dados (A=M[n])
// CARGX	n	coloca no acumulador o valor na posição que está na posição n da memória de dados (A=M[M[n]])
// ARMM	n	coloca o valor do acumulador na posição n da memória de dados (M[n]=A)
// ARMX	n	coloca o valor do acumulador posição que está na posição n da memória de dados (M[M[n]]=A)
// SOMA	n	soma ao acumulador o valor no endereço n da memória de dados (A=A+M[n])
// NEG		inverte o sinal do acumulador (A=-A)
// DESVZ	n	se A vale 0, coloca o valor n no PC
// outra		coloca a CPU em interrupção – instrução ilegal 
// PARA	n	pede ao SO para terminar a execução do programa (como exit)
// LE	n	pede ao SO para fazer a leitura de um dado (inteiro) do dispositivo de E/S n; o dado será colocado no acumulador
// GRAVA	n	pede ao SO gravar o valor do acumulador no dispositivo de E/S n