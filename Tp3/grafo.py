from random import randint, choice

VERTICE_NO_EXISTENTE = "El vértice ingresado no existe en el grafo."
VERTICES_NO_EXISTENTES = "Uno o ambos de los vértices ingresados no existen en el grafo."
VERTICE_YA_EXISTENTE = "El vertice ingresado ya existe en el grafo."
VERTICES_DISJUNTOS = "Los vértices ingresados no están unidos por una arista."
SIN_VERTICES = "El grafo no tiene vértices."

class Grafo():

    def __init__(self, es_dirigido, lista_vertices = []):
        self.es_dirigido = es_dirigido
        self.vertices = lista_vertices
        self.cant_vertices = len(lista_vertices)
        self.estructura = {}
        
        if lista_vertices:
            for vertice in lista_vertices:
                self.estructura[vertice] = {}
    
    def pertenece(self, v):
        return v in self.estructura

    def agregar_vertice(self, v):
        if self.pertenece(v):
            raise ValueError(VERTICE_YA_EXISTENTE) 
            
        self.vertices.append(v)
        self.estructura[v] = {}
        self.cant_vertices += 1

    def agregar_arista(self, v, w, peso = 1):
        if self.pertenece(v) and self.pertenece(w):
            self.estructura[v][w] = peso
            if not self.es_dirigido: 
                self.estructura[w][v] = peso
        else:
            raise ValueError(VERTICES_NO_EXISTENTES)

    def estan_unidos(self, v, w):
        if not self.pertenece(v) or not self.pertenece(w):
            raise ValueError(VERTICES_NO_EXISTENTES)
            
        w_en_v = (w in self.estructura[v])
        if self.es_dirigido:
            return w_en_v
        return v in self.estructura[w] and w_en_v

    def peso_arista(self, v, w):
        if not self.pertenece(v) or not self.pertenece(w):
            raise ValueError(VERTICES_NO_EXISTENTES)
        
        if not self.estan_unidos(v,w):
            return ValueError(VERTICES_DISJUNTOS)
        
        return self.estructura[v][w]

    def obtener_vertices(self):
        return self.vertices
    
    def vertice_aleatorio(self):
        if not self.vertices:
            return Exception(SIN_VERTICES)
        
        return choice(self.vertices)

    def adyacentes(self, v):
        if not self.pertenece(v):
            raise ValueError(VERTICE_NO_EXISTENTE)

        return list(self.estructura[v])

    def borrar_vertice(self, v):
        if not self.pertenece(v):
            raise ValueError(VERTICE_NO_EXISTENTE)

        for vertice, aristas in self.estructura.items():
            if vertice == v:
                del self.estructura[v]
                continue
            if v in aristas: 
                del aristas[v]

        self.vertices.remove(v)
        self.cant_vertices -= 1
            
    def borrar_arista(self, v, w):
        if not self.pertenece(v) or not self.pertenece(w):
            raise ValueError(VERTICES_NO_EXISTENTES)

        if not self.estan_unidos(v,w):
            return ValueError(VERTICES_DISJUNTOS)

        del self.estructura[v][w]
        if not self.es_dirigido:
            del self.estructura[w][v]

    def __len__(self):
        return self.cant_vertices

    def __iter__(self):
        return iter(self.vertices)
