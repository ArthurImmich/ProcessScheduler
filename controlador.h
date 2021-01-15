#include "timer.h"
#include "so.h"
#include <stdbool.h>
#define TAM_QUANTUM 5

t_processo *escalonador(t_processo *tabela_processos, bool *stop)
{
    t_processo *aux;
    t_processo *aux2 = NULL;
    float prioridade = 2;
    *stop = true;
    for (aux = tabela_processos; aux != NULL; aux = aux->next)
    {
        if (aux->estado_job == PRONTO && aux->prioridade < prioridade)
        {
            aux2 = aux;
            prioridade = aux->prioridade;
            *stop = false;
        }
        if (*stop && aux->estado_job == BLOQUEADO)
            *stop = false;
    }
    return aux2;
}

void checa_interrupcao(t_processo *tabela_processos, char *interr)
{
    t_processo *aux;
    for (aux = tabela_processos; aux != NULL; aux = aux->next)
    {
        if (strcmp(interr, aux->nome) == 0)
        {
            cpu_retorna_interrupcao(&aux->estado_cpu);
            aux->estado_job = PRONTO;
        }
    }
}

void ImprimeTabela(t_processo *tabela_processos)
{
    t_processo *aux;
    for (aux = tabela_processos; aux != NULL; aux = aux->next)
    {
        printf("\n\nNome: %s", aux->nome);
        printf("\nTempo de cpu: %i", aux->cpu_time);
        printf("\nHora de inicio: %i", aux->job.data);
        printf("\nHora de termino: %i", aux->fim);
        printf("\nVezes que foi bloqueado: %i", aux->vezes_bloqueado);
        printf("\nTempo bloqueado: %i", aux->tempo_bloqueado);
        printf("\nTempo de retorno: %i", aux->fim - aux->job.data);
        for (int i = 0; i < aux->job.qtd_saida; i++)
        {
            for (int j = 1; j < aux->job.posicao_saida[i]; j++)
            {
                printf("\nSaida: %i posicao %i: %i", i, j - 1, aux->job.saida[i][j]);
            }
        }
    }
}

void controlador_inicia(cpu *c, char **argv, int *argc)
{
    sistema_operacional_inicia(c, argv, argc);
}

void atualiza_tempo_bloqueado(t_processo *tabela_processos, int tempo_cpu)
{
    t_processo *aux;
    for (aux = tabela_processos; aux != NULL; aux = aux->next)
    {
        aux->tempo_bloqueado = tempo_cpu - aux->cpu_time;
    }
}

void controlador_executa(cpu *c, t_processo *tabela_processos, timer_t *timer)
{
    t_processo *aux = NULL;
    unsigned int cpu_ociosa = 0;
    bool stop = false;
    char *interr;
    int quantum;
    while (stop == false)
    {
        //Informa se ainda há processos para rodar
        //Se um processo estiver pronto, check_process deixará aux parado nesse processo
        aux = escalonador(tabela_processos, &stop);
        if (aux && aux->estado_job == PRONTO)
        {
            cpu_altera_estado(c, &aux->estado_cpu);
            cpu_altera_programa(c, aux->job.tam_programa, aux->job.programa);
            cpu_altera_dados(c, aux->job.tam_memoria, aux->dados);
            quantum = TAM_QUANTUM;
            while (aux->estado_job == PRONTO)
            {
                quantum--;
                if (quantum <= 0)
                {
                    cpu_dorme(c);
                    timer_int_de(timer, 1, aux->nome);
                    aux->estado_job = BLOQUEADO;
                    cpu_salva_estado(c, &aux->estado_cpu);
                    break;
                }
                if (cpu_interrupcao(c) == NORMAL)
                {
                    cpu_executa(c, aux->job.tam_programa);
                }
                if (cpu_interrupcao(c) == INSTRUCAOILEGAL || cpu_interrupcao(c) == VIOLACAODEMEMORIA)
                {
                    sistema_operacional_executa(c, timer, aux, (aux->prioridade * ((float)quantum / (float)TAM_QUANTUM)));
                }
                //checa por interrupções
                do
                {
                    interr = timer_interrupcao(timer);
                    if (strcmp(interr, "nenhum") != 0)
                    {
                        checa_interrupcao(tabela_processos, interr);
                    }
                } while (strcmp(interr, "nenhum") != 0);
                aux->cpu_time++;
                timer_tictac(timer);
            }
            aux->vezes_bloqueado++;
            cpu_salva_dados(c, aux->job.tam_memoria, aux->dados);
        }
        do
        {
            interr = timer_interrupcao(timer);
            if (strcmp(interr, "nenhum") != 0)
            {
                checa_interrupcao(tabela_processos, interr);
            }
        } while (strcmp(interr, "nenhum") != 0);
        cpu_ociosa++;
        timer_tictac(timer);
    }

    atualiza_tempo_bloqueado(tabela_processos, timer_agora(timer));
    printf("\n\nTempo de cpu ociosa: %i", cpu_ociosa);
    printf("\nTempo de execucao total: %i", timer_agora(timer));
    ImprimeTabela(tabela_processos);
}
