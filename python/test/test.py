def tt(f):
 def ttt():
  print "name1=",__name__
  f()
  print "name2=",__name__
 return ttt

@tt
def name():
 print "name3=",__name__
 
name()
