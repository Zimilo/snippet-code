#!/usr/bin/env python
#coding:utf-8
import hashlib
import time
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
            r['ErrorMsg'] = '该Email已经存在了'
            return r

        
        users = list(db.select(GLOBAL_DB_PRE + GLOBAL_DB_USERS_TABLE,
                               where="nickname=$nickname",
                               vars={"nickname":account_nickname}
                               ))

        if len(users):
            r['ErrorMsg'] = '该昵称已经存在了,换个昵称吧'
            return r

        r['user_id'] = db.insert(GLOBAL_DB_PRE + GLOBAL_DB_USERS_TABLE,
                                 email=account_email, 
                                 nickname = account_nickname, 
                                 password = hashlib.md5(account_passwd).hexdigest(),
                                 reg_time = int(time.time()))

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
    def GetUserByAccountID(account_id):
        user = list(db.select(GLOBAL_DB_PRE + GLOBAL_DB_USERS_TABLE,
                              where="id=$id",
                              vars={'id':account_id}))
        if not user:
            return False

        return user[0]
    

    @staticmethod
    def GetUser(account_nickname):
        user = list(db.select(GLOBAL_DB_PRE + GLOBAL_DB_USERS_TABLE,
                              where="nickname=$nickname",
                              vars={'nickname':account_nickname}))
        if not user:
            return False

        return user[0]


    @staticmethod
    def VerifyPasswd(account_id, account_current_passwd):
        user = list(db.select(GLOBAL_DB_PRE + GLOBAL_DB_USERS_TABLE,
                              where="id=$id AND password=$passwd",
                              vars={'id':account_id, 'passwd':hashlib.md5(account_current_passwd).hexdigest()}))
        if not user:
            return False

        return True
        
        
    @staticmethod
    def UpdateUserPasswd(account_id, account_passwd):
        db.update(GLOBAL_DB_PRE + GLOBAL_DB_USERS_TABLE,
                  password=hashlib.md5(account_passwd).hexdigest(),
                  where="id=$id",
                  vars={'id':account_id})

        return True
