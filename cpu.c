#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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



void cpu_altera_programa(cpu *c, int size, char *m[size]);
void cpu_altera_dados(cpu *c, int size, int m[size]);
void cpu_salva_dados(cpu *c, int size, int m[size]);
cpu_interrupcao_t cpu_interrupcao(cpu *c);
void cpu_retorna_interrupcao(cpu *c);
char *cpu_instrucao(cpu *c);
void cpu_salva_estado(cpu *c, cpu_estado_t *e);
void cpu_altera_estado(cpu *c, cpu_estado_t *e);
void cpu_estado_inicializa(cpu_estado_t *e);
void cpu_executa(cpu *c);
void cpu_estado_altera_acumulador(cpu_estado_t *e, int novo_valor_do_acum);
int cpu_estado_acumulador(cpu_estado_t *e);

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
    // inicializa o estado da CPU
    cpu_estado_inicializa(&e);
    // inicializa a CPU, com o estado interno ...
    cpu_altera_estado(&c, &e);
    // ... a memória de programa ...
    cpu_altera_programa(&c, sizeof(programa)/sizeof(programa[0]), programa);
    // ... e a memória de dados
    cpu_altera_dados(&c, sizeof(dados)/sizeof(dados[0]), dados);
    // faz a CPU executar cada instrução do programa, 
    // até que cause uma interrupção (que deve ser por instrução ilegal em PARA)
    while (cpu_interrupcao(&c) == 0) {
        cpu_executa(&c);
    }
    cpu_salva_dados(&c, sizeof(dados)/sizeof(dados[0]), dados); // se for o caso
    printf("CPU parou na instrucao %s (deve ser PARA)\n", cpu_instrucao(&c));
    printf("O valor de m[0] e %d (deve ser 42)\n", dados[0]);
}

void cpu_altera_programa(cpu *c, int size, char *m[size]){
    c->m._pm = malloc(sizeof(m));
    for(int i = 0; i < size; i++){
        c->m._pm[i] = malloc(sizeof(m[i]));
        strcpy(c->m._pm[i], m[i]);
    }
}

void cpu_altera_dados(cpu *c, int size, int *m){
    c->m._md = malloc(sizeof(m));
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

char *cpu_instrucao(cpu *c){
    if(sizeof(c->m._pm)/sizeof(c->m._pm[0]) < c->reg._pc)
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

void cpu_executa(cpu *c){
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
        if (sizeof(c->m._md)/sizeof(c->m._md[0]) > aux && aux >= 0)
        {
            c->reg._acc = c->m._md[aux];
            c->reg._pc++;
        }
        else
        {
            c->reg.status = 2ç
        }
    }
    else if (strcmp(instr, "CARGX") == 0){
        instr = strtok(NULL, " ");
        aux = atoi(instr);
        if (sizeof(c->m._md)/sizeof(c->m._md[0]) > c->m._md[aux] && c->m._md[aux] >= 0)
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
        if (sizeof(c->m._md)/sizeof(c->m._md[0]) > aux && aux >= 0)
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
        if (sizeof(c->m._md)/sizeof(c->m._md[0]) > c->m._md[aux] && c->m._md[aux] >= 0)
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
        if (sizeof(c->m._md)/sizeof(c->m._md[0]) > aux && aux >= 0)
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
    else if (strcmp(instr, "PARA") == 0){
        instr = strtok(NULL, " ");
        exit(atoi(instr));
    }
    else if (strcmp(instr, "LE") == 0){
        instr = strtok(NULL, " ");
        cpu_estado_altera_acumulador(&c->reg, dispositivoes); //dispositivoes sera implementado na proxima parte
        c->reg._pc++;
    }
    else if (strcmp(instr, "GRAVA") == 0){
        instr = strtok(NULL, " ");
        dispositivoes = cpu_estado_acumulador(&c->reg);
        c->reg._pc++;
    }
    else c->reg.status = 1;
}

/* instrução	argumentos	descrição
    CARGI	n	coloca o valor n no acumulador (A=n)
    CARGM	n	coloca no acumulador o valor na posição n da memória de dados (A=M[n])
    CARGX	n	coloca no acumulador o valor na posição que está na posição n da memória de dados (A=M[M[n]])
    ARMM	n	coloca o valor do acumulador na posição n da memória de dados (M[n]=A)
    ARMX	n	coloca o valor do acumulador posição que está na posição n da memória de dados (M[M[n]]=A)
    SOMA	n	soma ao acumulador o valor no endereço n da memória de dados (A=A+M[n])
    NEG		inverte o sinal do acumulador (A=-A)
    DESVZ	n	se A vale 0, coloca o valor n no PC
    outra		coloca a CPU em interrupção – instrução ilegal 
    PARA	n	pede ao SO para terminar a execução do programa (como exit)
    LE	n	pede ao SO para fazer a leitura de um dado (inteiro) do dispositivo de E/S n; o dado será colocado no acumulador
    GRAVA	n	pede ao SO gravar o valor do acumulador no dispositivo de E/S n*/