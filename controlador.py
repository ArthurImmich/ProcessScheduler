class Controlador:

    def inicia(self, c, argv, dados, mmu, so):
        so.inicia(c, argv, dados, mmu)
    


    def escalonador(self, tabela_processos):
        try:
            return tabela_processos.index(min(filter(lambda processo: processo.estado == 'PRONTO', tabela_processos), key=lambda processo:processo.prioridade))
        except ValueError:
            return None

    def checa_interrupcao(self, tabela_processos, interr, c):
        for processo in tabela_processos:
            if processo.nome == interr:
                c.retorna_interrupcao(processo.cpu_estado)
# void checa_interrupcao(t_processo *tabela_processos, char *interr)
# {
#     t_processo *aux;
#     for (aux = tabela_processos; aux != NULL; aux = aux->next)
#     {
#         if (strcmp(interr, aux->nome) == 0)
#         {
#             cpu_retorna_interrupcao(&aux->estado_cpu);
#             aux->estado_job = PRONTO;
#         }
#     }
# }

# void ImprimeTabela(t_processo *tabela_processos, int tempo_total_cpu)
# {
#     t_processo *aux;
#     for (aux = tabela_processos; aux != NULL; aux = aux->next)
#     {
#         printf("\n\nNome: %s", aux->nome);
#         printf("\nTempo de cpu: %i", aux->cpu_time);
#         printf("\nHora de inicio: %i", aux->job.data);
#         printf("\nHora de termino: %i", aux->fim);
#         printf("\nVezes que foi bloqueado: %i", aux->vezes_bloqueado);
#         printf("\nTempo bloqueado: %i", aux->tempo_bloqueado);
#         printf("\nQuantia de preempcao: %i", aux->preempcao);
#         printf("\nTempo de retorno: %i", aux->fim - aux->job.data);
#         printf("\nPercentual de tempo de cpu: %.2f", ((float) aux->cpu_time * 100 /(float)tempo_total_cpu));
#         for (int i = 0; i < aux->job.qtd_saida; i++)
#         {
#             for (int j = 1; j < aux->job.posicao_saida[i]; j++)
#             {
#                 printf("\nSaida: %i posicao %i: %i", i, j - 1, aux->job.saida[i][j]);
#             }
#         }
#     }
# }

# void sistema_operacional_inicia(cpu *c, char **argv, int *argc, mem_data *dados, mmu_t *mmu);

# void controlador_inicia(cpu *c, char **argv, int *argc, mem_data *dados, mmu_t *mmu)
# {
#     sistema_operacional_inicia(c, argv, argc, dados, mmu);
# }

# void atualiza_tempo_bloqueado(t_processo *tabela_processos, int tempo_cpu)
# {
#     t_processo *aux;
#     for (aux = tabela_processos; aux != NULL; aux = aux->next)
#     {
#         aux->tempo_bloqueado = tempo_cpu - aux->cpu_time;
#     }
# }

# void controlador_executa(cpu *c, t_processo *tabela_processos, timer_t *timer, mem_data *dados, mmu_t *mmu)
# {
#     t_processo *aux = NULL;
#     unsigned int cpu_ociosa = 0;
#     bool stop = false;
#     char *interr;
#     int quantum;
#     while (stop == false)
#     {
#         //Informa se ainda há processos para rodar
#         //Se um processo estiver pronto, check_process deixará aux parado nesse processo
#         aux = escalonador(tabela_processos, &stop);
#         if (aux && aux->estado_job == PRONTO)
#         {
#             cpu_altera_estado(c, &aux->estado_cpu);
#             // printf("\nProcesso: %s", aux->nome);
#             // printf("\nStatus: %i", c->reg.status);
#             // printf("\nPC: %i", c->reg._pc);
#             //por resolver
#             sistema_operacional_altera_mmu(mmu, aux);
#             cpu_altera_programa(c, aux->job.tam_programa, aux->job.programa);
#             //cpu_altera_dados(c, aux->job.tam_memoria, aux->dados);
#             quantum = TAM_QUANTUM;
#             while (aux->estado_job == PRONTO)
#             {
#                 quantum--;
#                 if (quantum <= 0)
#                 {
#                     aux->preempcao++;
#                     cpu_dorme(c);
#                     timer_int_de(timer, 1, aux->nome);
#                     aux->estado_job = BLOQUEADO;
#                     cpu_salva_estado(c, &aux->estado_cpu);
#                     break;
#                 }
#                 printf("\n\n%s", aux->nome);
#                 if (cpu_interrupcao(c) == NORMAL)
#                 {
#                     cpu_executa(c, dados, mmu, &aux->mem_secundaria, tabela_processos, timer, aux);
#                 }
#                 if (cpu_interrupcao(c) == INSTRUCAOILEGAL || cpu_interrupcao(c) == VIOLACAODEMEMORIA)
#                 {
#                     sistema_operacional_executa(c, timer, aux, (aux->prioridade * ((float)quantum / (float)TAM_QUANTUM)));
#                 }
#                 //checa por interrupções
#                 do
#                 {
#                     interr = timer_interrupcao(timer);
#                     if (strcmp(interr, "nenhum") != 0)
#                     {
#                         checa_interrupcao(tabela_processos, interr);
#                     }
#                 } while (strcmp(interr, "nenhum") != 0);
#                 aux->cpu_time++;
#                 timer_tictac(timer);
#             }
#             aux->vezes_bloqueado++;
#             cpu_salva_dados(dados, mmu, aux);
#         }
#         do
#         {
#             interr = timer_interrupcao(timer);
#             if (strcmp(interr, "nenhum") != 0)
#             {
#                 checa_interrupcao(tabela_processos, interr);
#             }
#         } while (strcmp(interr, "nenhum") != 0);
#         cpu_ociosa++;
#         timer_tictac(timer);
#     }

#     atualiza_tempo_bloqueado(tabela_processos, timer_agora(timer));
#     printf("\n\nTempo de cpu ociosa: %i", cpu_ociosa);
#     printf("\nTempo de execucao total: %i", timer_agora(timer));
#     ImprimeTabela(tabela_processos, timer_agora(timer));
# }