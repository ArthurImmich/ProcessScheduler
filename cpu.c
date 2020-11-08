#include <stdio.h>
#define TAM 100

typedef struct{
    //pc = program counter, ir = intruction register
    unsigned int _pc, _ir;
    //cpu status 'normal', 'instrução ilegal', 'violação de memória',  _pm = program memory
    char status[18], char _mp[TAM][TAM];
    //_md = data memory
    int *_md;
}cpu;

typedef struct{

}memory;

/* instrução	argumentos	descrição
    CARGI	n	coloca o valor n no acumulador (A=n)
    CARGM	n	coloca no acumulador o valor na posição n da memória de dados (A=M[n])
    CARGX	n	coloca no acumulador o valor na posição que está na posição n da memória de dados (A=M[M[n]])
    ARMM	n	coloca o valor do acumulador na posição n da memória de dados (M[n]=A)
    ARMX	n	coloca o valor do acumulador posição que está na posição n da memória de dados (M[M[n]]=A)
    SOMA	n	soma ao acumulador o valor no endereço n da memória de dados (A=A+M[n])
    NEG		inverte o sinal do acumulador (A=-A)
    DESVZ	n	se A vale 0, coloca o valor n no PC
    outra		coloca a CPU em interrupção – instrução ilegal */

void cpu_altera_programa(cpu *c, int size, char *m[size]);
void cpu_altera_dados(cpu *c, int size, int m[size]);
void cpu_salva_dados(cpu *c, int size, int m[size]);
cpu_interrupcao_t cpu_interrupcao(cpu *c);
void cpu_retorna_interrupcao(cpu *c);
char *cpu_instrucao(cpu *c);
void cpu_salva_estado(cpu *c, cpu_estado_t *e);
void cpu_altera_estado(cpu *c, cpu_estado_t *e);
void cpu_estado_inicializa(cpu *c);
void cpu_executa(cpu *c);

int main(){
    char *programa[] = {
        "CARGI 10",
        "ARMM 2",
        "CARGI 32",
        "SOMA 2",
        "ARMM 0",
        "PARA"
    };
    int dados[4];
    cpu c;
    cpu_estado_inicializa(&c);
    cpu_altera_estado(&c);
    cpu_altera_programa(&c, (sizeof(programa)/sizeof(programa[0])), programa);
    cpu_altera_dados(&c, sizeof(dados)/sizeof(dados[0]), dados);
    while (cpu_interrupcao(&c) == normal) {
        cpu_executa(&c);
    }
    // cpu_salva_dados(&c, 4, dados); // se for o caso
    printf("CPU parou na instrução %s (deve ser PARA)\n", cpu_instrucao(&c));
    printf("O valor de m[0] é %d (deve ser 42)\n", dados[0]);
}

void cpu_estado_inicializa(cpu *c){
    c->_pc = 0;
    c->_ir = 0;
    strcpy(c->status, "normal");
}

void cpu_altera_programa(cpu *c, int size, char *m[size]){
    for(int i = 0; i < size; i++){
        c->_pm[0] = m[0];
    }
}

void cpu_altera_dados(cpu *c, int size, int m[size]){
    for(int i = 0; i < size; i++){
        c->_md[i] = m[i];
    }
}

void cpu_salva_dados(cpu *c, int size, int m[size]){

}

void cpu_altera_estado(cpu *c){

}