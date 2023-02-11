import csv
fileA = input("File Arduino: ")
fileS = input("File Stratomini: ")
fileM = input("File merge: ")
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
