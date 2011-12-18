#!/usr/bin/env python
#coding:utf-8
import web
from config.Runtime import *
from config.Globals import *
from models.MUser import MUser
from models.MMessage import MMessage


class UserReg:
    def GET(self):
        return render.TUserReg()
    def POST(self):
        account_email          = web.input()['account_email'].strip().lower()
        account_nickname       = web.input()['account_nickname'].strip()
        account_passwd         = web.input()['account_passwd'].strip()
        account_passwd_confirm = web.input()['account_passwd_confirm'].strip()
        
        if not len(account_email) or not len(account_nickname) or not len(account_passwd) or not len(account_passwd_confirm):
            return render.TMessage(MMessage.ConstructCommonMessage(GLOBAL_MSG_ERROR, "注册失败: [所有字段都必须填写]。", [['javascript:history.go(-1)', '返回重新注册']]))
        
        if account_passwd != account_passwd_confirm:
            return render.TMessage(MMessage.ConstructCommonMessage(GLOBAL_MSG_ERROR, "注册失败: [两次密码输入不一致]。", [['javascript:history.go(-1)', '返回重新注册']]))
        
        r = MUser.AddUser(account_email, account_nickname, account_passwd)
        if r['Status'] == -1:
            return render.TMessage(MMessage.ConstructCommonMessage(GLOBAL_MSG_ERROR, "注册失败: [" + r['ErrorMsg'] + ']。', [['javascript:history.go(-1)', '返回重新注册']]))
        
        session['UserID']    = r['user_id']
        session['UserName']  = r['nickname']
        session['UserEmail'] = r['email']
        return render.TMessage(MMessage.ConstructCommonMessage(GLOBAL_MSG_NORMAL, "注册成功，系统已自动为您登陆。", [['/', '返回主页']]))
        

class UserLogin:
    def GET(self):
        return render.TUserLogin()
    def POST(self):
        account_email = web.input()['account_email'].strip().lower()
        account_passwd = web.input()['account_passwd'].strip()
        
        r = MUser.CheckUser(account_email, account_passwd)
        if not r:
            return render.TMessage("<span class='msg-error'>账号或密码不正确。</span><br /><a href='javascript:history.go(-1)'>返回重新登陆</a>")

        session['UserID']    = r.id
        session['UserName']  = r.nickname
        session['UserEmail'] = r.email

        return render.TMessage(MMessage.ConstructCommonMessage(GLOBAL_MSG_NORMAL, "您已经成功登陆。", [['/', '进入主页'], ['/post/add', '新建代码片段'], ['/user/profile/'+str(r.id), '个人资料']]))

class UserLogout:
    def GET(self):
        session.kill()
        return render.TMessage(MMessage.ConstructCommonMessage(GLOBAL_MSG_NORMAL, "您已经成功退出。", [['/', '返回主页'], ['/user/login', '重新登陆'], ['/user/reg', '注册新账号']]))


class UserProfile:
    def GET(self, user_id):
        user = {}
        return render.TUserProfile(user)
