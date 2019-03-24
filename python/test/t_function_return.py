def p0():
 print '0'

def p1():
 print '1'
 return p0

def p2():
 p3=p1()
 p4=p3()
 p3()
 print p4 


p2()
