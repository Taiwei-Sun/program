def h():
 print 'a'
 m=yield 1
 print m
 n=yield 2
 print 'b'

c=h()
c.next()
print c.send(10)
