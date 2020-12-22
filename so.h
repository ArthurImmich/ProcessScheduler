#include "jobs.h"

enum estados
{
    PRONTO,
    BLOQUEADO,
    TERMINADO
};

typedef struct processo
{
    cpu_estado_t estado_cpu;
    char *nome;
    memory memoria;
    enum estados estado_job;
    t_jobs job;
    int *dados;
    struct processo *next;

} t_processo;

void novo_processo(t_processo **tabela_processos, FILE *file, timer_t *timer, char *nome)
{

    t_processo *aux = malloc(sizeof(t_processo));
    if(!aux){
        printf("Not enough memory");
        exit(EXIT_FAILURE);
    }
    
    aux->job = create_job(file);
    if (aux->job.data > 0)
    {
        aux->estado_job = BLOQUEADO;
        timer_int_as(timer, aux->job.data, nome);
    }
    else
    {
        aux->estado_job = PRONTO;
    }
    aux->dados = malloc(aux->job.tam_memoria * sizeof(int));
    if (!aux->dados)
    {
        printf("Not enough memory");
    }
    cpu_estado_inicializa(&aux->estado_cpu);
    aux->nome = nome;
    aux->next = *tabela_processos;
    *tabela_processos = aux;
}

void cria_tabela_processos(int *argc, char **argv, t_processo **tabela_processos, timer_t *timer)
{
    int i;
    for (i = 1; i <= *argc; i++)
    {
        FILE *file;
        file = fopen(argv[i], "r");
        novo_processo(tabela_processos, file, timer, argv[i]);
        fclose(file);
    }
}

void controlador_executa(cpu *c, t_processo **tabela_processos, timer_t *timer, int entrada[][2], int saida[][2]);


void sistema_operacional_inicia(cpu *c, char **argv, int *argc, int entrada[][2], int saida[][2])
{
    timer_t timer;
    timer_inicializa(&timer);
    t_processo *tabela_processos = NULL;
    cria_tabela_processos(argc, argv, &tabela_processos, &timer);
    controlador_executa(c, &tabela_processos, &timer, entrada, saida);
}

void sistema_operacional_executa(cpu *c, timer_t *timer, t_processo *aux, int entrada[][2], int saida[][2])
{
    if (c->reg.status == INSTRUCAOILEGAL)
    {
        char instraux[strlen(c->m._pm[c->reg._pc])+1];
        strcpy(instraux, c->m._pm[c->reg._pc]);
        char *instr = strtok(instraux, " ");
        int intaux;
        if (strcmp(instr, "PARA") == 0)
        {
            printf("\nIntr: %s", instr);
            aux->estado_job = TERMINADO; 
        }
        else if (strcmp(instr, "LE") == 0)
        {
            printf("\nIntr: %s", instr);
            instr = strtok(NULL, " ");
            printf(" %s", instr);
            intaux = atoi(instr);
            cpu_dorme(c);
            timer_int_de(timer, entrada[intaux][0], aux->nome);
            aux->estado_job = BLOQUEADO;
            //Executa o que é pra executar
            cpu_salva_estado(c, &aux->estado_cpu);
            cpu_estado_altera_acumulador(&aux->estado_cpu, entrada[intaux][1]);
            cpu_altera_estado(c, &aux->estado_cpu);
        }
        else if (strcmp(instr, "GRAVA") == 0)
        {
            printf("\nIntr: %s", instr);
            instr = strtok(NULL, " ");
            printf(" %s", instr);
            intaux = atoi(instr);
            cpu_dorme(c);
            timer_int_de(timer, saida[intaux][0], aux->nome);
            aux->estado_job = BLOQUEADO;
            //Executa o que é pra executar
            cpu_salva_estado(c, &aux->estado_cpu);
            saida[intaux][1] = cpu_estado_acumulador(&aux->estado_cpu);
            cpu_altera_estado(c, &aux->estado_cpu);
        }
        else
        {
            printf("Intrucao Ilegal Detectada.");
            exit(EXIT_FAILURE);
        }
    }
    if (c->reg.status == VIOLACAODEMEMORIA)
        printf("Violação de memória");
}