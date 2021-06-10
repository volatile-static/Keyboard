import sys
import re

__version = '1.0.0'

def Process(file1, file2, addr_start):
    file_out = open("../Software.hex", "w")
    f1_lines = file1.readlines()
    f2_lines = file2.readlines()

    match_Obj = None
    for line in f1_lines:
        match_Obj = re.match(r':(..)' + addr_start, line)
        if(match_Obj==None):
            file_out.write(line)
        else:
            break
        
    match_Obj = None
    for line in f2_lines:
        if(match_Obj == None):
            match_Obj = re.match(r':(..)' + addr_start, line)
        if(match_Obj != None):
            file_out.write(line)


def main():
    if len(sys.argv) == 1:
        f1 =open(input("Enter Files 1:"), "r")
        f2 =open(input("Enter Files 2:"), "r")
        addr = input("Enter beginning address:")
        Process(f1, f2, addr)
    else:
        f1 = open(sys.argv[1], "r")
        f2 = open(sys.argv[2], "r")
        addr = sys.argv[3]
        Process(f1, f2, addr)

    return


if __name__ == '__main__':
    main()
