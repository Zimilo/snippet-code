#!/usr/bin/env python
#coding:utf-8

from config.Runtime import *

class Index:
    def GET(self):
        return render.TIndex()
