# Crawler.py
# 面向对象版本
import requests
import json
import time
import pandas as pd


class Crawler:
    def __init__(self, item, bounds, api_key):
        self.item = item
        self.bounds = bounds
        self.api_key = api_key
        self.urls = []
        self.create_urls()
        self.counter = 0

    def create_urls(self):
        for pages in range(0, 20):
            url = 'https://api.map.baidu.com/place/v2/search?query=' + self.item + '&bounds=' + self.bounds + '&page_size=20&page_num=' + str(pages) + '&output=json&ak=' + self.api_key
            self.urls.append(url)

    def crawl(self):
        try:
            for i, url in enumerate(self.urls):
                print(i, url)
                time.sleep(1)
                json_obj = requests.get(url).text
                print(json_obj)
                data = json.loads(json_obj)
                if data["status"] == 401:
                    print("当前并发量已经超过约定并发配额，限制访问")
                    break
                if data['total'] != 0:
                    for item in data['results']:
                        self.counter += 1
                        json_sel = dict()
                        json_sel['h1'] = h1
                        json_sel['h2'] = h2
                        json_sel['name'] = item["name"]
                        json_sel['latitude'] = item["location"]["lat"]
                        json_sel['longitude'] = item["location"]["lng"]
                        yield json_sel
                else:
                    print("此页及以后没有数据")
                    break
            else:
                if self.counter == 400:
                    print("可能存在未爬取数据")
        except:
            print("Error when crawling")
            with open('./ErrorLog.txt', 'a', encoding='utf-8') as fl:
                fl.write(url + '\n')


class LocaDiv:
    def __init__(self, loc_all, divd):
        self.loc_all = loc_all
        self.divd = divd

    def lat_all(self):
        lat_sw = float(self.loc_all.split(',')[0])
        lat_ne = float(self.loc_all.split(',')[2])
        lat_list = [str(lat_ne)]
        while lat_ne - lat_sw >= 0:
            m = lat_ne - self.divd
            lat_ne = lat_ne - self.divd
            lat_list.append('%.2f' % m)
        return sorted(lat_list)

    def lng_all(self):
        lng_sw = float(self.loc_all.split(',')[1])
        lng_ne = float(self.loc_all.split(',')[3])
        lng_list = [str(lng_ne)]
        while lng_ne - lng_sw >= 0:
            m = lng_ne - self.divd
            lng_ne = lng_ne - self.divd
            lng_list.append('%.2f' % m)
        return sorted(lng_list)

    def ls_com(self):
        l1 = self.lat_all()
        l2 = self.lng_all()
        ab_list = []
        for i in range(0, len(l1)):
            a = str(l1[i])
            for i2 in range(0, len(l2)):
                b = str(l2[i2])
                ab = a + ',' + b
                ab_list.append(ab)
        return ab_list

    def ls_row(self):
        l1 = self.lat_all()
        l2 = self.lng_all()
        ls_com_v = self.ls_com()
        ls = []
        for n in range(0, len(l1) - 1):
            for i in range(len(l2) * n, len(l2) * (n + 1) - 1):
                a = ls_com_v[i]
                b = ls_com_v[i + len(l2) + 1]
                ab = a + ',' + b
                ls.append(ab)
        return ls


if __name__ == '__main__':
    pois = {'商业': ['酒店'],
            # '公共服务': ['生活服务', '丽人', '运动健身', '生活服务', '留学中介机构', '培训机构', '汽车销售', '汽车维修', '汽车美容', '汽车配件', '汽车租赁', '汽车检测场'],, '购物', '金融'
            # '行政机构': ['政府机构'],
            # '停车场': ['停车场'],
            # '旅游景点': ['旅游景点'],
            # '工业': ['公司企业'],
            # '餐饮': ['美食'],
            # '娱乐': ['休闲娱乐'],
            # '教育': ['高等院校', '中学', '小学', '幼儿园', '成人教育', '亲子教育', '特殊教育学校'],
            # '医疗': ['医疗'],
            # '文化设施': ['图书馆', '科技馆', '新闻出版', '广播电视', '艺术团体', '美术馆', '展览馆', '文化宫'],
            # '商务办公': ['科研机构', '写字楼'],
            # '交通服务设施': ['飞机场', '火车站', '地铁站', '长途汽车站', '公交车站', '港口', '加油加气站', '服务区', '收费站', '桥', '充电站', '路侧停车位']
            }
    baidu_api_key = ''
    # boundary = '31.61,118.46,32.29,119.24'
    loc = LocaDiv('31.61,118.46,32.29,119.24', 0.02)  # 爬取区域坐标、子区域长宽
    locs_to_use = loc.ls_row()
    print("开始爬取数据，请稍等……")
    start_time = time.time()
    for h1, v in pois.items():
        for h2 in v:
            print('开始爬取 {}-{} 数据'.format(h1, h2))
            for location in locs_to_use:
                par = Crawler(h2, location, baidu_api_key)
                dt = par.crawl()
                df = pd.DataFrame(dt)
                if len(df) != 0:
                    print(df)
                    df.to_csv('南京' + h2 + '.csv', sep=',', header=True, index=True, encoding='ANSI', mode='a')
    end_time = time.time()
    print("数据爬取完毕，用时%.2f秒" % (end_time - start_time))

