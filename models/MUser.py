#!/usr/bin/env python
#coding:utf-8

import hashlib
from config.Runtime import *

class MUser:
    
    @staticmethod
    def AddUser(account_email, account_nickname, account_passwd):
        r = {}
        r['Status'] = -1
        r['ErrorMsg'] = ''
        
        users = list(db.select(GLOBAL_DB_PRE + GLOBAL_DB_USERS_TABLE,
                               where="email=$email",
                               vars = {'email':account_email}
                               ))
        if len(users):
            r['ErrorMsg'] = '该Email已经存在'
            return r
        
        r['user_id'] = db.insert(GLOBAL_DB_PRE + GLOBAL_DB_USERS_TABLE,
                                 email=account_email, 
                                 nickname = account_nickname, 
                                 password = hashlib.md5(account_passwd).hexdigest())

        r['nickname'] = account_nickname
        r['email']    = account_email
        r['Status']   = 0
        
        return r
        
    
    @staticmethod
    def CheckUser(account_email, account_passwd):
        if not len(account_email) or not len(account_passwd):
            return False
        
        user = list(db.select(GLOBAL_DB_PRE + GLOBAL_DB_USERS_TABLE,
                              where="email=$email AND password=$password",
                              vars = {"email":account_email, "password":hashlib.md5(account_passwd).hexdigest()}))
        
        if not len(user):
            return False

        return user[0]

    @staticmethod
    def GetUser(user_id):
        user = list(db.select(GLOBAL_DB_PRE + GLOBAL_DB_USERS_TABLE,
                             where="id=%d" % user_id))
        if not user:
            return False

        return user[0]
