#!/usr/bin/python
import os

for thread in [1,2,4,8]:
	for node in [64,128,256,512,1024,2048,4096,8192]:
		ccmd = "g++ -DNUMT=%d -DNUMNODES=%d proj1.cpp lib_cs475.cpp -o proj1 -lm -fopenmp" %(thread, node);
		rcmd = "./proj1";
		os.system(ccmd);
		os.system(rcmd);