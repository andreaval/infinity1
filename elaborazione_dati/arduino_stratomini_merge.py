'''
Script Python che esegue il merge dei due file CSV raccolti dalla sonda
creando un unico file CSV
@author Infinity1 Team - Liceo "E. Medi" Montegiorgio (FM)
@date 2022-11-20 Last review
'''
import csv
fileA = input("CSV dati Arduino: ")
fileS = input("CSV dati Stratomini: ")
fileM = input("CSV di output: ")
with open(fileM,'w',newline="") as csvM:
    writer = csv.writer(csvM,delimiter=';',quotechar='"',quoting=csv.QUOTE_MINIMAL)
    with open(fileS) as csvS:
        with open(fileA) as csvA:
            righeS = csv.reader(csvS, delimiter=';')
            righeA = csv.reader(csvA, delimiter=',')
            #salto intestazioni
            next(righeS)
            next(righeS)
            next(righeA)
            #
            iS=8
            for rigaS in righeS:
                # 
                if iS%16==0 and not(iS>=3020 and iS<=3091) and not(iS>=3420 and iS<=3457):
                    rigaA = ['']*8
                else:
                    rigaA = next(righeA)
                writer.writerow([iS,rigaS[2],rigaS[6],rigaS[7],rigaS[9],rigaS[10],rigaS[11],rigaS[12]]+rigaA)
                print(iS)
                iS+=1
