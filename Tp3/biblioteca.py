from random import shuffle
from grafo import * #Grafo
from TDAS import Cola, Pila
from heapq import *
from constantes import COEF_DE_AMORTIGUACION, LIMITE_ITERACIONES, TOPE_NAVEGACION, MAXIMAS_ITERACIONES, CONVERGENCIA_COMUNIDADES

def _construir_camino(padres, origen, destino):
    camino = []
    papa = destino
    while papa != origen:
        camino.append(papa)
        papa = padres[papa]
    
    camino.append(origen)
    return camino[::-1]

def camino_mas_corto(grafo, origen, destino): 
    '''
    Encuentra el camino más corto entre dos vértices especificados
    por parámetro y lo devuelve. En caso de no existir un camino 
    devuelve None.
    '''
    visitados = set()
    padres = {}
    padres[origen] = None
    visitados.add(origen)
    cola = Cola()
    cola.encolar(origen)
    while not cola.esta_vacia():
        v = cola.desencolar()
        for w in grafo.adyacentes(v):
            if not w in visitados:
                padres[w] = v
                visitados.add(w)
                cola.encolar(w)
    if destino in padres and origen in padres:
        return _construir_camino(padres, origen, destino)
    return []

def buscar_CFC(grafo, v, diccionario_conectados):
    '''
    Se encarga de buscar una componente fuertemente conexa que
    comience en el vértice especificado por parámetro y devolver
    una lista con los miembros de la CFC.
    '''
    if v in diccionario_conectados: return diccionario_conectados[v]
    apilados = set()
    pila = Pila()
    visitados = set()
    orden = {}
    mb = {}
    cfc = _CFC(grafo, v, apilados, pila, visitados, orden, mb)
    diccionario_conectados[v] = cfc #NO es CFC tmb de las componentes? Osea de cada vertice? 
    return cfc

contador = 0
def _CFC(grafo, v, apilados, pila, visitados, orden, mb):
    global contador 
    pila.apilar(v)
    visitados.add(v)
    apilados.add(v) #hace fala el apilados?????
    orden[v] = contador 
    mb[v] = contador
    contador += 1
    for w in grafo.adyacentes(v):
        if w not in visitados:
            _CFC(grafo, w, apilados, pila, visitados, orden, mb)
        
        if w in apilados and mb[w] < mb[v]:
            mb[v] = mb[w]

    if mb[v] == orden[v] and len(apilados) > 0:
        nueva_cfc = []
        while True:
            e = pila.desapilar()
            apilados.remove(e)
            nueva_cfc.append(e)
            if v == e:
                break
        return nueva_cfc

def diametro_grafo(grafo, diametro):
    '''
    Calcula el diámetro total del grafo. Esto significa devolver
    la longitud del camino mínimo más largo existente en el grafo.
    '''
    if diametro: return diametro
    orden_max = (0, {}, "", "")
    for v in grafo:
        orden_actual = _diametro_grafo(grafo, v)
        if orden_actual[0] > orden_max[0]:
            orden_max = orden_actual
    camino = _construir_camino(orden_max[1], orden_max[2], orden_max[3])
    longitud = len(camino)
    diametro_camino = ' -> '.join(camino)
    string_final = f'{diametro_camino}\nCosto: {longitud - 1}'
    return string_final

def _diametro_grafo(grafo, origen):
    visitados = set()
    orden = {}
    orden_max = 0
    padres = {}
    padres[origen] = None
    visitados.add(origen)
    orden[origen] = 0
    cola = Cola() 
    cola.encolar(origen)
    while not cola.esta_vacia():
        v = cola.desencolar()
        for w in grafo.adyacentes(v):
            if not w in visitados:
                padres[w] = v
                visitados.add(w)
                orden[w] = orden[v] + 1
                if orden[w] > orden_max: orden_max = orden[w]
                cola.encolar(w)
    return orden_max, padres, origen, v

def _es_compatible(grafo, lista, n, v, visitados):
    if v in visitados: return False
    if len(lista) == n:
        if lista[0] in grafo.adyacentes(v):
            return True
        return False
    return True

def _ciclo(grafo, v_act, lista, n, visitados):
    if len(lista) == n: return True
    
    for w in grafo.adyacentes(v_act):
        lista.append(w)
        if _es_compatible(grafo, lista, n, w, visitados):
            visitados.add(w)
            if _ciclo(grafo, w, lista, n, visitados): return lista
            visitados.remove(w)
        lista.pop() 
    
    return False


def ciclo_n(grafo, vertice, n):
    '''
    Busca y devuelve un ciclo de largo n comenzando en el
    vértice indicado por parámetro.
    '''
    if n < 1: return [vertice]
    lista = _ciclo(grafo, vertice, [vertice], n, set([vertice]))
    if lista:
        lista.append(lista[0])
        return lista
    else: return []

def _grados_entrada(grafo):
    grados = {}
    for v in grafo:
        grados[v] = grados.get(v,0)
        for w in grafo.adyacentes(v):
            grados[w] = grados.get(w, 0) + 1
    return grados

def crear_grafo_invertido(grafo_antiguo, vertices):
    '''
    A partir de un grafo pasado por parámetro crea uno
    nuevo que invierta las aristas del grafo y devuelve
    uno nuevo con estas carácterísticas.
    '''
    grafo = Grafo(True, vertices)
    for v in vertices:
        for w in vertices:
            if w == v: continue
            if grafo_antiguo.estan_unidos(v,w):
                grafo.agregar_arista(w, v)
    return grafo

def orden_topologico(grafo):
    '''
    Busca un órden topológico de los existentes
    para el grafo y lo devuelve.
    En caso de haber un ciclo devolverá None
    '''
    g_ent = _grados_entrada(grafo) 
    q = Cola()
    for v in grafo: 
        if g_ent[v] == 0:
            q.encolar(v)
    resultado = []
    while not q.esta_vacia():
        v = q.desencolar()
        resultado.append(v)
        for w in grafo.adyacentes(v):
            g_ent[w] -= 1
            if g_ent[w] == 0:
                q.encolar(w)
    
    if len(resultado) != len(grafo):
        return None
    return resultado

def vertices_entrada(grafo):
    '''
    Devuelve un diccionario con todos los vértices
    del grafo como claves y los vértices de entrada
    respectivos como valores.
    '''
    grados = {}
    for v in grafo:
        for w in grafo.adyacentes(v):
            grados[w] = grados.get(w, [])
            grados[w].append(v)
    for v in grafo:
        if v not in grados:
            grados[v] = []
    return grados

def vertices_salida(grafo):
    '''
    Devuelve un diccionario con todos los vértices
    del grafo como claves y los vértices de salida
    respectivos como valores.
    '''
    v_salida = {}
    for vertice in grafo:
        v_salida[vertice] = grafo.adyacentes(vertice)
    return v_salida
    
def _obtener_n_mas_imporantes(diccionario_pageranks, n):
    heap = []
    num = 0
    for pagina, pgrnk in diccionario_pageranks.items():
        if num < n:
            heappush(heap, (pgrnk, pagina))
            num += 1
            continue
        if pgrnk > heap[0][0]:
           heappushpop(heap, (pgrnk, pagina)) 
        
    n_mejor_rankeados = nlargest(n, heap)
    n_rankeados = []
    for elemento in n_mejor_rankeados:
        n_rankeados.append(elemento[1])
        
    return n_rankeados

def distancia_n(grafo, origen, n):
    '''
    Obtiene y devuelve todos los vértices a exacta
    distancia n, desde un origen proporcionado.
    '''
    visitados = set()
    cantidad_a_n = 0
    distancia = {} 
    distancia[origen] = 0
    visitados.add(origen)
    cola = Cola()
    cola.encolar(origen)
    while not cola.esta_vacia():
        v = cola.desencolar()
        for w in grafo.adyacentes(v):
            if not w in visitados:
                visitados.add(w)
                distancia[w] = distancia[v] + 1
                if distancia[w] == n:
                    cantidad_a_n += 1
                elif distancia[w] > n:
                    break
                cola.encolar(w)
    return cantidad_a_n

def _primer_adyacente(grafo, v_act, tope, camino):
    adyacentes = grafo.adyacentes(v_act)
    if not adyacentes or len(camino) > TOPE_NAVEGACION:
        return camino
    camino.append(adyacentes[0])
    return _primer_adyacente(grafo, adyacentes[0], tope, camino)

def primer_adyacente(grafo, origen):
    '''
    Devuelve un camino avanzando únicamente por 
    el primer vértice adyacente encontrado hasta 
    llegar a un tope de 20 vértices recorridos.
    '''
    return _primer_adyacente(grafo, origen, TOPE_NAVEGACION + 1,[origen])

def _clustering_individual(grafo, v):
    adyacentes_v = set(grafo.adyacentes(v))
    if v in adyacentes_v: adyacentes_v.remove(v)
    cantidad_adyacentes = len(adyacentes_v)
    denominador = cantidad_adyacentes * (cantidad_adyacentes - 1)
    if denominador == 0: return 0
    contador_transitividad = 0
    for w in adyacentes_v:
        for j in grafo.adyacentes(w):
            if w == j: continue
            if j in adyacentes_v:
                contador_transitividad += 1
    coeficiente_clustering_v = contador_transitividad / denominador
    return coeficiente_clustering_v

def coeficiente_clustering(grafo, v = None):
    '''
    Calcula el Coeficiente de Clustering de un vértice dado
    por parámetro, en caso de recibir None calculará
    el promedio de todos los Coeficientes del grafo.
    '''
    if v:
        return _clustering_individual(grafo, v)
    else:
        suma_contador = 0
        cantidad_vertices_grafo = len(grafo)
        for v in grafo:
           suma_contador += _clustering_individual(grafo, v)
        return suma_contador / cantidad_vertices_grafo

def _max_freq(labels, lista):
    dic = {}
    for v in lista:
        dic[labels[v]] = dic.get(labels[v], 0) + 1
    return max(dic, key= dic.get)

def _label_propagation(orden, labels, v_entrada):
    for vertice in orden:
        labels[vertice] = _max_freq(labels, v_entrada[vertice])

def comunidades(grafo, pagina):
    '''
    Obtiene la comunidad de una página pasada por parámetro
    utilizando el algoritmo de Label Propagation y la devuelve.
    '''
    labels = {}
    vertices_grafo = grafo.obtener_vertices()
    for i, v in enumerate(grafo):
        labels[v] = i
    v_entrada = vertices_entrada(grafo)
    shuffle(vertices_grafo)
    for _ in range(MAXIMAS_ITERACIONES):
        _label_propagation(vertices_grafo, labels, v_entrada)
    label_buscado = labels[pagina]
    resultado = []
    for vertice, label in labels.items():
        if label == label_buscado and vertice != v:
            resultado.append(vertice)
    return resultado






