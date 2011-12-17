#!/usr/bin/env python
#coding:utf-8
from config.Runtime import *
from config.Globals import *
from models import MPost
from models.MLanguage import MLanguage
import time

class PostAdd:
    def GET(self):
        return render.TIndex(MLanguage.GetAllLangs())

    def POST(self):
        code_title = web.input()['code_title'].strip()
        code_priviliedge = int(web.input()['code_priviledge'].strip())
        code_content = web.input()['code_content'].strip()
        code_language_type = int(web.input()['code_language_type'].strip())
        
        if session['UserID'] == -1 and code_priviliedge == GLOBAL_PRIVILIEDGE_PRIVATE:
            return render.TMessage("<span class='msg-error'>匿名用户不允许将代码设置为仅自己可见状态</span><br /><a href='javascript:history.go(-1);'>返回之前的页面</a>")

        if not len(code_content):
            return render.TMessage("<span class='msg-error'>外面PM2.5这么严重, 宅在家里至少写点代码再提交吧</span><br /><a href='javascript:history.go(-1);'>返回之前的页面</a>")

        if not MLanguage.IsSupportLang(code_language_type):
            return render.TMessage("<span class='msg-error'>未知的语言类型</span><br /><a href='javascript:history.go(-1);'>返回之前的页面</a>")

        if not len(code_title):
            code_title = GLOBAL_DEFAULT_POST_TITLE
        

        
        post = {
            'user_id':session['UserID'],
            'priviliedge':code_priviliedge,
            'language_type':code_language_type,
            'title':code_title,
            'content':code_content,
            'publish_time':int(time.time()),
            'last_edit_time':int(time.time())
            }

        return post


class PostView:
    def GET(self, post_id):
        try:
            pid = int(post_id)
        except:
            return render.TMessage("<span class='msg-error'>参数传递异常</span><br /> <a href='/'>返回主页</a>")

        post = MPost.Post(post_id = pid)
        post.QueryDB()
        if post.GetLastError():
            return render.TMessage("<span class='msg-error'>失败:"+post.GetLastError()['description']+"</span> <a href='/'>返回主页</a>")

        #检查查看的权限
        
        post = {
            'id': pid,
            'link': '1ACAFt',
            'user': {
                'id':-1,
                'name':'Guest'
                },
            'language_type':'Python',
            'title': 'Python Base62微薄短链接的实现',
            'priviledge': '1',
            'content': 'class Base62:\ndef __init__(self):\npass',
            'publish_time': '2011-12-17 22:22:00',
            'last_edit_time': '2011-12-17 22:24:00'
            }

        return render.TPostView(post)



