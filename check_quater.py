#!/usr/bin/python3

import os

if __name__ == "__main__":
    while 1:
        s = os.read(0, 40)
#        print(s.decode("utf-8"))
        q = [float(x) for x in s.decode("utf-8").split(sep=' ')]
        print("w:%.6f x:%.6f y:%.6f z:%.6f" %(q[0], q[1], q[2], q[3]))
        print("norm final value %f" %(q[0]**2 + q[1]**2 + q[2]**2 + q[3]**2))
    
