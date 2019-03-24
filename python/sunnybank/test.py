
import re

LOG_DATE_TIME = r'(?P<date_time>[A-Z][a-z][a-z][ ]+[0-9]+ [0-9][0-9]:[0-9][0-9]:[0-9][0-9]) '
F5_DEVICE_NAME_AND_LOG_LEVEL = r'(?P<device>\w+) (?P<log_level>\w+) '
#F5_SESSION_ID = r'(?P<pid>.+)\[\d+\]: .+:\d+: /Common/\w+:Common:(?P<session_id>\w+): '
F5_SESSION_ID = r'(?P<pid>.+)\[\d+\]: .+:\d+: .+:Common:(?P<session_id>\w+): '

F5_NEW_SESSION_LOGIN = r'.* (?P<new_session_id>\w+): New '
F5_LOGIN_IP = r'session from client IP (?P<login_ip>\d+.\d+.\d+.\d+)'
F5_LOGIN_USER = r'.* \'session.logon.last.username\' set to (?P<user>\'\w+\')'
F5_ASSIGN_IP = r'.* Assigned PPP Dynamic IPv4: (?P<client_ip>\d+.\d+.\d+.\d+) '
F5_LOGIN_MAC = r'.*mac_address is (?P<loginMac>\w+:\w+:\w+:\w+:\w+:\w+)'
F5_LOG_DETAIL = r'(?P<detail>.*)'

F5_FULL_LOG = LOG_DATE_TIME + F5_DEVICE_NAME_AND_LOG_LEVEL + F5_SESSION_ID + F5_LOG_DETAIL
F5_SESSION_LOGOUT = LOG_DATE_TIME + F5_DEVICE_NAME_AND_LOG_LEVEL + F5_SESSION_ID + r'.* (?P<log_out>Session deleted due to user logout request\.)'

# FTP
FTP_DEVICE_NAME = LOG_DATE_TIME + r'(?P<device>\w+) '
FTP_LOG_DETAIL = FTP_DEVICE_NAME + r'(?P<detail>.*)'
FTP_USER = r'.* Users: (?P<user>.+),'
FTP_USER_IP = r' Source IP: (?P<user_ip>.+),'
FTP_USER_COMPUTER_NAME = r' Computer name: (?P<pc_name>.+), .+,'
FTP_USER_ACCESS_FILE = r' Accessed resources: (?P<file>.+),'
FTP_USER_ACTION = r' Action: (?P<action>.+)'
FTP_USER_DETAIL = FTP_USER + FTP_USER_IP + FTP_USER_COMPUTER_NAME\
    + FTP_USER_ACCESS_FILE + FTP_USER_ACTION
FTP_CONNECTION_TYPE = r'.* Connection type: (?P<connection_type>.+), Acce.*'
F5_PID = ['tmm1', 'tmm', 'tmm2', 'apd']


LOG_FILE = 'log_data/messages2017'
#LOG_FILE = 'log_data/test_log'
lineN = 5

def get_log(logfile):
 while True:
  line = logfile.readline()
  if line == None:
   break
  yield line

def parse_log(line):
 log_check = re.compile(F5_FULL_LOG)
 res = log_check.match(line)

 if res is None:
  return None

 sessionId = res.group('session_id')
 return sessionId

def return_none():
 return None

logfile = open(LOG_FILE)
"""for line in logfile:
 sessionId=parse_log(line)
 if sessionId != None:
  #print line,
  print sessionId,' ',"""

"""for line in get_log(logfile):
 print line"""

if return_none() == None:
 print 'ok'
else:
 print 'not ok'
 
