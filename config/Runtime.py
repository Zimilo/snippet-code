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
    session = web.session.Session(app, web.session.DiskStore('sessions'), initializer={'UserID':-1, 'UserName':'Guest'})
    web.config._session = session
else:
    session = web.config._session

if web.config.get('_render') is None:
    render = web.template.render('templates/', base='TLayout', globals={'ctx':web.config._session, 'client':web.ctx})
    web.config._render = render
else:
    render = web.config._render


def NotFound():
    return web.notfound(str(render.TMessage("<span class='msg-error'>帝都雾气太严重啦，找不到页面啦!</span><br /><a href='/'>返回主页</a>")))

app.notfound = NotFound
