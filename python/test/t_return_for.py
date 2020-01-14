def h():
 return '1 3 5 7 9'

def h2():
 return '13579'

def h3():
 return [1,3,5,7,9]

def h4():
 j=1
 while j <10:
  yield j
  j=j+2

for i in h4():
 print i
