#!/usr/bin/env pytho
#coding:utf-8
from util.Util import Base62
from config.Runtime import *
from models.MLanguage import MLanguage
from models.MUser import MUser
import time

class Post:

    @staticmethod
    def QueryDB(post_id):
        #post = list(db.select(GLOBAL_DB_PRE + GLOBAL_DB_POSTS_TABLE,
                         #where = "id=%d" % post_id))

                         
        post = db.query("SELECT * FROM " + GLOBAL_DB_PRE + GLOBAL_DB_POSTS_TABLE + " AS POSTS LEFT JOIN " + GLOBAL_DB_PRE + GLOBAL_DB_USERS_TABLE + " USERS ON POSTS.user_id = USERS.id WHERE POSTS.id=%d" % post_id)

        if not len(post):
            return False

        post = post[0]
        post.lang = MLanguage.Query(post.language_type)[2]

        if not post.lang:
            return False
        post.publish_time = time.strftime("%Y-%m-%d %H:%M:%S", time.localtime(post.publish_time))
        post.last_edit_time = time.strftime("%Y-%m-%d %H:%M:%S", time.localtime(post.last_edit_time))

        return post


    @staticmethod
    def GetPostList(user_id, offset, limit):
        posts = list(db.select(GLOBAL_DB_PRE + GLOBAL_DB_POSTS_TABLE,
                               where = "user_id = %d" % user_id,
                               order = "publish_time DESC",
                               offset = offset,
                               limit = limit))
        for post in posts:
            post.lang = MLanguage.Query(post.language_type)[2]
            post.publish_time = time.strftime("%Y-%m-%d %H:%M:%S", time.localtime(post.publish_time))
            post.last_edit_time = time.strftime("%Y-%m-%d %H:%M:%S", time.localtime(post.last_edit_time))
        return posts

    @staticmethod
    def Insert2DB(post):
        r = {}
        r['Status'] = -1
        r['ErrorMsg'] = ""

        if not post:
            r['ErrorMsg'] = "Post对象不正确"
            return r
        
        post_id = db.insert(GLOBAL_DB_PRE + GLOBAL_DB_POSTS_TABLE, 
                            user_id        = post['user_id'], 
                            priviledge     = post['priviledge'], 
                            language_type  = post['language_type'],
                            title          = post['title'],
                            content        = post['content'],
                            publish_time   = post['publish_time'],
                            last_edit_time = post['last_edit_time'])

        b = Base62()
        short_link = b.Encode(post_id)
        db.update(GLOBAL_DB_PRE + GLOBAL_DB_POSTS_TABLE, where="id=%d" % post_id, link=short_link)

        r['Status'] = 0
        r['post_id'] = post_id
        r['link'] = short_link
        return r
