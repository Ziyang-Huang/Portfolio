# For WOT

import requests
# import json
import time


class Player:
    def __init__(self, name, area, zone):
        self.name = name
        self.area = area
        self.zone = zone
        self.time_str = str(int(time.time()))
        self.date_str = time.strftime("%Y%m%d")
        self.index_url = "http://rank.kongzhong.com/Data/action/WotAction/getIndex?name=" + self.name + "&area=" + \
                         self.area + "&zone=" + self.zone + "&ref=blog&_=" + self.time_str
        self.tanks_url = ""
        self.id = 0
        self.index_str = ""
        self.index = {}
        self.tanks_str = ""
        self.tanks = {}

    def get_index_str(self):
        r = requests.get(self.index_url)
        r.encoding = 'utf-8'
        text = r.text
        for i in range(len(text)):
            if text[i] == ',':
                if text[i + 1] != '\n':
                    self.index_str = sort_json_text(text)
                    break
                else:
                    self.index_str = text
                    break

    def save_index_str(self):
        filename = self.name + '_' + self.area + self.date_str + '_index.txt'
        try:
            f = open(filename, 'w', encoding='utf-8')
        except:
            print("Error when open file")
        else:
            f.write(self.index_str)
            f.close()

    def load_index_str(self, date=time.strftime("%Y%m%d")):
        filename = self.name + '_' + self.area + date + '_index.txt'
        try:
            f = open(filename, 'r', encoding='utf-8')
        except FileNotFoundError:
            print("No such file or directory:", filename)
        else:
            self.index_str = f.read()
            f.close()

    def get_tanks(self):
        self.tanks_url = "http://rank.kongzhong.com/Data/action/WotAction/getTank?aid=" + str(self.id) + \
                         "&_" + self.time_str


def sort_json_text(json_text):
    result = ""
    brace_count = 0
    for i in json_text:
        if i in ['{', '[', '(']:
            brace_count += 1
            result += i
            result += '\n'
            for j in range(brace_count):
                result += '\t'
        elif i == ',':
            result += i
            result += '\n'
            for j in range(brace_count):
                result += '\t'
        elif i in ['}', ']', ')']:
            brace_count -= 1
            result += '\n'
            for j in range(brace_count):
                result += '\t'
            result += i
        else:
            result += i
    return result

