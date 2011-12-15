#!/usr/bin/env python
#coding:utf-8

from controllers import *

def Router():
    urls = [['/', 'CIndex.Index'],
            ['/user/reg', 'CUser.Reg'],
            ['/user/login', 'CUser.Login'],
            ['/user/logout', 'CUser.Logout'],
            ['/(*)', 'CPager.CPager']
            ]

    route_urls = []

    for url in urls:
        route_urls.extend([url[0], 'controllers.' + url[1]])

    return tuple(route_urls)

