#!/usr/bin/python3

from sys import argv
from subprocess import call


def cmake():
    try:
        call('mkdir build && cd build && cmake ..', shell=True)
    except:
        print('Error')


def make():
    try:
        call('cd build && make', shell=True)
    except:
        print('Error')


def run():
    try:
        call('cd build && cd bin && ./app', shell=True)
    except:
        print('Error')


def clear():
    try:
        call('rm -rf build', shell=True)
    except:
        print('Error')


if __name__ == "__main__":
    command = str(argv[-1])
    if(command == "build"):
        clear()
        cmake()
        make()
    if(command == "cmake"):
        cmake()
    if(command == "make"):
        make()
    if(command == "run"):
        run()
    if(command == "clear"):
        clear()
