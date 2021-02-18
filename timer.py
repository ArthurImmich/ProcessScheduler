class Interrupcao:
    def __init__(self, tipo, time):
        self.tipo = tipo
        self.time = time

class Timer:
    
    def __init__(self):
        self.count = 0
        self.termina = False
        self.interrupcoes = []

    def timer_int_de(self, time, tipo):
        self.interrupcoes.append(Interrupcao(tipo, self.count + time))
    
    def timer_int_as(self, time, tipo):
        self.interrupcoes.append(Interrupcao(tipo, time))

    def timer_tictac(self):
        self.count += 1
    
    def timer_interrupcao(self):
        try:
            return self.interrupcoes.pop(self.interrupcoes.index(next((x for x in self.interrupcoes if x.time == self.count), None))).tipo
        except ValueError:
            return None
    
    def timer_agora(self):
        return self.count