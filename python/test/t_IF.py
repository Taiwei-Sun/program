
class test(object):

 def __init__(self, word):
  self.word=word

 def check(self):
  result = self.word
  #return True
  return False



tt=test('ppp')
if('ppp') == (tt.word) or tt.check():
 print 'true'
else:
 print 'false'
