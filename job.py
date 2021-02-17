class Job:
    
    entrada = []
    saida = []
    programa = []

    def __init__(self, txt):

        with open(txt, 'r') as f:

            #data de lançamento
            self.data = int(f.readline())

            #cria dispositivos de entrada
            self.qtd_entrada = int(f.readline())
            for i in range(self.qtd_entrada):
                tam_entrada = int(f.readline())
                self.entrada.append([int(f.readline()) for j in range(tam_entrada)])
            print(self.entrada)

            #cria dispositivos de saida
            self.qtd_saida = int(f.readline())
            for i in range(self.qtd_saida):
                tam_saida = int(f.readline())
                self.saida.append([int(f.readline())])
                for j in range(tam_saida):
                    self.saida[i].append(0)
            print(self.saida)
            
            #preenche memoria de dados
            for line in f:
                if 'NEG' not in line and 'PARA' not in line:
                    instr, arg = line.split()
                    arg = int(arg)
                    self.programa.append({'instr': instr, 'arg': arg})
                else:
                    instr = line
                    self.programa.append({'intr': instr})
                    i += 1