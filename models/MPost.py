#!/usr/bin/env pytho
#coding:utf-8

class Post:
    def __init__(self, post_id):
        self.post_id = post_id
        self.errors = []
    def GetLastError(self):
        if not len(self.errors):
            return None
        else:
            return self.errors[len(self.errors)-1]
    def QueryDB(self):
        pass
    def Format(self):
        pass

