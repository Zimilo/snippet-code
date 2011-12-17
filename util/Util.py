#!/usr/bin/env python
#coding:utf-8

from math import floor
from math import log10
from math import pow

#@see http://www.code-trick.com/base62-php/
class Base62:
    def __init__(self):
        self.magic = "vPh7zZwA2LyU4bGq5tcVfIMxJi6XaSoK9CNp0OWljYTHQ8REnmu31BrdgeDkFs"
    def Encode(self, num):
        out = ''
        L = range(int(floor(log10(num)/log10(62)))+1)
        L.reverse()
        for t in L:
            a = int(floor(num / pow(62, t)))
            out += self.magic[a:a+1]
            num = num - (a * pow(62, t))
        return out
    def Decode(self, decode_str):
        out = 0
        str_len = len(decode_str) - 1
        for t in range(str_len + 1):
            out = out + self.magic.find(decode_str[t:t+1]) * pow(62, str_len - t)
        return int(out)


if __name__ == "__main__":
    b = Base62()
    link = b.Encode(209000000016)
    print link
    print b.Decode(link)
