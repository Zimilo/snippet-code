#!/usr/bin/env python
#coding:utf-8

def Router():
    urls = [
        ['/',                 'CIndex.Index'],
        ['/user/reg',         'CUser.UserReg'],
        ['/user/login',       'CUser.UserLogin'],
        ['/user/logout',      'CUser.UserLogout'],
        ['/user/profile/(.*)','CUser.UserProfile'],
        ['/post/add',         'CPost.PostAdd'],
        ['/post/edit/(.*)',   'CPost.PostEdit'],
        ['/post/del/(.*)',    'CPost.PostDel'],
        
        ['/post/my',          'CPost.PostList'], #设置默认列表分页
        ['/post/my/(\d+)',    'CPost.PostList'],

        ['/post/genimg/(.*)', 'CPost.PostGenImage'],
        ['/post/view/(.*)',   'CPost.PostView'],
        #['/(*)', 'CPager.CPager']
        ]

    route_urls = []

    for url in urls:
        route_urls.extend([url[0], 'controllers.' + url[1]])

    return tuple(route_urls)

