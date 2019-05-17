import numpy as np
import pandas as pd

#导入csv文件
frame1 = pd.read_csv("heroDone.csv", names=['hero', 'win', 'position','userid'])

#按id分组计算出各个id的胜率
groups = frame1['win'].groupby(frame1['userid'])
#计算出来后，得到排名前10的选手,输出为一个csv文件
mostTen = groups.mean().sort_values(ascending=False).head(10)
#把userid导入到ids[]数组中
mostTen.index
#画一个图表，柱形图，将前10的胜率给做出来

#按上面的userid筛选出一个新的表，只有这10个id
#print(frame1.userid[98])

#groupby是可以按行来进行分类汇总的。
#frame1['win'].groupby(frame1.userid == (mostTen.index[1]))
print(mostTen.index)
#按照id进行筛选,并且重新设定index,
frame2 = frame1[frame1.userid.isin(mostTen.index)].sort_values(by='userid',ascending=True)

#将frame2输出到CSV文件

#这10个人分别打什么位置最好
frame3 = frame2['win'].groupby([frame2.userid,frame2.position]).mean()

frame3.to_csv("whowhere1.csv",index=True,header=False)

type(frame3)#可知这个其实是一个series，必须要把series变为dataframe结构才行

#得出了一个表
frame4 = frame3.groupby(level=0,group_keys=False).nlargest(5)

#找到每个人最适合的位置
frame5 = frame3.groupby(level=0,group_keys=False).nlargest(1)
print(frame5)
frame5.to_csv("whowhere2.csv",index=True,header=False)
