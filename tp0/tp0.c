#include "tp0.h"

/* *****************************************************************
 *                     FUNCIONES A COMPLETAR                       *
 *         (ver en tp0.h la documentación de cada función)         *
 * *****************************************************************/

void swap(int *x, int *y) {
    int aux = *x;
    *x = *y;
    *y = aux;    
}

int maximo(int vector[], int n) {
    if (n < 1){return -1;}
    int valor_max = vector[0];
    int pos_max = 0;
    for(int i = 1; i < n; i++){
        if (vector[i] > valor_max){
            pos_max = i;
            valor_max = vector[i];
        }
    }
    return pos_max;
}

int comparar(int vector1[], int n1, int vector2[], int n2) {
    for(int i = 0; i <n1 && i<n2; i++){
        if (vector1[i] < vector2[i]){
            return -1;
        }
        else if (vector1[i] > vector2[i]){
            return 1;
        }
    }
    if (n2>n1){return -1;}
    else if (n1>n2){return 1;}
    return 0;
}


void seleccion(int vector[], int n) {
    for(int i = 0; n > 0;i++){
        int pos_max = maximo(vector, n);
        swap(&vector[pos_max], &vector[n-1]);
        n--;
    }
}
