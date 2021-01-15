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
    int fim;
    int tempo_bloqueado;
    int vezes_bloqueado;
    float prioridade;
    char *nome;
    memory memoria;
    enum estados estado_job;
    t_jobs job;
    int *dados;
    unsigned int cpu_time;
    struct processo *next;
} t_processo;

void novo_processo(t_processo **tabela_processos, FILE *file, timer_t *timer, char *nome)
{

    t_processo *aux = malloc(sizeof(t_processo));
    if (!aux)
    {
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
    for (int i = 0; i < aux->job.qtd_entrada; i++)
    {
        aux->job.posicao_entrada[i] = 1;
    }
    for (int i = 0; i < aux->job.qtd_saida; i++)
    {
        aux->job.posicao_saida[i] = 1;
    }
    aux->cpu_time = 0;
    aux->fim = 0;
    aux->tempo_bloqueado = 0;
    aux->vezes_bloqueado = 0;
    aux->prioridade = 0.5;
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
        int length = snprintf(NULL, 0, "%d", i);
        char *str = malloc(length + 1);
        snprintf(str, length + 1, "%d", i);
        strcat(argv[i], str); //impede processos com nomes iguais
        novo_processo(tabela_processos, file, timer, argv[i]);
        fclose(file);
    }
}

void controlador_executa(cpu *c, t_processo *tabela_processos, timer_t *timer);

void sistema_operacional_inicia(cpu *c, char **argv, int *argc)
{
    timer_t timer;
    timer_inicializa(&timer);
    t_processo *tabela_processos = NULL;
    cria_tabela_processos(argc, argv, &tabela_processos, &timer);
    controlador_executa(c, tabela_processos, &timer);
}

void sistema_operacional_executa(cpu *c, timer_t *timer, t_processo *aux, float prioridade)
{
    if (c->reg.status == INSTRUCAOILEGAL)
    {
        char instraux[strlen(c->m._pm[c->reg._pc]) + 1];
        strcpy(instraux, c->m._pm[c->reg._pc]);
        char *instr = strtok(instraux, " ");
        int intaux;
        if (strcmp(instr, "PARA") == 0)
        {
            aux->fim = timer_agora(timer);
            aux->estado_job = TERMINADO;
        }
        else if (strcmp(instr, "LE") == 0)
        {
            instr = strtok(NULL, " ");
            intaux = atoi(instr);
            c->reg._pc++;
            cpu_dorme(c);
            timer_int_de(timer, aux->job.entrada[intaux][0], aux->nome);
            aux->estado_job = BLOQUEADO;
            aux->prioridade -= prioridade; //calcula quanto do quantum restou e retira esse valor em forma de % do valor da prioridade anterior
            //Executa o que é pra executar
            cpu_salva_estado(c, &aux->estado_cpu);
            cpu_estado_altera_acumulador(&aux->estado_cpu, aux->job.entrada[intaux][aux->job.posicao_entrada[intaux]]);
            aux->job.posicao_entrada[intaux]++;
            cpu_altera_estado(c, &aux->estado_cpu);
        }
        else if (strcmp(instr, "GRAVA") == 0)
        {
            instr = strtok(NULL, " ");
            intaux = atoi(instr);
            c->reg._pc++;
            cpu_dorme(c);
            timer_int_de(timer, aux->job.saida[intaux][0], aux->nome);
            aux->estado_job = BLOQUEADO;
            aux->prioridade -= prioridade; //calcula quanto do quantum restou e retira esse valor em forma de % do valor da prioridade anterior
            //Executa o que é pra executar
            cpu_salva_estado(c, &aux->estado_cpu);
            aux->job.saida[intaux][aux->job.posicao_saida[intaux]] = cpu_estado_acumulador(&aux->estado_cpu);
            aux->job.posicao_saida[intaux]++;
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
