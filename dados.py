class Dados:


    def __init__(self, size, page_size):
        self.data = []
        self.data = [None for i in range(size)]
        self.mem_size = size
        self.page_size = page_size
        self.next_page_out = 0
    
    #pega um valor
    def getData(self, i):
        return self.data[i]

    #define um valor
    def setData(self, i, value):
        self.data[i] = value
    
    #pega uma pagina
    def getPage(self, pagina):
        pagina = pagina * self.page_size
        quadro = []
        for i in range(pagina, pagina + self.page_size):
            quadro.append(self.data[i])
        return quadro

    #define uma pagina
    def setPage(self, pagina, quadro):
        j = 0
        pagina = pagina * self.page_size
        for i in range(pagina, pagina + self.page_size):
            self.data[i] = quadro[j]
            j += 1
