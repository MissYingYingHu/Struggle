#!/usr/bin/env python
# coding=utf-8

import sys

if len(sys.argv) != 4:
    Usage = 'python generate input.txt output.txt size'
    print Usage
    exit()
input_file_path = sys.argv[1]
output_file_path = sys.argv[2]
size = int(sys.argv[3])*1024*1024

input_file = open(input_file_path,'r')
output_file = open(output_file_path,'w')
index = 0
total_size = 0
input_data = input_file.readlines()

while True:
    if total_size > size:
        break
    output_file.write(input_data[index])
    total_size += len(input_data[index])
    index += 1
    if index > len(input_data):
        index = 0
