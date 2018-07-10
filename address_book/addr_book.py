#!/usr/bin/env python
# coding=utf-8
import pickle
import cPickle

phone_dire = []
path = './addr.txt'
def menu():
    print '**************welcome to address book************'
    usage = '''
    1.add contact
    2.search contact
    3.erase contact
    4.display all contact
    0.quit address book
    '''
    print usage
    print '*************************************************'

def add_contact():
     name = raw_input('name:')
     tel = raw_input('tel:')
     qq = raw_input('QQ:')
     e_mail = raw_input('e_mail:')
     #phone_dire.append({'name':name,'tel':tel,'QQ':qq,'e_mail':e_mail})
     phone_dire.append({'e_mail':e_mail,'QQ':qq,'tel':tel,'name':name})
     print 'add sucess!'
     
def menu_rearch():
    print '***************************************'
    method = '''
    1.按名字查找
    2.按手机号查找
    3.按QQ号查找
    4.按邮箱号查找
    '''
    print method
    print '***************************************'
    while True:
        i = raw_input('请输入查询方式:')
        if int(i) < 1 or int(i) > 4:
            print '请输入正确的选项'
            continue
        else:
            break
    return i

def rearch_contact():
    f = open(path,'rb')
    global phone_dire
    phone_dire = cPickle.load(f)

    if len(phone_dire) == 0:
        print 'address book is empty'
        return
    i = menu_rearch()
    if i == '1':
        r_name = raw_input('please input name:')
        for user in phone_dire:
            if user['name'] == r_name:
                print user
                break
        else:
            print 'the user is not exists'
    elif i == '2':
        r_tel = raw_input('please input tel:')
        for user in phone_dire:
            if user['tel'] == r_tel:
                print user
                break
        else:
            print 'the user is not exists'
    elif i == '3':
        r_qq = raw_input('please input QQ:')
        for user in phone_dire:
            if user['QQ'] == r_qq:
                print user
                break
        else:
            print 'the user is not exists'
    elif i == '4':
        r_mail = raw_input('please input e_mail:')
        for user in phone_dire:
            if user['e_mail'] == r_mail:
                print user
                break
        else:
            print 'the user is not exists'
    f.close()

def deleted_contact():
    del_name = raw_input('please input the name of deleted:')

    for user in phone_dire:
        if user['name'] == del_name:
            phone_dire.remove(user)
            print 'delete success!'
            break
    else:
        print 'the user is not exits!'

#保存到文件
def save_to_file():
    f = open(path,'wb')
    global phone_dire
    cPickle.dump(phone_dire,f)
    print 'save success'
    f.close()

#加载通讯中中所有的人
def load_all_people():
    f = open(path,'rb')
    global phone_dire
    #pickle.loads(f)
    phone_dire = cPickle.load(f)
    for user in phone_dire:
        print user
    f.close()
