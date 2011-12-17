#!/usr/bin/env python
#coding:utf-8

from config.Runtime import *
from models.MLanguage import MLanguage

class Index:
    def GET(self):
        return render.TIndex(MLanguage.GetAllLangs())
