import numpy as np
import pandas as pd

#导入csv文件
frame = pd.read_csv("heroDone.csv", names=['hero', 'win', 'position','userid'])

#按id分组计算出各个id的胜率
groups = frame['win'].groupby(frame['userid'])

#计算出来后，得到排名前10的选手
everyone = groups.mean().sort_values(ascending=False)
everyone.to_csv('everyone_rate.csv',index=True,header=False)

bestTen = everyone.head(10)
pd.to_csv('bestTen.csv',index=True,header=False)