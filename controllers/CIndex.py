#!/usr/bin/env python
#coding:utf-8

from config.Runtime import *
from models.MLanguage import MLanguage
from models.MMessage import MMessage
from models import MPost

class Index:
    def GET(self, page_idx = 1):

        page_idx = int(page_idx)

        if page_idx <= 0:
            return render.TMessage(MMessage.ConstructCommonMessage(GLOBAL_MSG_ERROR, "参数传递错误", [['javascript:history.go(-1)', '返回']]))

    	params = {
    		'count' : GLOBAL_POST_LIST_PAGE_SIZE,
    		'order' : 'publish_time DESC',
    		'offset' : (page_idx - 1) * GLOBAL_POST_LIST_PAGE_SIZE,
    		'where' : 'priviledge=%d' % GLOBAL_PRIVILEDGE_PUBLIC,
    	}

    	(posts, total_count) = MPost.Post.GetPosts(params)

    	max_page_idx = (total_count + GLOBAL_POST_LIST_PAGE_SIZE - 1) / GLOBAL_POST_LIST_PAGE_SIZE

    	if max_page_idx > 0 and page_idx > max_page_idx:
        	return render.TMessage(MMessage.ConstructCommonMessage(GLOBAL_MSG_ERROR, "参数传递错误", [['javascript:history.go(-1)', '返回']]))
   		

    	return render.TIndexV2(posts, page_idx, max_page_idx)
