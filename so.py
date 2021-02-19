from job import *
from cpu import Cpu_estado
from mmu import Mmu
from dados import Dados
from copy import *

class Processo:

    #cria o processo
    def __init__(self, txt, timer, dados, j):
        #cria o job
        self.job = Job(txt)
        
        #nome do processo
        self.nome = txt + str(j)
        
        #prepara o timer
        if self.job.data > 0:
            self.estado = 'BLOQUEADO'
            timer.timer_int_as(self.job.data, self.nome)
        else:
            self.estado = 'PRONTO'
        
        #prepara o estado da cpu
        self.cpu_estado = Cpu_estado()


        #copia da mmu
        self.copia_mmu = Mmu(int(dados.mem_size/dados.page_size), dados.page_size)
        
        #memoria secundaria /copia da memoria principal
        self.mem_secundaria = Dados(dados.mem_size, dados.page_size)
        self.index_saida = 1
        self.cpu_time = 0
        self.fim = 0
        self.tempo_em_es = 0
        self.tempo_em_troca_pagina = 0
        self.vezes_bloqueado = 0
        self.prioridade = 0.5
        self.preempcao = 0
        self.falha_pagina = 0
        self.vezes_escalonado = 0


class So:

    #sistema operacional cria tabela
    def __init__(self, timer, argv, dados):
        self.tabela_processos = []
        j = 0
        del argv[0]
        for i in argv:
            self.tabela_processos.append(Processo(i, timer, dados, j))     
            j += 1

    #inicia o s.o
    def inicia(self, controlador, c, argv, dados, mmu, timer):
        controlador.executa(self, c, mmu, timer, dados)
    
    #coloca a copia da mmu no processo na mmu
    def altera_mmu(self, processo, mmu):
        mmu.tabela_pagina = deepcopy(self.tabela_processos[processo].copia_mmu.tabela_pagina)
    
    #pega uma pagina da memoria //passar memoria secundaria por parametro
    def get_pagina(self, i, c, timer, processo):
        c.dorme()
        return self.tabela_processos[processo].mem_secundaria.getPage(i)

    #coloca uma pagina na memoria
    #pagina = pagina a ser colocada na memoria principal
    def set_pagina(self, data, pagina, index, mmu, descritor, c, timer, fifo):

        self.tabela_processos[index].falha_pagina += 1
        
        #verifica se tem uma pagina livre
        paginas_ocupadas = [False for i in range(mmu.tam_tabela)]
        for processo in self.tabela_processos:
            for page in processo.copia_mmu.tabela_pagina:
                if page.n_pagina != None:
                    paginas_ocupadas[page.n_pagina] = True
        
        
        for i, value in enumerate(paginas_ocupadas):
            #encontrou pagina livre
            if value == False:
                #pagina ja existe na memoria secundaria
                if pagina != None:
                    data.data[(i * data.page_size):((i * data.page_size) + data.page_size)] = pagina
                    mmu.tabela_pagina[descritor].validade = 'VALIDO'
                    mmu.tabela_pagina[descritor].alterado = 'NAOALTERADO'
                    mmu.tabela_pagina[descritor].n_pagina = int(i)
                    c.dorme()
                    timer.timer_int_de(10, self.tabela_processos[index].nome)
                    self.tabela_processos[index].tempo_em_troca_pagina += 10
                    return
                #pagina nao existe na memoria secundaria
                else:
                    mmu.tabela_pagina[descritor].validade = 'VALIDO'
                    mmu.tabela_pagina[descritor].alterado = 'NAOALTERADO'
                    mmu.tabela_pagina[descritor].n_pagina = int(i)
                    c.dorme()
                    timer.timer_int_de(10, self.tabela_processos[index].nome)
                    self.tabela_processos[index].tempo_em_troca_pagina += 10
                    return
        
        
        #caso nao encontrar pagina livre substitui de acordo com o algoritmo de substituicao
        if fifo == True:
            for processo in self.tabela_processos:
                for page in processo.copia_mmu.tabela_pagina:
                    if page.n_pagina == data.next_page_out:
                        #pagina ja existe na memoria secundaria
                        if pagina != None:
                            data.data[(data.next_page_out * data.page_size):((data.next_page_out * data.page_size) + data.page_size)] = deepcopy(pagina)
                            page.alterado == 'ALTERADO'
                            mmu.tabela_pagina[descritor].validade = 'VALIDO'
                            mmu.tabela_pagina[descritor].alterado = 'NAOALTERADO'
                            mmu.tabela_pagina[descritor].n_pagina = int(data.next_page_out)
                            data.next_page_out += 1
                            if data.next_page_out >= mmu.tam_tabela:
                                data.next_page_out = 0
                            c.dorme()
                            timer.timer_int_de(10, self.tabela_processos[index].nome)
                            self.tabela_processos[index].tempo_em_troca_pagina += 10
                            return
                        #pagina nao existe na memoria secundaria
                        else:
                            page.alterado = 'ALTERADO'
                            mmu.tabela_pagina[descritor].validade = 'VALIDO'
                            mmu.tabela_pagina[descritor].alterado = 'NAOALTERADO'
                            mmu.tabela_pagina[descritor].n_pagina = int(data.next_page_out)
                            data.next_page_out += 1
                            c.dorme()
                            timer.timer_int_de(10, self.tabela_processos[index].nome)
                            self.tabela_processos[index].tempo_em_troca_pagina += 10
                            if data.next_page_out >= mmu.tam_tabela:
                                data.next_page_out = 0
                            return
        
        if fifo == False:
            while True:
                for processo in self.tabela_processos:
                    for page in processo.copia_mmu.tabela_pagina:
                        if page.n_pagina == data.next_page_out:
                            if page.acessado == 'ACESSADO':
                                page.acessado = 'NAOACESSADO'
                                data.next_page_out += 1
                                if data.next_page_out >= mmu.tam_tabela:
                                    data.next_page_out = 0
                            else:    
                                #pagina ja existe na memoria secundaria
                                if pagina != None:
                                    data.data[(data.next_page_out * data.page_size):((data.next_page_out * data.page_size) + data.page_size)] = deepcopy(pagina)
                                    page.alterado == 'ALTERADO'
                                    mmu.tabela_pagina[descritor].validade = 'VALIDO'
                                    mmu.tabela_pagina[descritor].alterado = 'NAOALTERADO'
                                    mmu.tabela_pagina[descritor].n_pagina = int(data.next_page_out)
                                    data.next_page_out += 1
                                    if data.next_page_out >= mmu.tam_tabela:
                                        data.next_page_out = 0
                                    c.dorme()
                                    timer.timer_int_de(10, self.tabela_processos[index].nome)
                                    self.tabela_processos[index].tempo_em_troca_pagina += 10
                                    return
                                #pagina nao existe na memoria secundaria
                                else:
                                    page.alterado = 'ALTERADO'
                                    mmu.tabela_pagina[descritor].validade = 'VALIDO'
                                    mmu.tabela_pagina[descritor].alterado = 'NAOALTERADO'
                                    mmu.tabela_pagina[descritor].n_pagina = int(data.next_page_out)
                                    data.next_page_out += 1
                                    c.dorme()
                                    timer.timer_int_de(10, self.tabela_processos[index].nome)
                                    self.tabela_processos[index].tempo_em_troca_pagina += 10
                                    if data.next_page_out >= mmu.tam_tabela:
                                        data.next_page_out = 0
                                    return

    
    def executa(self, c, timer, processo, tam_quantum):

        if c.reg.status == 'INSTRUCAOILEGAL':

            #Se a instrucao for PARA, coloca o processo como TERMINADO
            if c._pm[c.reg._pc]['instr'] == 'PARA':
                self.tabela_processos[processo].fim = timer.timer_agora()
                self.tabela_processos[processo].estado = 'TERMINADO'
            
            #Se for le, le um numero de um dispositivo de entrada
            elif c._pm[c.reg._pc]['instr'] == 'LE':
                self.tabela_processos[processo].estado = 'BLOQUEADO'
                self.tabela_processos[processo].prioridade -= (self.tabela_processos[processo].prioridade * c.quantum / tam_quantum)
                self.tabela_processos[processo].cpu_estado = c.salva_estado()
                #Altera o acumulador com o proximo valor da entrada
                c.estado_altera_acumulador(self.tabela_processos[processo].cpu_estado, 
                    self.tabela_processos[processo].job.entrada[c._pm[c.reg._pc]['arg']].pop(1))
                c.altera_estado(self.tabela_processos[processo].cpu_estado)
                #Configura o timer de acordo com o tempo de acesso ao dispositivo de entrada correspondente
                timer.timer_int_de(self.tabela_processos[processo].job.entrada[c._pm[c.reg._pc]['arg']][0], self.tabela_processos[processo].nome)
                self.tabela_processos[processo].tempo_em_es += self.tabela_processos[processo].job.entrada[c._pm[c.reg._pc]['arg']][0]
                c.dorme()
                c.reg._pc += 1
            
            #Se for grava, grava um numero em um dispositivo de saida
            elif c._pm[c.reg._pc]['instr'] == 'GRAVA':
                self.tabela_processos[processo].estado = 'BLOQUEADO'
                self.tabela_processos[processo].prioridade -= (self.tabela_processos[processo].prioridade * c.quantum / tam_quantum)
                self.tabela_processos[processo].cpu_estado = c.salva_estado()
                self.tabela_processos[processo].job.saida[c._pm[c.reg._pc]['arg']][self.tabela_processos[processo].index_saida] = c.estado_acumulador(self.tabela_processos[processo].cpu_estado)
                self.tabela_processos[processo].index_saida += 1
                c.altera_estado(self.tabela_processos[processo].cpu_estado)
                timer.timer_int_de(self.tabela_processos[processo].job.saida[c._pm[c.reg._pc]['arg']][0], self.tabela_processos[processo].nome)
                self.tabela_processos[processo].tempo_em_es += self.tabela_processos[processo].job.saida[c._pm[c.reg._pc]['arg']][0]
                c.dorme()
                c.reg._pc += 1
            else:
                exit("Intrucao Ilegal Detectada.")