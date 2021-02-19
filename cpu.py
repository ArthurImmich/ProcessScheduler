from copy import *

class Cpu_estado:
    def __init__(self):
        self._pc = 0
        self._acc = 0
        self.status = 'NORMAL'
class Cpu:

    def __init__(self):
        self.reg = Cpu_estado()
        self._pm = []
        self.tempo_cpu = 0
        self.cpu_ociosa = 0
        self.quantum = 0

    def dorme(self):
        self.status = 'DORMINDO'
    
    def altera_programa(self, m):
        self._pm = m

    def salva_dados(self, memoria, mmu, processo):
        processo.copia_mmu.tabela_pagina = deepcopy(mmu.tabela_pagina)
        processo.mem_secundaria.data = deepcopy(memoria.data)
    
    def retorna_interrupcao(self, e):
        if e.status != 'NORMAL':
            e.status = 'NORMAL'
    
    def instrucao(self):
        return self._pm[self.reg._pc] if len(self._pm) > self.reg._pc else "Invalida"

    def salva_estado(self):
        return deepcopy(self.reg)
    
    def altera_estado(self, e):
        self.reg = deepcopy(e)
    
    def estado_altera_acumulador(self, e, novo_valor):
        e._acc = novo_valor
    
    def estado_acumulador(self, e):
        return e._acc

    def interrupcao(self):
        return self.reg.status
    
    def executa(self, dados, mmu, so, processo, timer):

        instrucao = self._pm[self.reg._pc]['instr']
        #Caso CARGI 
        if instrucao == 'CARGI':
            self.reg._acc = self._pm[self.reg._pc]['arg']    
            self.reg._pc += 1

        #Caso CARGM
        elif instrucao == "CARGM":
            argument = self._pm[self.reg._pc]['arg']
            if dados.mem_size > argument and argument >= 0:
                if self.reg.status != 'PAGINAINVALIDA' and self.reg.status != 'ACESSOINVALIDO':
                    self.reg._acc = mmu.mmuGetData(argument, so, processo, self, timer, dados)
                    self.reg._pc += 1
            else:
                self.reg.status = 'VIOLACAODEMEMORIA'
        
        #Caso CARGX
        elif instrucao == "CARGX":
            argument = self._pm[self.reg._pc]['arg']
            position = mmu.mmuGetData(argument, so, processo, self, timer, dados)
            if dados.mem_size > position and position >= 0:
                value = mmu.mmuGetData(position, so, processo, self, timer, dados)
                if self.reg.status != 'PAGINAINVALIDA' and self.reg.status != 'ACESSOINVALIDO':
                    self.reg._acc = value
                    self.reg._pc += 1
            else:
                self.reg.status = 'VIOLACAODEMEMORIA'
        
        #Caso ARMM
        elif instrucao == "ARMM":
            argument = self._pm[self.reg._pc]['arg']
            if dados.mem_size > argument and argument >= 0:
                mmu.mmuSetData(argument, dados, self.reg._acc, so, timer, processo, self)
                print('AQUI')
                self.reg._pc += 1
            else:
                self.reg.status = 'VIOLACAODEMEMORIA'

        #Caso ARMX
        elif instrucao == "ARMX":
            argument = self._pm[self.reg._pc]['arg']
            value = mmu.mmuGetData(argument, so, processo, self, timer, dados)
            if value < dados.mem_size and value >= 0:
                if self.reg.status != 'PAGINAINVALIDA' and self.reg.status != 'ACESSOINVALIDO':
                    mmu.mmuSetData(value, dados, self.reg._acc, so, timer, processo, self)
                    self.reg._pc += 1
            else:
                self.reg.status = 'VIOLACAODEMEMORIA'

        #Caso SOMA        
        elif instrucao == "SOMA":
            argument = self._pm[self.reg._pc]['arg']
            if dados.mem_size > argument and argument >= 0:
                if self.reg.status != 'PAGINAINVALIDA' and self.reg.status != 'ACESSOINVALIDO':
                    self.reg._acc += mmu.mmuGetData(argument, so, processo, self, timer, dados)
                    self.reg._pc += 1
            else:
                self.reg.status = 'VIOLACAODEMEMORIA'
        
        #Caso NEG
        elif instrucao == "NEG":
            self.reg._acc = -self.reg._acc
            self.reg._pc += 1
        
        #Caso DESVZ
        elif instrucao == "DESVZ":
            if self.reg._acc == 0:
                self.reg._pc = self._pm[self.reg._pc]['arg']
            else:
                self.reg._pc += 1
        
        #Caso ILEGAL chama o SO depois
        else:
            self.reg.status = 'INSTRUCAOILEGAL'