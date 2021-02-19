from cpu import Cpu
from mmu import Mmu
from sys import argv
from dados import *
from controlador import *
from so import So
from timer import Timer

def MEMORIA():
    return 51

def PAGINA():
    return 3

argv.append('matrizcoluna.txt')
argv.append('matrizlinha.txt')
argv.append('matrizcoluna.txt')
argv.append('matrizlinha.txt')
argv.append('matrizcoluna.txt')
argv.append('matrizlinha.txt')
argv.append('matrizcoluna.txt')
if len(argv) <= 1:
    print("Usage: pyton main.py programa1.txt programa2.txt...")
    exit()

c = Cpu()
mmu = Mmu(int(MEMORIA()/PAGINA()), PAGINA())
dados = Dados(MEMORIA(), PAGINA())
controlador = Controlador()
timer = Timer()
so = So(timer, argv, dados)
controlador.inicia(c, argv, dados, mmu, so, timer)