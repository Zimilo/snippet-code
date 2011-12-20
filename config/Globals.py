#!/usr/bin/env python
#coding:utf-8

#项目相关的信息
GLOBAL_PROJECT = "snippet-code"
GLOBAL_PROJECT_MAJOR_VERSION = "0"
GLOBAL_PROJECT_MINOR_VERSION = "1"
GLOBAL_PROJECT_AUTHOR = "Zimilo"
GLOBAL_PROJECT_AUTHOR_EMAIL = "zimilo@code-trick.com"


#数据库相关的配置
GLOBAL_DB_HOST        = "localhost"
GLOBAL_DB_PORT        = 3306
GLOBAL_DB_USER        = "zJiXU"
GLOBAL_DB_PASSWD      = "iUpIUY"
GLOBAL_DB_DB          = "snippet"
GLOBAL_DB_PRE         = "snippet_"
GLOBAL_DB_POSTS_TABLE = "posts"
GLOBAL_DB_USERS_TABLE = "users"

#系统支持的语言类型的配置
GLOBAL_LANGUAGES = [
    #ID, Type, DisplayName
    [1,   "txt",    "纯文本"],
    [2,   "cpp",    "C/CPP"],
    [2,   "python", "Python"],
    [3,   "php",    "PHP"],
    [4,   "erlang", "Erlang"],
    [5,   "ruby",   "Ruby"],
    [6,   "perl",   "Perl"],
    [7,   "scala",  "Scala"],
    [8,   "css",    "CSS"],
    [9,   "javascript", "Javascript"],
    [10,  "lua",    "Lua"],
    [11,  "java",   "Java"],
    [12,  "objectivec", "Objective-C"],
    [13,  "lisp",    "Lisp"],
    [14,  "bash",    "Bash"],
    [15,  "haskell", "Haskell"],
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
