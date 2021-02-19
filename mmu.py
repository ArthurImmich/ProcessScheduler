class Pagina:
    def __init__(self):
        self.validade = 'INVALIDO'
        self.acessado = 'ACESSADO'
        self.alterado = 'ALTERADO'
        self.n_pagina = None

class Mmu:

    def __init__(self, tam_tabela, tam_pagina):
        self.tabela_pagina = []
        self.tam_tabela = tam_tabela
        self.tam_pagina = tam_pagina
        for i in range(self.tam_tabela):
            self.tabela_pagina.append(Pagina())
        
    def fifo(self):
        return True
    
    def mmuGetData(self, i, so, processo, c, timer, dados):

        #Pega o indice do descritor correspondete na tabela de pagina
        descritor = int(i / self.tam_pagina)
        #pega a posicao do dado na pagina
        posicao = int(i % self.tam_pagina)

        #Pagina invalida 
        if self.tabela_pagina[descritor].validade == 'INVALIDO':
            c.reg.status = "PAGINAINVALIDA"
            return None
        
        #Pagina nao esta na memoria principal
        if self.tabela_pagina[descritor].alterado == 'ALTERADO':
            pagina = so.get_pagina(self.tabela_pagina[descritor].n_pagina, c, timer, processo)
            so.set_pagina(dados, pagina, processo, self, descritor, c, timer, self.fifo())
        value = dados.getData((self.tabela_pagina[descritor].n_pagina * self.tam_pagina) + posicao)
        #Se o valor nao existir
        if value == None:
            c.reg.status = 'ACESSOINVALIDO'
            return None
        else:
            self.tabela_pagina[descritor].acessado = 'ACESSADO'
            return value

    def mmuSetData(self, i, dados, value, so, timer, processo, c):

        #Pega o indice do descritor correspondete na tabela de pagina
        descritor = int(i / self.tam_pagina)
        #pega a posicao do dado na pagina
        posicao = int(i % self.tam_pagina)
        
        #Pagina valida
        if self.tabela_pagina[descritor].validade == 'VALIDO':
            #Pagina esta na memoria principal
            if self.tabela_pagina[descritor].alterado == 'NAOALTERADO':
                dados.setData(((self.tabela_pagina[descritor].n_pagina * self.tam_pagina) + posicao), value)
            #Pagina nao esta na memoria principal
            else:
                pagina = so.get_pagina(self.tabela_pagina[descritor].n_pagina, c, timer, processo)
                so.set_pagina(dados, pagina, processo, self, descritor, c, timer, self.fifo())
                self.tabela_pagina[descritor].alterado = 'NAOALTERADO' 
                dados.setData(((self.tabela_pagina[descritor].n_pagina * self.tam_pagina) + posicao), value)
        #Pagina invalida
        else:
            so.set_pagina(dados, None, processo, self, descritor, c, timer, self.fifo())
            dados.setData(((self.tabela_pagina[descritor].n_pagina * self.tam_pagina) + posicao), value)