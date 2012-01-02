#!/usr/bin/env python
#coding:utf-8

#项目相关的信息
GLOBAL_PROJECT = "snippet-code"
GLOBAL_PROJECT_MAJOR_VERSION = "0"
GLOBAL_PROJECT_MINOR_VERSION = "1"
GLOBAL_PROJECT_AUTHOR = "Zimilo"
GLOBAL_PROJECT_AUTHOR_EMAIL = "zimilo@code-trick.com"
GLOBAL_PROJECT_DOMAIN = "snippet-code.com"
GLOBAL_PROJECT_ROOT = "/Users/Zimilo/ZimiloProjects/snippet-code"

#数据库相关的配置
GLOBAL_DB_HOST        = "localhost"
GLOBAL_DB_PORT        = 3306
GLOBAL_DB_USER        = "zJiXU"
GLOBAL_DB_PASSWD      = "iUpIUY"
GLOBAL_DB_DB          = "snippet"
GLOBAL_DB_PRE         = "snippet_"
GLOBAL_DB_POSTS_TABLE = "posts"
GLOBAL_DB_USERS_TABLE = "users"
GLOBAL_DB_COMMENTS_TABLE = "comments"

#系统支持的语言类型的配置
GLOBAL_LANGUAGES = [
    #ID, Type, DisplayName
    [1,   "txt",    "纯文本"],
    [2,   "cpp",    "C/CPP"],
    [3,   "python", "Python"],
    [4,   "php",    "PHP"],
    [5,   "erlang", "Erlang"],
    [6,   "ruby",   "Ruby"],
    [7,   "perl",   "Perl"],
    [8,   "scala",  "Scala"],
    [9,   "css",    "CSS"],
    [10,   "javascript", "Javascript"],
    [11,  "lua",    "Lua"],
    [12,  "java",   "Java"],
    [13,  "objectivec", "Objective-C"],
    [14,  "lisp",    "Lisp"],
    [15,  "bash",    "Bash"],
    [16,  "haskell", "Haskell"],
    [999, "Others", "其它" ]
    ]



#系统发布的权限
GLOBAL_PRIVILEDGE_PUBLIC = 1
GLOBAL_PRIVILEDGE_PRIVATE = 0


#POST的相关配置
GLOBAL_DEFAULT_POST_TITLE = "未命名代码片段"
GLOBAL_DEFAULT_POST_PUBLISH_INTERVAL = 3

#系统消息相关配置
GLOBAL_MSG_ERROR = 2
GLOBAL_MSG_NORMAL = 0
GLOBAL_MSG_WARNING = 1


#代码片段列表分页相关数量
GLOBAL_POST_LIST_PAGE_SIZE = 10



#图片生成
GLOBAL_PIC_STORE_DIRECTORY = "/Users/Zimilo/ZimiloProjects/snippet-code/pics/"
GLOBAL_PHANTOMJS_BIN_DIRECTORY = "/Users/Zimilo/ZimiloProjects/snippet-code/scripts/phantomjs-1.4.1/bin"
GLOBAL_PHANTOMJS_SCRIPTS_DIRECTORY = "/Users/Zimilo/ZimiloProjects/snippet-code/scripts/phantomjs-1.4.1/examples"
GLOBAL_USING_XVFB = False
