# ProcessScheduler
operating systems class activity

Um programa deve possuir o seguinte formato:
1 linha = data de lançamento
2 linha = tamanho da memória de dados necessária
3 linha = quantos dispositivos de entrada
4 linha = informa o tamanho do primeiro dispositivo de entrada // lembrando que o tamanho mínimo é dois pois o primeiro valor
5 linha = informa o tempo de aceso ao dispositivo de entrada 
linhas subsquentes = valores do primeiro dispositivo de entrada
proxima linha = se ouver mais de um dispositivo de entrada repete a 4 linha e as linhas subsequentes com os novos valores
proxima linha = informa quantos dispositivos de saida
proxima linha = informa o tamanho do primeiro dispositivo de saida // lembrando que o tamanho mínimo é dois pois o primeiro 
proxima linha = informa o tempo de acesso ao dispositivo de saida
linhas subsequentes = programa 


2 // data de lançamento
2 // tamanho da memória de dados necessária
1 // quantos dispositivos de entrada
5 // informa o tamanho do primeiro dispositivo de entrada (minimo 2)
3 // informa o tempo de acesso ao dispositivo de entrada
5 // valores
2 //
1 //
4 //
1 // quantos dispositivos de saida
2 // tamanho do primeiro disp de saida (minimo 2)
5 // tempo de acesso ao primeiro dispositivo de saida
CARGI 0 // programa
ARMM  0
LE    0
ARMM  1
CARGI 1
NEG
ARMM  2
CARGM 0
SOMA  1
ARMM  0
CARGM 1
SOMA  2
ARMM  1
DESVZ 16
CARGI 0
DESVZ 7
CARGM 0
GRAVA 1
PARA