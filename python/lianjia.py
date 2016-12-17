#!/usr/bin/python
# -*- coding: utf-8 -*-

# Reference: http://www.toutiao.com/a6359964850847744258/

import requests
import sys
import time
from bs4 import BeautifulSoup
import pandas as pd
# 导入图表库
import matplotlib.pyplot as plt
# 导入数值计算库
import numpy as np
# 导入sklearn中的KMeans进行聚类分析
from sklearn.cluster import KMeans

# 设置默认编码为 utf-8
reload(sys)
sys.setdefaultencoding('utf-8')

# 设置列表页URL的固定部分
url = 'http://bj.lianjia.com/ershoufang/'
# 设置页面页的可变部分
page = 'pg'
# 设置请求头部信息
headers = {
    'User-Agent': 'Mozilla/5.0 (Windows NT 6.1) AppleWebKit/537.11 (KHTML, like Gecko) Chrome/23.0.1271.64 Safari/537.11',
    'Accept': 'text/html;q=0.9,*/*;q=0.8',
    'Accept-Charset': 'ISO-8859-1,utf-8;q=0.7,*;q=0.3',
    'Accept-Encoding': 'gzip',
    'Connection': 'close',
    'Referer': 'http://www.baidu.com/link?url=_andhfsjjjKRgEWkj7i9cFmYYGsisrnm2A-TN3XZDQXxvGsM9k9ZZSnikW2Yds4s&wd=&eqid=c3435a7d00006bd600000003582bfd1f'
}

# 循环抓取列表页信息
for i in range(1, 2):
    if i == 1:
        i = str(i)
        a = (url + page + i + '/')
        r = requests.get(url=a, headers=headers)
        html = r.content
    else:
        i = str(i)
        a = (url + page + i + '/')
        r = requests.get(url=a, headers=headers)
        html2 = r.content
        html = html + html2
    # 每次间隔0.5秒
    time.sleep(0.5)

# 解析抓取的页面内容
lj = BeautifulSoup(html, 'html.parser')

# 提取房源总价
price = lj.find_all('div', attrs={'class': 'priceInfo'})
tp = []
for a in price:
    totalPrice = a.span.string
    tp.append(totalPrice)

# 提取房源信息
houseInfo = lj.find_all('div', attrs={'class': 'houseInfo'})
hi = []
for b in houseInfo:
    house = b.get_text()
    hi.append(house)

# 提取房源关注度
followInfo = lj.find_all('div', attrs={'class': 'followInfo'})
fi = []
for c in followInfo:
    follow = c.get_text()
    fi.append(follow)

# 创建数据表
house = pd.DataFrame({'totalprice': tp, 'houseinfo': hi, 'followinfo': fi})
# 查看数据表的内容
print (house.head())

# 对房源信息进行分列
houseinfo_split = pd.DataFrame((x.split('|') for x in house.houseinfo), index=house.index,
                               columns=['xiaoqu', 'huxing', 'mianji', 'chaoxiang', 'zhuangxiu', 'dianti'])
# 查看分列结果
print (houseinfo_split.head())

# 将分列结果拼接回原数据表，完成拼接后的数据表中既包含了原有字段，也包含了分列后的新增字段。
house = pd.merge(house, houseinfo_split, right_index=True, left_index=True)
# 查看拼接后的数据表
print (house.head())

# 对房源关注度进行分列
followinfo_split = pd.DataFrame((x.split('/') for x in house.followinfo), index=house.index,
                                columns=['guanzhu', 'daikan', 'fabu'])
# 将分列后的关注度信息拼接回原数据表
house = pd.merge(house, followinfo_split, right_index=True, left_index=True)

# 按房源户型类别进行汇总
huxing = house.groupby('huxing')['huxing'].agg(len)
# 查看户型汇总结果
print (huxing)

# 绘制房源户型分布条形图
# plt.rc('font', family='STXihei', size=11)
# a=np.array([1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20])
# plt.barh([1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20],huxing,color='#052B6C',alpha=0.8,align='center',edgecolor='white')
# plt.ylabel('户型')
# plt.xlabel('数量')
# plt.xlim(0,1300)
# plt.ylim(0,20)
# plt.title('房源户型分布情况')
# plt.legend(['数量'], loc='upper right')
# plt.grid(color='#95a5a6',linestyle='--', linewidth=1,axis='y',alpha=0.4)
# plt.yticks(a,('1室0厅','1室1厅','1室2厅','2室0厅','2室1厅','2室2厅','3室0厅','3室1厅','3室2厅','3室3厅','4室1厅','4室2厅','4室3厅','5室2厅','5室3厅','6室1厅','6室2厅','7室2厅','7室3厅'))
# plt.show()

# 对房源面积进行二次分列
mianji_num_split = pd.DataFrame((x.split('平') for x in house.mianji), index=house.index, columns=['mianji_num', 'mi'])
# 将分列后的房源面积拼接回原数据表
house = pd.merge(house, mianji_num_split, right_index=True, left_index=True)

# 去除mianji_num字段两端的空格
# house['mianji_num']=house['mianji_num'].map(str.strip)
# 更改mianji_num字段格式为float
house['mianji_num'] = house['mianji_num'].astype(float)

# 查看所有房源面积的范围值
house['mianji_num'].min(), house['mianji_num'].max()

# 对房源面积进行分组
bins = [0, 50, 100, 150, 200, 250, 300, 350]
group_mianji = ['小于50', '50-100', '100-150', '150-200', '200-250', '250-300', '300-350']
house['group_mianji'] = pd.cut(house['mianji_num'], bins, labels=group_mianji)

# 按房源面积分组对房源数量进行汇总
group_mianji = house.groupby('group_mianji')['group_mianji'].agg(len)

# 绘制房源面积分布图
plt.rc('font', family='STXihei', size=15)
a = np.array([1, 2, 3, 4, 5, 6, 7])
plt.barh([1, 2, 3, 4, 5, 6, 7], group_mianji, color='#052B6C', alpha=0.8, align='center', edgecolor='white')
plt.ylabel('面积分组')
plt.xlabel('数量')
plt.title('房源面积分布')
plt.legend(['数量'], loc='upper right')
plt.grid(color='#95a5a6', linestyle='--', linewidth=1, axis='y', alpha=0.4)
plt.yticks(a, ('小于50', '50-100', '100-150', '150-200', '200-250', '250-300', '300-350'))
plt.show()

# 对房源关注度进行二次分列
guanzhu_num_split = pd.DataFrame((x.split('人') for x in house.guanzhu), index=house.index,
                                 columns=['guanzhu_num', 'ren'])
# 将分列后的关注度数据拼接回原数据表
house = pd.merge(house, guanzhu_num_split, right_index=True, left_index=True)
# 去除房源关注度字段两端的空格
# house['guanzhu_num']=house['guanzhu_num'].map(str.strip)
# 更改房源关注度及总价字段的格式
house[['guanzhu_num', 'totalprice']] = house[['guanzhu_num', 'totalprice']].astype(float)

# 查看房源关注度的区间
house['guanzhu_num'].min(), house['guanzhu_num'].max()

# 对房源关注度进行分组
bins = [0, 100, 200, 300, 400, 500, 600, 700, 800]
group_guanzhu = ['小于100', '100-200', '200-300', '300-400', '400-500', '500-600', '600-700', '700-800']
house['group_guanzhu'] = pd.cut(house['guanzhu_num'], bins, labels=group_guanzhu)
group_guanzhu = house.groupby('group_guanzhu')['group_guanzhu'].agg(len)

# 绘制房源关注度分布图
plt.rc('font', family='STXihei', size=15)
a = np.array([1, 2, 3, 4, 5, 6, 7, 8])
plt.barh([1, 2, 3, 4, 5, 6, 7, 8], group_guanzhu, color='#052B6C', alpha=0.8, align='center', edgecolor='white')
plt.ylabel('关注度分组')
plt.xlabel('数量')
plt.xlim(0, 3000)
plt.title('房源关注度分布')
plt.legend(['数量'], loc='upper right')
plt.grid(color='#95a5a6', linestyle='--', linewidth=1, axis='y', alpha=0.4)
plt.yticks(a, ('小于100', '100-200', '200-300', '300-400', '400-500', '500-600', '600-700', '700-800'))
plt.show()

# 使用房源总价，面积和关注度三个字段进行聚类
house_type = np.array(house[['totalprice', 'mianji_num', 'guanzhu_num']])
# 设置质心数量为3
clf = KMeans(n_clusters=3)
# 计算聚类结果
clf = clf.fit(house_type)

# 查看分类结果的中心坐标
print (clf.cluster_centers_)

# 在原数据表中标注所属类别
house['label'] = clf.labels_
