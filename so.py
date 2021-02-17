from job import *
from cpu import Cpu_estado
from mmu import Mmu
from dados import Dados

class Processo:

    #cria o processo
    def __init__(self, txt, timer, dados, mmu, j):
        #cria o job
        self.job = Job(txt)
        
        #prepara o timer
        if self.job.data > 0:
            self.estado = 'BLOQUEADO'
            timer.timer_int_as(self.job.data, txt)
        else:
            self.estado = 'PRONTO'
        
        #prepara o estado da cpu
        self.cpu_estado = Cpu_estado()

        #mome do processo
        self.nome = txt + j

        #copia da mmu
        self.copia_mmu = mmu
        
        #memoria secundaria /copia da memoria principal
        self.mem_secundaria = dados

        self.cpu_time = 0
        self.fim = False
        self.tempo_bloqueado = 0
        self.vezes_bloqueado = 0
        self.prioridade = 0.5
        self.preempcao = 0


class So:

    tabela_processos = []

    #sistema operacional cria tabela
    def __init__(self, timer, argv, dados, mmu):
        j = 0
        for i in argv:
            self.tabela_processos.append(Processo(i, timer, dados, mmu, j))     
            j += 1

    #inicia o s.o
    def inicia(self, controlador):
        controlador.executa()
    
    #coloca a copia da mmu no processo na mmu
    def altera_mmu(self, processo, mmu):
        for i, pagina in enumerate(mmu.tabela_pagina):
            pagina = self.tabela_processos[processo].copia_mmu.tabela_pagina[i]
    
    #pega uma pagina da memoria //passar memoria secundaria por parametro
    def get_pagina(self, i, c, timer, processo):
        c.dorme()
        timer.timer_int_de(8, self.tabela_processos[processo].nome)
        return self.tabela_processos[processo].mem_secundaria.getPage(i)

    #coloca uma pagina na memoria
    #pagina = pagina a ser colocada na memoria principal
    def set_pagina(self, data, pagina, processo, mmu, descritor, c, timer):

        #verifica se tem uma pagina livre
        paginas_ocupadas = [False for i in range(mmu.tam_tabela)]
        for processo in self.tabela_processos:
            for page in processo.copia_mmu.tabela_pagina:
                paginas_ocupadas[page.n_pagina] = True
        
        for i, value in enumerate(paginas_ocupadas):
            #encontrou pagina livre
            if value == False:
                #pagina ja existe na memoria secundaria
                if pagina != None:
                    data.data[(i * data.page_size):((i * data.page_size) + data.page_size)] = pagina
                    mmu.tabela_pagina[descritor].validade = 'VALIDO'
                    mmu.tabela_pagina[descritor].alterado = 'NAOALTERADO'
                    mmu.tabela_pagina[descritor].n_pagina = i
                    c.dorme()
                    timer.timer_int_de(10, processo.nome)
                    return
                #pagina nao existe na memoria secundaria
                else:
                    mmu.tabela_pagina[descritor].validade = 'VALIDO'
                    mmu.tabela_pagina[descritor].alterado = 'NAOALTERADO'
                    mmu.tabela_pagina[descritor].n_pagina = i
                    c.dorme()
                    timer.timer_int_de(10, processo.nome)
                    return
        
        #caso nao encontrar pagina livre substitui de acordo com o algoritmo de substituicao
        for processo in self.tabela_processos:
            for page in processo.tabela_pagina:
                if page.n_pagina == data.next_page_out:
                    #pagina ja existe na memoria secundaria
                    if pagina != None:
                        data.data[(data.next_page_out * data.page_size):((data.next_page_out * data.page_size) + data.page_size)] = pagina
                        pagina.alterado == 'ALTERADO'
                        mmu.tabela_pagina[descritor].validade = 'VALIDO'
                        mmu.tabela_pagina[descritor].alterado = 'NAOALTERADO'
                        mmu.tabela_pagina[descritor].n_pagina = data.next_page_out
                        data.next_page_out += 1
                        c.dorme()
                        timer.timer_int_de(10, processo.nome)
                        return
                    #pagina nao existe na memoria secundaria
                    else:
                        pagina.alterado = 'ALTERADO'
                        mmu.tabela_pagina[descritor].validade = 'VALIDO'
                        mmu.tabela_pagina[descritor].alterado = 'NAOALTERADO'
                        mmu.tabela_pagina[descritor].n_pagina = data.next_page_out
                        data.next_page_out += 1
                        c.dorme()
                        timer.timer_int_de(10, processo.nome)
                        if data.next_page_out >= (data.mem_size / data.page_size):
                            data.next_page_out = 0
                        return
    
    def executa(self, c, timer, processo, prioridade):
        if c.reg.status == 'INSTRUCAOILEGAL':

            #Se a instrucao for PARA, coloca o processo como TERMINADO
            if c._pm[c.reg._pc]['instr'] == 'PARA':
                self.tabela_processos[processo].fim = timer.agora()
                self.tabela_processos[processo].estado = 'TERMINADO'
            
            #Se for le, le um numero de um dispositivo de entrada
            elif c._pm[c.reg._pm]['instr'] == 'LE':
                self.tabela_processos[processo].estado = 'BLOQUEADO'
                self.tabela_processos[processo].prioridade -= prioridade
                c.salva_estado(self.tabela_processos[processo].cpu_estado)
                #Altera o acumulador com o proximo valor da entrada
                c.estado_altera_acumulador(self.tabela_processos[processo].cpu_estado, 
                    self.tabela_processos[processo].job.entrada[c._pm[c.reg._pm]['arg']].pop(1))
                c.altera_estado(self.tabela_processos[processo].cpu_estado)
                #Configura o timer de acordo com o tempo de acesso ao dispositivo de entrada correspondente
                timer.timer_int_de(self.tabela_processos[processo].job.entrada[c._pm[c.reg._pc]['arg']][0], self.tabela_processos[processo].nome)
                c.dorme()
                c.reg._pc += 1
            
            #Se for grava, grava um numero em um dispositivo de saida
            elif c._pm[c.reg._pm]['instr'] == 'GRAVA':
                self.tabela_processos[processo].estado = 'BLOQUEADO'
                self.tabela_processos[processo].prioridade -= prioridade
                c.salva_estado(self.tabela_processos[processo].cpu_estado)
                self.tabela_processos[processo].job.saida[c._pm[c.reg._pc]['arg']].append(c.estado_acumulador(self.tabela_processos[processo].estado_cpu))
                c.altera_estado(self.tabela_processos[processo].estado_cpu)
                timer.timer_int_de(self.tabela_processos[processo].job.saida[c._pm[c.reg._pc]['arg']][0], self.tabela_processos[processo].nome)
                c.dorme()
                c.reg._pc += 1
            else:
                exit("Intrucao Ilegal Detectada.")