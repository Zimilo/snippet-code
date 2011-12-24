#!/usr/bin/env python
#coding:utf-8

from config.Runtime import *
from config.Globals import *
from models import MPost
from models.MLanguage import MLanguage
from models.MMessage import MMessage

class Lab:
    def GET(self):
        return render.TMessage(MMessage.ConstructCommonMessage(GLOBAL_MSG_ERROR, "该功能正在开发中，敬请期待。", [['javascript:history.go(-1)', '返回之前的页面']]))
