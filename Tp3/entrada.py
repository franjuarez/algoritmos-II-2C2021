import csv
from grafo import *
from biblioteca import *
from constantes import ENTRADA_ERRONEA, COMANDO_DESCONOCIDO, ORDEN_INEXISTENTE, COMANDOS_EXISTENTES

def cargar_en_memoria(path):
    grafo = Grafo(True)
    with open(path, newline='', encoding='utf8') as archivo:
        tsv_reader = csv.reader(archivo, delimiter='\t')
        for vertice in tsv_reader:
            for arista in range(len(vertice)):
                if not grafo.pertenece(vertice[arista]):
                    grafo.agregar_vertice(vertice[arista])
                if arista != 0:
                    grafo.agregar_arista(vertice[0], vertice[arista])
    return grafo

def printear_flechas(lista):
    for indice, elemento in enumerate(lista):
        if indice == len(lista)-1:
            print(elemento)
        else: print(f"{elemento} -> ", end='')

def listar_operaciones():
    operaciones = "camino\nconectados\nciclo\ndiametro\nlectura\nrango\nnavegacion\nclustering\ncomunidad" # poner diametro y arreglarlo
    print(operaciones)

def parsear_entrada(grafo, entrada, conectados, diametro):
    entrada_separada = entrada.split()
    if not entrada_separada: 
        comando = None
        params = ""
    else:
        comando, *parametros = entrada_separada
        params = ' '.join(parametros) if parametros else ""

    if not comando in COMANDOS_EXISTENTES:
        print(COMANDO_DESCONOCIDO)
        return

    elif comando == "listar_operaciones":
        listar_operaciones()

    elif comando == "camino":
        try:
            v, w = params.split(',')
        except ValueError:
            print(ENTRADA_ERRONEA)
            return
        camino = camino_mas_corto(grafo, v, w)
        if not camino:
            print("No se encontro recorrido")
        else:
            printear_flechas(camino)
            print(f"Costo: {len(camino) - 1}")
    
    elif comando == "conectados":
        if not params:
            print(ENTRADA_ERRONEA)
            return
        v = params
        CFC = buscar_CFC(grafo, v, conectados)
        cfc_cadena = ', '.join(CFC) 
        print(cfc_cadena)

    elif comando == "diametro":
        if diametro: 
            print(diametro[0])
            return
        diametro.append(diametro_grafo(grafo, diametro))
        print(diametro[0])

    elif comando == "ciclo":
        try:
            pag, n = params.split(',')
        except ValueError:
            print(ENTRADA_ERRONEA)
            return
        camino = ciclo_n(grafo, pag, int(n))
        if not camino:
            print("No se encontro recorrido")
        else: printear_flechas(camino)

    elif comando == "lectura":
        try:
            pags = params.split(',')
        except ValueError:
            print(ENTRADA_ERRONEA)
            return
        grafo_topogolico = crear_grafo_invertido(grafo, pags)
        resultado = orden_topologico(grafo_topogolico)
        if not resultado:
            print(ORDEN_INEXISTENTE)
            return
        print(', '.join(resultado))

    elif comando == "rango":
        try:
            origen, n = params.split(',')
        except ValueError:
            print(ENTRADA_ERRONEA)
            return
        print(distancia_n(grafo, origen, int(n)))

    elif comando == "navegacion":
        if not params:
            print(ENTRADA_ERRONEA)
            return
        origen = params
        camino = primer_adyacente(grafo, origen)
        if not camino:
            print("No se encontro recorrido")
        else: printear_flechas(camino)
    
    elif comando == "clustering":
        pagina = params
        coef_clustering = coeficiente_clustering(grafo, pagina)
        print('%.3f' % coef_clustering)
        
    elif comando == "comunidad":
        if not params:
            print(ENTRADA_ERRONEA)
            return
        pagina = params
        comunidad = comunidades(grafo, pagina)
        cadena_a_printear = ', '.join(comunidad)
        print(cadena_a_printear)
        print(len(comunidad))
    