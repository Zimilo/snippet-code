#!/usr/bin/env python
#coding:utf-8
import time
from config.Runtime import *
from config.Globals import *
from models import MPost
from models.MLanguage import MLanguage
from models.MMessage import MMessage
from models import MComment
import os

class PostAdd:
    def GET(self):
        return render.TIndex(MLanguage.GetAllLangs())

    def POST(self):
        interval = int(time.time()) - session['Status']['LastPublishTime']
        if interval < GLOBAL_DEFAULT_POST_PUBLISH_INTERVAL:
            return render.TMessage(MMessage.ConstructCommonMessage(GLOBAL_MSG_ERROR, "写的也忑快了吧，慢点儿提交吧，请 %d 秒之后重新尝试" % (GLOBAL_DEFAULT_POST_PUBLISH_INTERVAL - interval), [['javascript:history.go(-1)', '返回之前的页面']]))
            
        code_title = web.input()['code_title'].strip()
        code_priviledge = int(web.input()['code_priviledge'].strip())
        code_content = web.input()['code_content'].strip()
        code_language_type = int(web.input()['code_language_type'].strip())
        
        if session['UserID'] == -1 and code_priviledge == GLOBAL_PRIVILEDGE_PRIVATE:
            return render.TMessage(MMessage.ConstructCommonMessage(GLOBAL_MSG_ERROR, "匿名用户不允许将代码设置为仅自己可见状态", [['javascript:history.go(-1)', '返回之前的页面']]))

        if not len(code_content):
            return render.TMessage(MMessage.ConstructCommonMessage(GLOBAL_MSG_ERROR, "外面PM2.5这么严重, 宅在家里至少写点代码再提交吧", [['javascript:history.go(-1)', '返回之前的页面']]))

        if not MLanguage.IsSupportLang(code_language_type):
            return render.TMessage(MMessage.ConstructCommonMessage(GLOBAL_MSG_ERROR, "未知的语言类型", [['javascript:history.go(-1)', '返回之前的页面']]))

        if not len(code_title):
            code_title = GLOBAL_DEFAULT_POST_TITLE

        post = {
            'user_id':session['UserID'],
            'priviledge':code_priviledge,
            'language_type':code_language_type,
            'title':code_title,
            'content':code_content,
            'publish_time':int(time.time()),
            'last_edit_time':int(time.time())
            }

        r = MPost.Post.Insert2DB(post)
        if r['Status'] == -1:
            return render.TMessage(MMessage.ConstructCommonMessage(GLOBAL_MSG_ERROR, "操作失败: [" + r['ErrorMsg'] + "]。", [['javascript:history.go(-1)', '返回之前的页面']]))

        #更新一些状态
        session['Status']['LastPublishTime'] = int(time.time())

        short_url = "http://snippet-code.com/" + r['link']
        msg = "<div style='margin-top:15px;'>快使用该链接与大家分享代码吧<input type='text' onclick='javascript:this.select()' class='shorturl-input' value='" + short_url + "' /></div>"
        msg += "<div style='margin-top:5px;'>"
        msg += "还可以执行以下操作:" 
        msg += "</div>"
        msg += "<div style='margin-top:5px;'>"
        msg += "<a class='button-a' href='/post/view/"+ str(r['post_id'])  + "'>查看</a>"
        msg += "<a class='button-a' href='/"+ str(r['link'])  + ".gif'>生成图片</a>"
        if session['UserID'] != -1:
            msg += "<a class='button-a' href='/post/edit/"+ str(r['post_id'])  + "'>编辑</a>"
            msg += "<a class='button-a' href='/post/del/"+ str(r['post_id'])  + "'>删除</a>"
        msg += "<a class='button-a' href='/post/add' target='_blank'>新建代码片段</a>"
        msg += "<a class='button-a' href='/post/my'>我的代码片段</a>"
        msg += "</div>"

        return render.TMessage("<span class='msg-success'>发布成功啦.</span><br />" + msg)


class PostView:
    def GET(self, post_id):
        try:
            pid = int(post_id)
        except:
            return render.TMessage("<span class='msg-error'>参数传递异常</span><br /><br /><a href='/'>返回主页</a>")

        post = MPost.Post.QueryDB(post_id = pid)

        if not post:
            return render.TMessage("<span class='msg-error'>失败: [查看该代码片段发生异常]</span><br /><a href='/'>返回主页</a>")

        #检查查看的权限
        if post['priviledge'] == GLOBAL_PRIVILEDGE_PRIVATE:
            if session['UserID'] != post['user_id']:
                return render.TMessage("<span class='msg-error'>对不起，您没有权限查看此代码片段</span><br /><a href='/'>返回主页</a>")
        


        comments = MComment.Comment.QueryDB(post_id=pid)

        if not comments:
            comments = []


        return render.TPostView(post, comments)


class PostList:
    def GET(self, page_idx = 1):
        if session["UserID"] == -1:
            web.seeother("/user/login")

        page_idx = int(page_idx)

        if page_idx <= 0:
            return render.TMessage(MMessage.ConstructCommonMessage(GLOBAL_MSG_ERROR, "参数传递错误", [['javascript:history.go(-1)', '返回']]))

        (posts, total_count) = MPost.Post.GetPostList(session["UserID"], page_idx - 1, GLOBAL_POST_LIST_PAGE_SIZE)
        max_page_idx = (total_count + GLOBAL_POST_LIST_PAGE_SIZE - 1) / GLOBAL_POST_LIST_PAGE_SIZE

        if max_page_idx > 0 and page_idx > max_page_idx:
            return render.TMessage(MMessage.ConstructCommonMessage(GLOBAL_MSG_ERROR, "参数传递错误", [['javascript:history.go(-1)', '返回']]))

        return render.TPostList(posts, page_idx, max_page_idx)


class PostEdit:
    def GET(self, post_id):
        if session['UserID'] == -1:
            web.seeother("/user/login")
        try:
            pid = int(post_id)
        except:
            return render.TMessage("<span class='msg-error'>参数传递异常</span><br /><br /><a href='/'>返回主页</a>")

        post = MPost.Post.QueryDB(post_id = pid)

        if post.user.id != session['UserID']:
            return render.TMessage(MMessage.ConstructCommonMessage(GLOBAL_MSG_ERROR, "您没有权限修改该代码片段", [['javascript:history.go(-1)', '返回']]))

        if not post:
            return render.TMessage("<span class='msg-error'>失败: [查看该代码片段发生异常]</span><br /><a href='/'>返回主页</a>")

        #检查查看的权限
        if post['priviledge'] == GLOBAL_PRIVILEDGE_PRIVATE:
            if session['UserID'] != post['user_id']:
                return render.TMessage("<span class='msg-error'>对不起，您没有权限编辑此代码片段</span><br /><a href='/'>返回主页</a>")
        
        return render.TPostEditView(post, MLanguage.GetAllLangs())
        
    def POST(self, c_id):
        
        if session['UserID'] == -1:
            web.seeother("/user/login")
        
        code_id = web.input()['code_id'].strip()
        code_title = web.input()['code_title'].strip()
        code_priviledge = int(web.input()['code_priviledge'].strip())
        code_content = web.input()['code_content'].strip()
        code_language_type = int(web.input()['code_language_type'].strip())

        if code_id != c_id:
            return render.TMessage("<span class='msg-error'>参数传递异常</span><br /><br /><a href='/'>返回主页</a>")

        try:
            code_id = int(code_id)
        except:
            return render.TMessage("<span class='msg-error'>参数传递异常</span><br /><br /><a href='/'>返回主页</a>")

        #先检查该Post是否为当前用户所有

        if not MPost.Post.CheckPostOwner(code_id, session['UserID']):
            return render.TMessage(MMessage.ConstructCommonMessage(GLOBAL_MSG_ERROR, "你没有权限修改该文章", [['javascript:history.go(-1)', '返回之前的页面']]))
        

        if not len(code_content):
            return render.TMessage(MMessage.ConstructCommonMessage(GLOBAL_MSG_ERROR, "外面PM2.5这么严重, 宅在家里至少写点代码再提交吧", [['javascript:history.go(-1)', '返回之前的页面']]))

        if not MLanguage.IsSupportLang(code_language_type):
            return render.TMessage(MMessage.ConstructCommonMessage(GLOBAL_MSG_ERROR, "未知的语言类型", [['javascript:history.go(-1)', '返回之前的页面']]))

        if not len(code_title):
            code_title = GLOBAL_DEFAULT_POST_TITLE

        post = {
            'id' : code_id,
            'priviledge':code_priviledge,
            'language_type':code_language_type,
            'title':code_title,
            'content':code_content,
            'last_edit_time':int(time.time())
            }

        r = MPost.Post.UpdateDB(post)

        if r['Status'] == -1:
            return render.TMessage(MMessage.ConstructCommonMessage(GLOBAL_MSG_ERROR, "操作失败: [" + r['ErrorMsg'] + "]。", [['javascript:history.go(-1)', '返回之前的页面']]))

        msg = ""
        msg += "<div style='margin-top:5px;'>"
        msg += "还可以继续执行以下操作:" 
        msg += "</div>"
        msg += "<div style='margin-top:5px;'>"
        msg += "<a class='button-a' href='/post/view/"+ str(post['id'])  + "'>查看</a>"
        if session['UserID'] != -1:
            msg += "<a class='button-a' href='/post/edit/"+ str(post['id'])  + "'>编辑</a>"
            msg += "<a class='button-a' href='/post/del/"+ str(post['id'])  + "'>删除</a>"
        msg += "<a class='button-a' href='/post/add' target='_blank'>新建代码片段</a>"
        msg += "<a class='button-a' href='/post/my'>我的代码片段</a>"
        msg += "</div>"

        return render.TMessage("<span class='msg-success'>编辑成功啦.</span><br />" + msg)



class PostDel:
    def GET(self, post_id):
        if session['UserID'] == -1:
            web.seeother("/user/login")
        try:
            pid = int(post_id)
        except:
            return render.TMessage("<span class='msg-error'>参数传递异常</span><br /><br /><a href='/'>返回主页</a>")

        post = MPost.Post.QueryDB(post_id = pid)

        if not post:
            return render.TMessage("<span class='msg-error'>失败: [查看该代码片段发生异常]</span><br /><a href='/'>返回主页</a>")
        

        if post.user.id != session['UserID']:
            return render.TMessage(MMessage.ConstructCommonMessage(GLOBAL_MSG_ERROR, "您没有权限修改该代码片段", [['javascript:history.go(-1)', '返回']]))


        #检查查看的权限
        if post['priviledge'] == GLOBAL_PRIVILEDGE_PRIVATE:
            if session['UserID'] != post['user_id']:
                return render.TMessage("<span class='msg-error'>对不起，您没有权限编辑此代码片段</span><br /><a href='/'>返回主页</a>")

        return render.TPostDelView(post)

    def POST(self, post_id):
        if session['UserID'] == -1:
            web.seeother("/user/login")
        
        code_id = web.input()['code_id'].strip()

        if code_id != post_id:
            return render.TMessage("<span class='msg-error'>参数传递异常</span><br /><br /><a href='/'>返回主页</a>")

        try:
            code_id = int(code_id)
        except:
            return render.TMessage("<span class='msg-error'>参数传递异常</span><br /><br /><a href='/'>返回主页</a>")

        #先检查该Post是否为当前用户所有

        if not MPost.Post.CheckPostOwner(code_id, session['UserID']):
            return render.TMessage(MMessage.ConstructCommonMessage(GLOBAL_MSG_ERROR, "你没有权限修改该文章", [['javascript:history.go(-1)', '返回之前的页面']]))
        
        r = MPost.Post.DeleteFromDBByPostID(code_id)

        if r['Status'] == -1:
            return render.TMessage(MMessage.ConstructCommonMessage(GLOBAL_MSG_ERROR, "操作失败: [" + r['ErrorMsg'] + "]。", [['javascript:history.go(-1)', '返回之前的页面']]))

        msg = ""
        msg += "<div style='margin-top:5px;'>"
        msg += "您还可以执行以下操作:" 
        msg += "</div>"
        msg += "<div style='margin-top:5px;'>"
        msg += "<a class='button-a' href='/post/add'>新建代码片段</a>"
        msg += "<a class='button-a' href='/post/my'>我的代码片段</a>"
        msg += "</div>"

        return render.TMessage("<span class='msg-success'>删除成功.</span><br />" + msg)
        
        
       
class PostGenImage:
    def GET(self, short_link):
        if not short_link:
            return render.TMessage(MMessage.ConstructCommonMessage(GLOBAL_MSG_ERROR, "参数传递错误", [['javascript:history.go(-1)', '返回之前的页面']]))

        post = MPost.Post.QueryDB(short_lnk = short_link)

        if not post:
            return render.TMessage("<span class='msg-error'>失败: [查看该代码片段发生异常]</span><br /><a href='/'>返回主页</a>")

  
        #检查查看的权限
        if post['priviledge'] == GLOBAL_PRIVILEDGE_PRIVATE:
            if session['UserID'] != post['user_id']:
                return render.TMessage("<span class='msg-error'>对不起，您没有权限查看此代码片段</span><br /><a href='/'>返回主页</a>") 
  
        
        filename = GLOBAL_PIC_STORE_DIRECTORY + '/' + post.link + ".gif"

        regen = not os.path.exists(filename)

        if not regen:
            #check the post ever be edited after the pic generated
            st = os.stat(filename)
            regen = regen or (int(st.st_ctime) < int(post.last_edit_time_ts))

        if regen:
            if GLOBAL_USING_XVFB:
                cmd = 'xvfb-run --server-args="-screen 0, 800x600x24"'
            else:
                cmd = ""
            cmd += " " + GLOBAL_PHANTOMJS_BIN_DIRECTORY + "/phantomjs" + " " + GLOBAL_PHANTOMJS_SCRIPTS_DIRECTORY + "/rasterize.js" + " " + "http://" + GLOBAL_PROJECT_DOMAIN + "/F_" + post.link + " "+filename   
            os.system(cmd)

        f = open(filename, "r")
        
        web.header('Content-type', 'image/gif')

        data = f.read()

        f.close()

        return data


class PostShortLnkViewer:
    def GET(self, short_lnk):
        if not short_lnk:
            return render.TMessage("<span class='msg-error'>参数传递错误</span><br /><a href='/'>返回主页</a>")

        post = MPost.Post.QueryDB(short_lnk = short_lnk)

        if not post:
            return render.TMessage("<span class='msg-error'>失败: [查看该代码片段发生异常]</span><br /><a href='/'>返回主页</a>")

        #检查查看的权限
        if post['priviledge'] == GLOBAL_PRIVILEDGE_PRIVATE:
            if session['UserID'] != post['user_id']:
                return render.TMessage("<span class='msg-error'>对不起，您没有权限查看此代码片段</span><br /><a href='/'>返回主页</a>")


        comments = MComment.Comment.QueryDB(post_id=post['id'])

        if not comments:
            comments = []

        
        return render.TPostView(post, comments)
        

class PostEmbed:
    def GET(self, short_link):
        if not short_link:
            return render.TMessage("<span class='msg-error'>参数传递错误</span><br /><a href='/'>返回主页</a>")

        post = MPost.Post.QueryDB(short_lnk = short_link)

        if not post:
            return render.TMessage("<span class='msg-error'>失败: [查看该代码片段发生异常]</span><br /><a href='/'>返回主页</a>")
        
        #检查查看的权限
        if post['priviledge'] == GLOBAL_PRIVILEDGE_PRIVATE:
            if session['UserID'] != post['user_id']:
                return render.TMessage("<span class='msg-error'>对不起，您没有权限查看此代码片段</span><br /><a href='/'>返回主页</a>")        

        render = web.template.render(GLOBAL_PROJECT_ROOT + '/templates/')

        return render.TPostEmbed(post)

