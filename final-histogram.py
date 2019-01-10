# Plotting Realtime histogram for the Output Data
# Code by: Akash Lad for Pollen detection
# Prerequisites:
# 1. matplotlib & numpy installed
# 2. data from OPCN3 in opcn3.txt file


import matplotlib.pyplot as plt           #function for plotting graph(histogram)
import numpy as np
import matplotlib
import os
import time
from datetime import datetime


text_file = open("opcn3.txt", "r")      #Reading Data from the Parent file 
#print(text_file)
lines = text_file.readlines()           #Reading Data of line or linr by line
#print(lines)
TT = lines
#f = open("temp.txt","w")
text_file.close()

def testing(array):
    f = open("temp.txt","w")          #Temporary file creation
    array_new=array.replace("\n","")    #Removal of \n from the end of line
    #print(array_new)
    s3 = array_new.split(',')           #Taking the Data after the first ',' (Required Data)
    timename = s3[0]
    s3.remove(s3[0])
    for s in s3:
        f.write(s)
        f.write("\n")
        
    f.close()                           #file Operation close file
    #return 1
    f= np.loadtxt('temp.txt', delimiter=',', unpack= True)     #function for reading the tepmporary file
        
    bins = [0,50,100,150,200]                                    #creation of intervals (Bins)
    plt.hist(f, histtype = 'bar', bins = bins, rwidth = 1, color = 'c')
    #plt.ion()
    plt.xlabel('x values')
    plt.ylabel('y values')
    plt.title('OPC_N3 Histogram'+' ' + timename)
    outputfile = datetime.now().strftime("%Y%m%d-%H%M%S")
    
    
    plt.show()
    plt.draw()
    plt.savefig('image_' + outputfile + '.png')
    #plt.close()
    #plt.pause(0.1)
    os.remove("temp.txt")
    return 1


#print (len(lines))

tmp = 0
while tmp<len(TT):
    #Run C codes first
    os.system("./tcu_flowrate")
    #os.system("./opcn3")
    time.sleep(2)

    #print(TT[tmp])
    testing(TT[tmp])   
    tmp=tmp+1
