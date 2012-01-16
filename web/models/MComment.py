#!/usr/bin/env pytho
#coding:utf-8
from util.Util import Base62
from config.Runtime import *
from models.MLanguage import MLanguage
from models.MUser import MUser
import time



class Comment:
    @staticmethod
    def Insert2DB(comment):
        r = {}
        r['Status'] = -1
        r['ErrorMsg'] = ""

        if not comment:
            r['ErrorMsg'] = "Comment对象不正确"
            return r
        
        comment_id = db.insert(GLOBAL_DB_PRE + GLOBAL_DB_COMMENTS_TABLE, 
                            post_id = comment["post_id"],
                            user_name        = comment['user_name'], 
                            content  = comment['comment_content'], 
                            publish_time     = comment['publish_time'])

        r['Status'] = 0
        r['comment_id'] = comment_id
        return r	


    @staticmethod
    def QueryDB(post_id):
        if not post_id:
            return False
        
        comments = list(db.select(GLOBAL_DB_PRE + GLOBAL_DB_COMMENTS_TABLE,
                        where = "post_id=$post_id",
                        order = "publish_time DESC",
                        vars = {'post_id':post_id}))
        
        for comment in comments:
            comment.publish_time =  time.strftime("%Y-%m-%d %H:%M:%S", time.localtime(comment.publish_time))

        return comments
