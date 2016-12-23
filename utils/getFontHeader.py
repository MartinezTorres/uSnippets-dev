#!/usr/bin/python

import sys, os
import Image

if len(sys.argv)<2:
	os.system("convert -list font | grep Font: | less")
	sys.exit();
	
if sys.argv[1]=="all":
	print os.system("for f in `convert -list font | grep Font: | cut -d: -f2 `; do python "+sys.argv[0]+" $f; echo; done")
	sys.exit();

if 0!=os.system("convert -list font | grep Font: | cut -d: -f2 | cut -c2- | cat | grep -cx "+sys.argv[1]+" > /dev/null"):
	print "Font "+sys.argv[1]+" not found!"
	sys.exit();
	
print "Font "+sys.argv[1]+" found!"
print "Using ImageMagick to create the font texture"
if not os.path.isfile("tmp/"+sys.argv[1]+".png"):
	os.system("mkdir -p fonts tmp")
	os.system("perl -e 'for ($a=32; $a<128; $a = $a+1) { print \" \".chr($a).\" \n\"; }' | convert -font "+sys.argv[1]+" -pointsize 96 label:'@-' tmp/"+sys.argv[1]+".png");


print "Reading Font Data"
I = Image.open("tmp/"+sys.argv[1]+".png").convert('L')
rledata = []
csizes = []

w, h = I.size
ch = h/97
for c in range(0,96):
	sz = w
	f = 0
	while f==0 and sz>1:
		sz = sz-1
		for i in range(0, ch):
			if (I.getpixel((sz,c*ch+i))<128):
				f=1
	csizes.append(sz)
	
v=15
l=0
for j in range(0, w):
	for i in range(0,ch*96):
		if ((I.getpixel((j,i))+8)/17)==v:
			l = l+1
		else:
			rledata.append(v+l*16)
			v = ((I.getpixel((j,i))+8)/17)
			l=0
rledata.append(v + l*16);

print "Using Writing Header File"
out = open("fonts/"+sys.argv[1].replace('-', '')+".h","w")
out.write("#pragma once\n")
out.write("static const int "+sys.argv[1].replace('-', '')+"[]={"+str(w)+","+str(ch*96)+","+(",".join(map(str,csizes)))+","+(",".join(map(str,rledata)))+" };\n");
