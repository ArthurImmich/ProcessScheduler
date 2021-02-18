from cpu import Cpu
from mmu import Mmu
from sys import argv
from dados import *
from controlador import *
from so import So
from timer import Timer

def MEMORIA():
    return 16

def PAGINA():
    return 4

if len(argv) <= 1:
    print("Usage: pyton main.py programa1.txt programa2.txt...")
    exit()

c = Cpu()
mmu = Mmu(int(MEMORIA()/PAGINA()), PAGINA())
dados = Dados(MEMORIA(), PAGINA())
controlador = Controlador()
timer = Timer()
so = So(timer, argv, dados, mmu)
controlador.inicia(c, argv, dados, mmu, so, timer)