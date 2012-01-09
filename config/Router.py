#!/usr/bin/env python
#coding:utf-8

def Router():
    urls = [
        ['/index/(.*)',      'CIndex.Index'],
        ['/',                 'CIndex.Index'],

        ['/user/reg',         'CUser.UserReg'],
        ['/user/login',       'CUser.UserLogin'],
        ['/user/logout',      'CUser.UserLogout'],
        ['/user/profile/(.*)','CUser.UserProfile'],
        ['/post/add',         'CPost.PostAdd'],
        ['/post/edit/(.*)',   'CPost.PostEdit'],
        ['/post/del/(.*)',    'CPost.PostDel'],
        ['/post/like/(.*)',   'CPost.PostLike'],
        
        ['/post/my',          'CPost.PostList'], #设置默认列表分页
        ['/post/my/(.*)',    'CPost.PostList'],

        ['/(.*).gif', 'CPost.PostGenImage'],
        ['/post/view/(.*)',   'CPost.PostView'],

        ['/(.*).js', 'CPost.PostJSEmbed'], #嵌入代码支持

        ['/online_dev', 'CLab.Lab'], #在线编程

        ['/comment/post', 'CComment.CommentPost'],
        
        ['/about', 'CAbout.About'],
                
        ['/F_(.*)', 'CPost.PostEmbed'],

        ['/(.*)', 'CPost.PostShortLnkViewer'], #短链接

        ]

    route_urls = []

    for url in urls:
        route_urls.extend([url[0], 'controllers.' + url[1]])

    return tuple(route_urls)

