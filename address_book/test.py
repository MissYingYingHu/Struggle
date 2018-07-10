#!/usr/bin/env python
# coding=utf-8
import addr_book

while True:
    addr_book.menu()
    input = raw_input('please input need:')
    if input == '1':
        addr_book.add_contact()
        addr_book.save_to_file()
    elif input == '2':
        addr_book.rearch_contact()
    elif input == '3':
        addr_book.deleted_contact()
        addr_book.save_to_file()
    elif input == '4':
        addr_book.load_all_people()
    elif input == '0':
        exit()
    


