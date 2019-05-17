import sys

file = open("hero.txt", mode = "r+")
newFile  = open("heroDone.csv",mode = "a+")#以追加模式打开文件，并且把文件指针移动到末尾
flag = 0
#txt里面的字符都是字符串形式，而没有数字，需要进行处理
ids = []
#去除双引号以及逗号
while(1):
    str = file.readline()
    #把文件里的逗号改为\t并且切分
    str = str.replace("\"", "").replace(",","\t")
    #print(str)
    #把字符串输入到新的文件中，按照userID
    strs = str.split('\t')
    id = strs[3]#把字符变为数字
   
    if(not str):
        break
file.close()