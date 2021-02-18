class Controlador:

    def TAM_QUANTUM(self):
        return 4

    #inicia o controlador
    def inicia(self, c, argv, dados, mmu, so, timer):
        so.inicia(self, c, argv, dados, mmu, timer)

    #retorna o indice do processo a ser executado se pronto, se existir processo bloqueado
    #retorna none, se todos processos estiverem terminados, retorna False
    def escalonador(self, tabela_processos):
        try:
            index = tabela_processos.index(min(filter(lambda processo: processo.estado == 'PRONTO', tabela_processos), key=lambda processo:processo.prioridade, default=None))
        except ValueError:
            index = None
        if index == None:
            for processo in tabela_processos:
                if processo.estado == 'BLOQUEADO':
                    index = 'ESPERANDOPROCESSO'
        return index

    #retorna processo da interrupcao se tiver no tempo do timer
    def checa_interrupcao(self, tabela_processos, interr, c):
        for processo in tabela_processos:
            if processo.nome == interr:
                processo.estado = 'PRONTO'
                c.retorna_interrupcao(processo.cpu_estado)

    #imprime os resultados
    def imprime_tabela(self, tabela_processos, c, timer):
        for processo in tabela_processos:
            print('Nome: ', processo.nome)
            print('Tempo de cpu: ', processo.cpu_time)
            print('Hora de inicio: ', processo.job.data)
            print('Hora de termino: ', processo.fim)
            print('Vezes que foi bloqueado: ', processo.vezes_bloqueado)
            print('Tempo bloqueado: ', processo.tempo_bloqueado)
            print('Quantia de preempcao: ', processo.preempcao)
            print('Tempo de retorno: ', processo.fim - processo.job.data)
            print('Percentual de tempo de cpu: ', (processo.cpu_time * 100 / timer))
            for i, saida in enumerate(processo.job.saida):
                print('Saída: ', i)
                for j, posicao in enumerate(saida):
                    if j != 0:
                        print('Posição[{}]: {}'.format(j, posicao))

    def atualiza_tempo_bloqueado(self, tabela_processos, timer):
        for processo in tabela_processos:
            processo.tempo_bloqueado = timer.timer_agora() - processo.cpu_time

    def executa(self, so, c, mmu, timer, dados):
        index = True
        while True:
            #Procura um processo
            index = self.escalonador(so.tabela_processos)
            if index == None:
                break
            if index != 'ESPERANDOPROCESSO' and index != None:
                print(so.tabela_processos[index].nome)
                c.altera_estado(so.tabela_processos[index].cpu_estado)
                so.altera_mmu(index, mmu)
                c.altera_programa(so.tabela_processos[index].job.programa)
                c.quantum = self.TAM_QUANTUM()
                while so.tabela_processos[index].estado == 'PRONTO':
                    c.quantum -= 1
                    if c.quantum <= 0:
                        so.tabela_processos[index].preempcao += 1
                        c.dorme()
                        timer.timer_int_de(1, so.tabela_processos[index].nome)
                        so.tabela_processos[index].estado = 'BLOQUEADO'
                        break
                    if c.interrupcao() == 'NORMAL':
                        c.executa(dados, mmu, so, index, timer)
                    if c.interrupcao() == 'INSTRUCAOILEGAL' or c.interrupcao() == 'VIOLACAODEMEMORIA':
                        so.executa(c, timer, index, self.TAM_QUANTUM())
                    #checa por interrupções
                    while True:
                        interr = timer.timer_interrupcao() 
                        if interr != None:
                            self.checa_interrupcao(so.tabela_processos, interr, c)
                        else:
                            break
                    so.tabela_processos[index].cpu_time += 1
                    timer.timer_tictac()
                so.tabela_processos[index].vezes_bloqueado += 1
                c.salva_dados(dados, mmu, so.tabela_processos[index])
                so.tabela_processos[index].cpu_estado = c.salva_estado()
            while True:
                interr = timer.timer_interrupcao() 
                if interr != None:
                    self.checa_interrupcao(so.tabela_processos, interr, c)
                else:
                    break
            c.cpu_ociosa += 1
            timer.timer_tictac()
        self.atualiza_tempo_bloqueado(so.tabela_processos, timer)
        print('Tempo de cpu ociosa: ', c.cpu_ociosa)
        print('Tempo de execucao total: ', timer.timer_agora())
        self.imprime_tabela(so.tabela_processos, c, timer.timer_agora())