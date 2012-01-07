#!/usr/bin/env python
#coding:utf-8
import web
import sys
from config.Setting import *
from config.Runtime import *

if __name__ == '__main__':
	reload(sys)
	sys.setdefaultencoding("utf-8")
	if DEPLOY_SERVER == "FASTCGI":
		web.wsgi.runwsgi = lambda func, addr=None: web.wsgi.runfcgi(func, addr)
	app.run()
