#!/usr/bin/env python
#coding:utf-8

from config.Runtime import *
from config.Globals import *
from models import MPost
from models.MLanguage import MLanguage
from models.MMessage import MMessage

class About:
    def GET(self):
        return render.TAbout()
