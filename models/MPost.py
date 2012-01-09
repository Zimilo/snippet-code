#!/usr/bin/env pytho
#coding:utf-8
from util.Util import Base62
from config.Runtime import *
from models.MLanguage import MLanguage
from models.MUser import MUser
import time

class Post:

    @staticmethod
    def QueryDB(post_id = None, short_lnk = None):
        if (post_id == None and short_lnk == None) or (post_id != None and short_lnk != None):
            return False

        vars_cond = {}
        if post_id != None:
            where_cond = "id=%d" % post_id
            
        if short_lnk != None:
            where_cond = "link=$link"
            vars_cond['link'] = short_lnk

        post = list(db.select(GLOBAL_DB_PRE + GLOBAL_DB_POSTS_TABLE,
                              where = where_cond,
                              vars = vars_cond))
        if not len(post):
            return False

        post = post[0]

        lang = MLanguage.Query(post.language_type)

        post.lang = lang[2]
        post.lang_type = lang[1]
        post.lang_id = lang[0]

        if post.user_id != -1:
            post.user = MUser.GetUserByAccountID(post.user_id)
            if not post.user:
                return False

        if not post.lang:
            return False
        
        post.publish_time_ts = post.publish_time
        post.publish_time = time.strftime("%Y-%m-%d %H:%M:%S", time.localtime(post.publish_time))
        post.last_edit_time_ts = post.last_edit_time
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



        post_count = db.query("SELECT COUNT(*) AS TotalCount FROM " + GLOBAL_DB_PRE + GLOBAL_DB_POSTS_TABLE + " WHERE user_id = %d" % user_id)

        return (posts, post_count[0].TotalCount)


    @staticmethod
    def GetPosts(params):
        posts = list(db.select(GLOBAL_DB_PRE + GLOBAL_DB_POSTS_TABLE,
                               where  = params['where'],
                               order  = params['order'],
                               offset = params['offset'],
                               limit  = params['count']))

        posts = list(db.query("select P.id,P.link,P.user_id,P.priviledge,P.language_type,P.title,P.content,P.publish_time,P.last_edit_time,U.email,U.nickname from %s as P left join %s U on P.user_id = U.id AND %s ORDER BY %s LIMIT %d, %d" 
                                % (GLOBAL_DB_PRE + GLOBAL_DB_POSTS_TABLE, GLOBAL_DB_PRE + GLOBAL_DB_USERS_TABLE, 
                                    params['where'], params['order'], params['offset'],params['count'])
                            ))

        for post in posts:
            post.lang = MLanguage.Query(post.language_type)[2]
            post.publish_time = time.strftime("%Y-%m-%d %H:%M:%S", time.localtime(post.publish_time))
            post.last_edit_time = time.strftime("%Y-%m-%d %H:%M:%S", time.localtime(post.last_edit_time))

        post_count = db.query("SELECT COUNT(*) AS TotalCount FROM " + GLOBAL_DB_PRE + GLOBAL_DB_POSTS_TABLE + " WHERE " + params['where'])

        return (posts, post_count[0].TotalCount)



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


    @staticmethod
    def UpdateDB(post):
        r = {}
        r['Status'] = -1
        r['ErrorMsg'] = ""

        if not post:
            r['ErrorMsg'] = "Post对象不正确"
            return r
        
        db.update(GLOBAL_DB_PRE + GLOBAL_DB_POSTS_TABLE, 
                            priviledge     = post['priviledge'], 
                            language_type  = post['language_type'],
                            title          = post['title'],
                            content        = post['content'],
                            last_edit_time = post['last_edit_time'],
                            where = "link=$link",
                            vars = {'link':post['link']})


        r['Status'] = 0
        return r


    @staticmethod
    def CheckPostOwner(code_link, user_id):
        post = Post.QueryDB(short_lnk = code_link)
        if post.user_id != user_id:
            return False
        return True


    @staticmethod
    def DeleteFromDBByPostShortLink(code_link):
        r = {}
        r['Status'] = -1
        r['ErrorMsg'] = ""

        db.delete(GLOBAL_DB_PRE + GLOBAL_DB_POSTS_TABLE, 
                  where="link=$link",
                  vars = {'link' : code_link})

        r['Status'] = 0
        return r
