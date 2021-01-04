#include "timer.h"
#include "so.h"
#include <stdbool.h>
#define TAM 5

t_processo *escalonador(t_processo **tabela_processos, bool *stop)
{
    t_processo *aux;
    t_processo *aux2;
    *stop = true;
    aux2 = *tabela_processos;
    for (aux = *tabela_processos; aux2; aux2 = aux2->next)
    {
        aux2 ? aux = aux2 : aux; 
        if (aux->estado_job == PRONTO)
        {
            *stop = false;
            return aux;
        }
        if (aux->estado_job == BLOQUEADO)
        {
            *stop = false;
        }
    }
    return aux;
}

void checa_interrupcao(t_processo **tabela_processos, char *interr)
{
    t_processo *aux;
    for (aux = *tabela_processos; aux != NULL; aux = aux->next)
    {
        if (strcmp(interr, aux->nome) == 0)
        {
            cpu_retorna_interrupcao(&aux->estado_cpu);
            aux->estado_job = PRONTO;
        }
    }
}

void controlador_inicia(cpu *c, char **argv, int *argc, int entrada[][TAM], int saida[][TAM])
{
    sistema_operacional_inicia(c, argv, argc, entrada, saida);
}

void controlador_executa(cpu *c, t_processo **tabela_processos, timer_t *timer, int entrada[][TAM], int saida[][TAM])
{
    t_processo *aux = NULL;
    unsigned int cpu_ociosa = 0;
    bool stop = false;
    char *interr;
    while (stop == false)
    {
        //Informa se ainda há processos para rodar
        //Se um processo estiver pronto, check_process deixará aux parado nesse processo
        aux = escalonador(tabela_processos, &stop);
        if (aux->estado_job == PRONTO)
        {   
            cpu_altera_estado(c, &aux->estado_cpu);
            cpu_altera_programa(c, aux->job.tam_programa, aux->job.programa);
            cpu_altera_dados(c, aux->job.tam_memoria, aux->dados);
            while (aux->estado_job == PRONTO)
            {
                if (cpu_interrupcao(c) == NORMAL)
                {
                    cpu_executa(c, aux->job.tam_programa);
                }
                if (cpu_interrupcao(c) == INSTRUCAOILEGAL || cpu_interrupcao(c) == VIOLACAODEMEMORIA)
                {
                    sistema_operacional_executa(c, timer, aux, entrada, saida);
                }
                //checa por interrupções
                interr = timer_interrupcao(timer);
                if (strcmp(interr, "nenhum") != 0)
                {
                    checa_interrupcao(tabela_processos, interr);
                }
                aux->cpu_time++;
                printf("\nTempo do processo %s : %i", aux->nome, aux->cpu_time);
                //printf("\nAcc: %i", c->reg._acc);
                timer_tictac(timer);
            }
            cpu_salva_dados(c, aux->job.tam_memoria, aux->dados);
        }
        interr = timer_interrupcao(timer);
        if (strcmp(interr, "nenhum") != 0)
        {
            checa_interrupcao(tabela_processos, interr);
        }
        cpu_ociosa++;
        timer_tictac(timer);
    }
    printf("\nTempo de cpu ociosa: %i", cpu_ociosa);
}