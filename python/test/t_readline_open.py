import time
FILE='./file.txt'

def get_txt(file_all):
 n=0
 while True:
  time.sleep(1)
  line=file_all.readline()
  if not line:
   n=n+1
   print 'no line',n
   if n == 3:
    file_all.seek()
   continue
  else:
   n=0
  yield line


file_all=open(FILE)
for line in get_txt(file_all):
 print line,
