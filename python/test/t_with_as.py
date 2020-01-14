from contextlib import contextmanager

@contextmanager
def make_context() :
 print 'enter'
 try :
   yield {}
 except RuntimeError, err :
  print 'error' , err
 finally :
  print 'exit'

with make_context() as value :
 print '1'
 print value
 print '2'
