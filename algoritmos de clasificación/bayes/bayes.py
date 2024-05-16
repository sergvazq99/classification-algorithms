import numpy as np

DIMENSIONES = 4

clases = set()

fichero = open("clases.txt")
for linea in fichero: 
    if(linea.strip() != 'FIN-CLASES'):
        clases.add(linea.strip())

mapa_str = {}
mapa_matr = {}

for clase in clases:
    #Inicializar la lista de ejemplos de cada clase
    mapa_str[clase] = ""
    

clase_actual = ""

fichero_ejemplos = open("entrada.txt")
for line in fichero_ejemplos:
    if line.strip() in clases:
        #Pasamos a leer los casos de clase_actual
        clase_actual = line.strip()
    else:
        mapa_str[clase_actual] += line.strip()
        mapa_str[clase_actual] += " "


for clase, ejemplos in mapa_str.items():
    mapa_matr[clase] = np.matrix(ejemplos.removesuffix("; "))
    
    #print("Matriz de ejemplos para la clase " + clase)
    #print(mapa_matr[clase])

#Calcular m para cada clase
medias = {}
for clase, matriz_ejemplos in mapa_matr.items():
    medias[clase] = matriz_ejemplos.mean(0)
    #print(medias[clase])
    

Ci = {}

#Calcular las matrices C inversas
for clase, matriz_ejemplos in mapa_matr.items():
    #Inicializar C[clase] a una matriz de todo 0
    ac = ""
    for i in range(DIMENSIONES):
        for j in range(DIMENSIONES):
            ac += "0."
            if(j < DIMENSIONES - 1):
                ac += ", "
            #Si estamos al final de fila y no al final de la matriz
            elif (i < DIMENSIONES - 1):
                ac += "; "
                
    Ci[clase] = np.matrix(ac)
        
    
    num_ejemplos = 0
    for ejemplo in matriz_ejemplos:
        #Restar ejemplo - m
        resta = ejemplo - medias[clase]
        resultado = resta.transpose() * resta
        
        Ci[clase] = Ci[clase] + resultado
        num_ejemplos += 1
    
    #Dividir entre el numero de ejemplos
    Ci[clase] = Ci[clase] / num_ejemplos

    #Inversa
    Ci[clase] = Ci[clase].getI()

    #print(Ci[clase])


#Valorar casos
fichero_casos = open("casos.txt")
for caso in fichero_casos:
    primero = True
    menor = 0
    clase_menor = ""
    
    caso_mtr = np.matrix(caso.strip())

    for clase in clases:
        resta = caso_mtr - medias[clase] 
        resta_t = resta.transpose()
        #print(resta_t)

        #Hacer resta * Ci * resta_t
        resultado = resta * Ci[clase]
        resultado = resultado * resta_t

        #Es una matriz con un unico elemento
        resultado = resultado.item(0)

        if(primero):
            primero = False
            menor = resultado
            clase_menor = clase
        elif(resultado < menor):
            menor = resultado
            clase_menor = clase

    print('Caso: ' + caso.strip())
    print("Clase: " + clase_menor)

