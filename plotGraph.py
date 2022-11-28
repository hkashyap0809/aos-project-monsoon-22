import matplotlib.pyplot as plot
import numpy as np

file1 = open("hitRate.txt","r")
print("Output of Readlines after appending") 
hitRateSTR = file1.readlines()
file1.close()
hitRate = hitRateSTR[0].split(',')
hitRate.pop(len(hitRate)-1)

for i in range(len(hitRate)):
    hitRate[i]= float(hitRate[i])

print(hitRate)
xarr = [1,2,3,4,5,6,7,8,9,10,11]

xpoints = np.array(xarr)
ypoints = np.array(hitRate)
algos=['RAND','OPT','NRU','FIFO','FIFO 2C','CLK','LRU','NFU','WS','AGNG' ,'WS CLK']
plot.xticks(xpoints,algos)
plot.plot(xpoints,ypoints,marker = 'o')

plot.xlabel('Algorithms')
plot.ylabel('Hit Rates')
plot.title('Page Replacement Algorithms Comparison')



plot.show()
