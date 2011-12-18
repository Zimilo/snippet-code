#!/usr/bin/env python
#coding:utf-8

from config.Globals import *

class MLanguage:
    @staticmethod
    def GetAllLangs():
        return GLOBAL_LANGUAGES

    @staticmethod
    def IsSupportLang(lang_id):
        for lang in GLOBAL_LANGUAGES:
            if lang[0] == lang_id:
                return True
        return False

    @staticmethod
    def Query(lang_id):
        for lang in GLOBAL_LANGUAGES:
            if lang[0] == lang_id:
                return lang
        return None
