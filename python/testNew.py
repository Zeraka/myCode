import sys
class NewStr(str):
    def __new__(cls,string):
        string = string.upper()
        return super().__new__(cls,string)
a = NewStr("i love china!")
print(a)

class nuStr(str):#继承了str，应该也是可以直接打印出来的
    def __init__(self,string):
        string2 = string.upper() #该方法没有返回值


b = nuStr("i love china!")
print(b)# 这里的结果就不可改变了。因此__new__是非常有用的