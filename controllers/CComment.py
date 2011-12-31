#!/usr/bin/env python
#coding:utf-8
import time
import web
from config.Runtime import *
from config.Globals import *
from models.MMessage import MMessage
from models import MPost
from models import MComment

class CommentPost:
	def POST(self):
		pid = int(web.input().post_id.strip())
		user_name = web.input().user_name.strip()
		comment_content = web.input().comment_content.strip()
		if not user_name or not comment_content:
			return render.TMessage(MMessage.ConstructCommonMessage(GLOBAL_MSG_ERROR, "所有字段均不能为空", [['javascript:history.go(-1)', '返回之前的页面']]))
		


		post = MPost.Post.QueryDB(post_id = pid)

		if not post:
			return render.TMessage(MMessage.ConstructCommonMessage(GLOBAL_MSG_ERROR, "参数传递出错", [['javascript:history.go(-1)', '返回之前的页面']]))


		comment = {
			'post_id' : pid,
			'user_name' : user_name,
			'comment_content' : comment_content,
			'publish_time' : int(time.time()),
		}

		r = MComment.Comment.Insert2DB(comment)
		if r['Status'] == -1:
			return render.TMessage(MMessage.ConstructCommonMessage(GLOBAL_MSG_ERROR, "提交评论失败: [" + r['ErrorMsg'] + "]。", [['javascript:history.go(-1)', '返回之前的页面']]))


		return render.TMessage(MMessage.ConstructCommonMessage(GLOBAL_MSG_NORMAL, "发布评论成功啦!", [['/post/view/'+str(pid), '返回']]))
