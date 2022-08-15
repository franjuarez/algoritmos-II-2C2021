#!/usr/bin/python3
from entrada import *
import sys

sys.setrecursionlimit(100000)


def main():
    path = sys.argv[1]
    diametro = []
    conectados = {}
    grafo = cargar_en_memoria(path)
    while True:
        try:
            entrada = input()
        except EOFError:
            break
        parsear_entrada(grafo, entrada, conectados, diametro)


main()

