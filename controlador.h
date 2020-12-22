#include "timer.h"
#include "so.h"
#include <stdbool.h>

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

void controlador_inicia(cpu *c, char **argv, int *argc, int entrada[][2], int saida[][2])
{
    sistema_operacional_inicia(c, argv, argc, entrada, saida);
}

void controlador_executa(cpu *c, t_processo **tabela_processos, timer_t *timer, int entrada[][2], int saida[][2])
{
    t_processo *aux = NULL;
    bool stop = false; 
    while (stop == false)
    {
        //Informa se ainda há processos para rodar
        //Se um processo estiver pronto, check_process deixará aux parado nesse processo
        aux = escalonador(tabela_processos, &stop);
        printf("\nexecutando processo: %s", aux->nome);
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
            }
            cpu_salva_dados(c, aux->job.tam_memoria, aux->dados);
        }
        char *interr = timer_interrupcao(timer);
        if (strcmp(interr, "nenhum") != 0)
        {
            printf("\ninterrupcao");
            checa_interrupcao(tabela_processos, interr);
        }
        printf("\ntimer: %i", timer_agora(timer));
        timer_tictac(timer);
    }
}