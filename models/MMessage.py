#!/usr/bin/env python
#coding:utf-8

from config.Globals import *

class MMessage:
    @staticmethod
    def ConstructCommonMessage(warning_level, msg, controls):
        ret = '<div><span'

        if warning_level == GLOBAL_MSG_ERROR:
            ret += " class='msg-error'"

        if warning_level == GLOBAL_MSG_NORMAL:
            ret += " class='msg-success'"
        
        ret += ">"
        ret += msg
        ret += '</span></div>'
        ret += "<div style='margin-top:10px'>"

        for control in controls:
            ret += "<a href='" + control[0] + "' class='button-a'>" + control[1] + "</a>"
        
        ret += "</div>"

        return ret
