#!/usr/bin/env python
#coding:utf-8
import web
from config.Router import *

web.config.debug = True

if web.config.get('_app') is None:
    app = web.application(Router(), globals())
    web.config._app = app
else:
    app = web.config._app

if web.config.get('_session') is None:
    session = web.session.Session(app, web.session.DiskStore('sessions'), initializer={'IsLogin':0})
    web.config._session = session
else:
    session = web.config._session

if web.config.get('_render') is None:
    render = web.template.render('templates/', base='TLayout', globals={'ctx':web.config._session})
else:
    render = web.config._session



