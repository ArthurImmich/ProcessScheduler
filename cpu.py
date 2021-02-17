class Cpu_estado:
    _pc = 0
    _acc = 0
    status = 'NORMAL'

class Cpu:
    reg = Cpu_estado()
    _pm = []

    def dorme(self):
        self.status = 'DORMINDO'
    
    def altera_programa(self, m):
        self._pm = m

    def salva_dados(self, memoria, mmu, processo):
        processo.copia_mmu.tabela_pagina = mmu.tabela_pagina
        processo.mem_secundaria.data = memoria.data
        return processo
        
    
    def retorna_interrupcao(self, e):
        if e.status != 'NORMAL':
            e.status = 'NORMAL'
    
    def intrucao(self):
        return self._pm[self.reg._pc] if len(self._pm) > self.reg._pc else "Invalida"

    def salva_estado(self):
        return self.reg
    
    def altera_estado(self, e):
        self.reg = e
    
    def estado_altera_acumulador(self, e, novo_valor):
        e._acc = novo_valor
    
    def estado_acumulador(self, e):
        return e._acc

    # def cpu_executa(self):
