class Pagina:
    validade = 'INVALIDO'
    acessado = 'ACESSADO'
    alterado = 'ALTERADO'
    n_pagina = None

class Mmu:

    tabela_pagina = []

    def __init__(self, tam_tabela, tam_pagina):
        self.tam_tabela = tam_tabela
        self.tam_pagina = tam_pagina
        for i in range(tam_tabela):
            self.tabela_pagina.append(Pagina())
    
    def mmuGetData(self, i, so, processo, c, timer, dados):

        #Pega o indice do descritor correspondete na tabela de pagina
        descritor = i / self.tam_pagina
        #pega a posicao do dado na pagina
        posicao = i % self.tam_pagina

        #Pagina invalida 
        if self.tabela_pagina[descritor].validade == 'INVALIDO':
            c.reg.status = "PAGINAINVALIDA"
            return None
        
        #Pagina nao esta na memoria principal
        if self.tabela_pagina[descritor].alterado == 'ALTERADO':
            pagina = so.get_pagina(self.tabela_pagina[descritor].n_pagina, c, timer, processo)
            so.set_pagina(dados, pagina, processo, self, descritor, c, timer)
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
        descritor = i / self.tam_pagina
        #pega a posicao do dado na pagina
        posicao = i % self.tam_pagina
        
        #Pagina valida
        if self.tabela_pagina[descritor].validade == 'VALIDO':
            #Pagina esta na memoria principal
            if self.tabela_pagina[descritor].alterado == 'NAOALTERADO':
                dados.setData(((self.tabela_pagina[descritor].n_pagina * self.tam_pagina) + posicao), value)
            #Pagina nao esta na memoria principal
            else:
                pagina = so.get_pagina(self.tabela_pagina[descritor].n_pagina, c, timer, processo)
                so.set_pagina(dados, pagina, processo, self, descritor, c, timer)
                self.tabela_pagina[descritor].alterado = 'NAOALTERADO' 
                dados.setData(((self.tabela_pagina[descritor].n_pagina * self.tam_pagina) + posicao), value)
        #Pagina invalida
        else:
            so.set_pagina(dados, None, processo, self, descritor, c, timer)
            dados.setData(((self.tabela_pagina[descritor].n_pagina * self.tam_pagina) + posicao), value)



