# Python library
import re
import time
import datetime
import json
import logging

from sunnybank.database import tools, schema
from sunnybank.config.enviroment import LOG_FILE, LOG_TIME

LOG = logging.getLogger('FlaskApp')

# Regular match formate
# F5
LOG_DATE_TIME = r'(?P<date_time>[A-Z][a-z][a-z][ ]+[0-9]+ [0-9][0-9]:[0-9][0-9]:[0-9][0-9]) '
F5_DEVICE_NAME_AND_LOG_LEVEL = r'(?P<device>\w+) (?P<log_level>\w+) '
#F5_SESSION_ID = r'(?P<pid>.+)\[\d+\]: .+:\d+: /Common/\w+:Common:(?P<session_id>\w+): '
F5_SESSION_ID = r'(?P<pid>.+)\[\d+\]: .+:\d+: .+:Common:(?P<session_id>\w+): '
#F5_SESSION_ID = r'(?P<pid>.+)\[\d+\]: .+:\d+: (?P<session_id>\w+): '

#F5_ASSIGN_IP = r'.* Assigned PPP Dynamic IPv4: (?P<client_ip>\d+.\d+.\d+.\d+) '
F5_ASSIGN_IP = r'.* Assigned PPP \w+ IPv4: (?P<client_ip>\d+.\d+.\d+.\d+) '

#F5_NEW_SESSION_LOGIN = r'.* (?P<new_session_id>\w+): New '
F5_NEW_SESSION_LOGIN = r'.+:Common:(?P<new_session_id>\w+): New '
F5_LOGIN_IP = r'session from client IP (?P<login_ip>\d+.\d+.\d+.\d+)'

#F5_LOGIN_MAC = r'.*mac_address is (?P<loginMac>\w+:\w+:\w+:\w+:\w+:\w+)'
F5_LOGIN_MAC = r'.*\[0\]\.mac_address is (?P<loginMac>\w+:\w+:\w+:\w+:\w+:\w+)'

#F5_LOGIN_USER = r'.* \'session.logon.last.username\' set to (?P<user>\'\w+\')'
F5_LOGIN_USER = r'.*: Username (?P<user>\'\w+\')'
F5_LOG_DETAIL = r'(?P<detail>.*)'

F5_FULL_LOG = LOG_DATE_TIME + F5_DEVICE_NAME_AND_LOG_LEVEL + F5_SESSION_ID + F5_LOG_DETAIL
F5_SESSION_LOGOUT = LOG_DATE_TIME + F5_DEVICE_NAME_AND_LOG_LEVEL + F5_SESSION_ID + r'.*(?P<log_out>PPP tunnel \w+ closed\.)'

# FTP
FTP_DEVICE_NAME = LOG_DATE_TIME + r'(?P<device>\w+) '
FTP_LOG_DETAIL = FTP_DEVICE_NAME + r'(?P<detail>.*)'
FTP_USER = r'.* Users: (?P<user>.+),'
FTP_USER_IP = r' Source IP: (?P<user_ip>.+),'
FTP_USER_COMPUTER_NAME = r' Computer name: (?P<pc_name>.+), .+,'
FTP_USER_ACCESS_FILE = r' Accessed resources: (?P<file>.+),'
FTP_USER_ACTION = r' Action: (?P<action>.+)'
FTP_USER_DETAIL = FTP_USER + FTP_USER_IP + FTP_USER_COMPUTER_NAME + FTP_USER_ACCESS_FILE + FTP_USER_ACTION
FTP_CONNECTION_TYPE = r'.* Connection type: (?P<connection_type>.+), Acce.*'
F5_PID = ['tmm', 'tmm1', 'tmm2', 'tmm3', 'apd', 'apmd']


class Singleton(type):
    _instance = {}

    def __call__(cls, *args, **kwargs):
        if cls not in Singleton._instance:
            Singleton._instance[cls] = type.__call__(cls, *args, **kwargs)
        return Singleton._instance[cls]


class Logparse(object):
    __metaclass__ = Singleton

    def __init__(self):
        self.all_f5 = list()
        self.all_ftp = list()
        self.session_data = dict()
        self.lastLogTime=""

    def checkTime(self,dataT):
     #print datetime.datetime.strptime(dataT.date_time,'%Y-%m-%d %H:%M:%S')
     if datetime.datetime.strptime(dataT.date_time,'%Y-%m-%d %H:%M:%S').date() > datetime.date.today():
      return False
     else:
      return True
    
    def getLogTimeForCheck(self):
     ff=open(LOG_TIME,'a+')
     log_time=ff.read(19)
     if log_time == "":
      logf=open(LOG_FILE,'r')
      log_time=logf.read(15)
      logf.close()
      today = datetime.date.today()
      log_time = str(today.year) + ' ' + log_time
      log_time = time.strptime(log_time, "%Y %b %d %H:%M:%S")
      log_time = time.strftime('%Y-%m-%d %H:%M:%S', log_time)
      ff.write(log_time)

     ff.close()
     self.lastLogTime=log_time

    def checkLogTime(self,dataT):
     if not self.checkTime(dataT):
      return False
 
     date_time1=time.strptime(self.lastLogTime,'%Y-%m-%d %H:%M:%S')
     date_time2=time.strptime(dataT.date_time,'%Y-%m-%d %H:%M:%S')
     if date_time1 > date_time2:
      return False
     else:
      ff=open(LOG_TIME,'w')
      ff.write(dataT.date_time)
      ff.close()
      self.lastLogTime=dataT.date_time
      return True


    def save_to_database(self):
        LOG.debug("Save log data to database.")
        with tools.init_session() as session:
            LOG.debug("Starting save F5 log data.")
            for log_data in self.all_f5:
                log_data = schema.LogData(
                    log_level=log_data.get('logLevel'),
                    log_type='F5',
                    f5_session_id=log_data.get('sessionId'),
                    f5_login_ip=log_data.get('loginIp'),
                    f5_client_ip=log_data.get('clientIp'),
                    device=log_data.get('device'),
                    f5_user=log_data.get('user'),
                    f5_user_mac=log_data.get('loginMac'),
                    f5_user_action=log_data.get('clientAction'),
                    detail=log_data.get('detail'),
                    date_time=log_data.get('dateTime'))
               
                if self.checkTime(log_data):
                 session.add(log_data)

            LOG.debug("Starting save FTP log data.")
            for log_data in self.all_ftp:
                log_data = schema.LogData(
                    log_type='FTP',
                    ftp_user_computer=log_data.get('computer'),
                    ftp_user_ip=log_data.get('userIp'),
                    ftp_user_access_file=log_data.get('file'),
                    device=log_data.get('device'),
                    ftp_user=log_data.get('ftpUser'),
                    f5_user=log_data.get('user'),
                    ftp_user_action=log_data.get('action'),
                    detail=log_data.get('detail'),
                    date_time=log_data.get('dateTime'))

                if self.checkTime(log_data):
                 session.add(log_data)

        self.all_f5 = list()
        self.all_ftp = list()

    def full_log_check(self, line):
        f5_log = dict()

        LOG.debug("Start check F5 log now.")
        sessionId = self.get_basic_info(line)
        if sessionId is None:
            LOG.debug("Not F5 log, start FTP check now.")
            self.get_basic_ftp_info(line)
            return

        self.get_login_ip(line, sessionId)
        self.get_login_user(line, sessionId)
        self.get_assign_ip(line, sessionId)
        self.get_login_mac(line, sessionId)
        #print 'login_ip=', self.get_login_ip(line, sessionId)
        #print 'login_user=', self.get_login_user(line, sessionId)
        #print 'assign_ip=', self.get_assign_ip(line, sessionId)
        #print 'login_mac=', self.get_login_mac(line, sessionId)
        session_id = self.get_logout_session(line)

        #print 'session_id=', session_id

        f5_log.update(self.session_data[sessionId])
        if session_id is not None:
            LOG.debug("Session data ended, delete session data in memory.")
            del self.session_data[session_id]

        self.all_f5.append(f5_log)

    def get_basic_info(self, line):
        log_check = re.compile(F5_FULL_LOG)
        res = log_check.match(line)
        
        if res is None:
            return None

        sessionId = res.group('session_id')
        pid = res.group('pid')
        if pid not in F5_PID:
            return None

        if sessionId not in self.session_data:
            self.session_data[sessionId] = dict()

        self.session_data[sessionId]['sessionId'] = sessionId
        self.session_data[sessionId]['device'] = res.group('device')
        self.session_data[sessionId]['detail'] = res.group('detail')
        self.session_data[sessionId]['logLevel'] = res.group('log_level')

        

        today = datetime.date.today()
        date_time = str(today.year) + ' ' + res.group('date_time')
        date_time = time.strptime(date_time, "%Y %b %d %H:%M:%S")
        self.session_data[sessionId][
            'dateTime'] = time.strftime('%Y-%m-%d %H:%M:%S', date_time)

        return sessionId

    def get_basic_ftp_info(self, line):
        log_data = dict()

        log_check = re.compile(FTP_CONNECTION_TYPE)
        res = log_check.match(line)
        if res is not None and res.group('connection_type') != 'FTP' and res.group('connection_type') != 'HTTP':
            LOG.debug("Not FTP data, log check ended.")
            return

        log_check = re.compile(FTP_LOG_DETAIL)
        user_check = re.compile(FTP_USER_DETAIL)

        res = log_check.match(line)
        if res is None:
            return

        today = datetime.date.today()
        date_time = str(today.year) + ' ' + res.group('date_time')
        date_time = time.strptime(date_time, "%Y %b %d %H:%M:%S")
        log_data['dateTime'] = time.strftime('%Y-%m-%d %H:%M:%S', date_time)

        log_data['device'] = res.group('device')
        log_data['detail'] = res.group('detail')

        res = user_check.match(line)
        if res is None:
            return

        log_data['ftpUser'] = res.group('user')
        log_data['userIp'] = res.group('user_ip')
        log_data['computer'] = res.group('pc_name')
        if res.group('file') != 'Administration':
         log_data['file'] = res.group('file')
        else:
         log_data['file'] = '---'
        log_data['action'] = res.group('action')

        for session, data in self.session_data.items():
            if 'loginIp' not in data or 'clientIp' not in data\
                    or 'user' not in data:
                continue

            if data['loginIp'] == log_data['userIp'] or\
               data['clientIp'] == log_data['userIp']:
                print data
                log_data['user'] = data['user']

        self.all_ftp.append(log_data)

    def get_login_ip(self, line, sessionId):
        log_check = re.compile(F5_NEW_SESSION_LOGIN + F5_LOGIN_IP)
        res = log_check.match(line)

        if res is not None:
            self.session_data[sessionId]['loginIp'] = res.group('login_ip')
            self.session_data[sessionId]['clientAction'] = 'User login'
            return res.group('login_ip')
        return 'None'

    def get_login_user(self, line, sessionId):
        log_check = re.compile(F5_LOGIN_USER)
        res = log_check.match(line)

        if res is not None:
            self.session_data[sessionId]['user'] = res.group('user')
            return res.group('user')
        return 'None'

    def get_assign_ip(self, line, sessionId):
        log_check = re.compile(F5_ASSIGN_IP)
        res = log_check.match(line)

        if res is not None:
            self.session_data[sessionId]['clientIp'] = res.group('client_ip')
            self.session_data[sessionId]['clientAction'] = 'User get IP'
            return res.group('client_ip')
        """else:
            log_check = re.compile(F5_ASSIGN_IP2)
            res = log_check.match(line)
            if res is not None:
              self.session_data[sessionId]['clientIp'] = res.group('client_ip')
              self.session_data[sessionId]['clientAction'] = 'User get IP'
              return res.group('client_ip')"""

        return 'None'


    def get_login_mac(self, line, sessionId):
        log_check = re.compile(F5_LOGIN_MAC)
        res = log_check.match(line)

        if res is not None:
            self.session_data[sessionId]['loginMac'] = res.group('loginMac')
            return res.group('loginMac')
        return 'None'


    def get_logout_session(self, line):
        log_check = re.compile(F5_SESSION_LOGOUT)
        res = log_check.match(line)

        if res is not None:
            self.session_data[
                res.group('session_id')]['logOut'] = res.group('log_out')
            self.session_data[
                res.group('session_id')]['clientAction'] = 'User logout'
            return res.group('session_id')

        return None

    def get_f5_json(self):
        return json.dumps(self.all_f5)

    def get_ftp_json(self):
        return json.dumps(self.all_ftp)

#check log when changed
def checkLogFile():
 rows =list()
 with tools.init_session() as session:
  rows = session.query(schema.LogFileDetail).all()
  if rows == []:
    saveLogFile(True)
    return False
  else:
    if rows[0].log_file_date == getFileOneDate():
      return False
    else:
      return True

def saveLogFile(z=False):
 if z:
  with tools.init_session() as session:
    try:
      log_date = schema.LogFileDetail(log_file_date=getFileOneDate())
      session.add(log_date)
    except Exception as e:
      message = "General Error: %s" % str(e)
      print message
 else:
   rows =list()
   with tools.init_session() as session:
     rows = session.query(schema.LogFileDetail).all()
     rows[0].log_file_date=getFileOneDate()


def getFileOneDate():
 ff=open(LOG_FILE)
 ss=ff.readline()

 ff.close()
 return ss[0:15]


def get_log(logfile):
    log_data = Logparse()
    today = datetime.datetime.now()
    preTime=time.time()

    while True:
	if (time.time()-preTime) > 3600:
         setLogPosition(logfile.tell())
	 break
	
        
        if checkLogFile():
         break
         

        line = logfile.readline()
        if not line:
            time.sleep(0.5)
            log_data.save_to_database()
            continue

        yield line


def log_handler():
    log_data = Logparse()
    log_data.getLogTimeForCheck()

    while True:
        print 'Logparse thread start.'
        LOG.info("Logparse thread start.")
        logfile = open(LOG_FILE)
	if checkLogFile():
	  saveLogFile()
	  setLogPosition(0)
	else:
	  logfile.seek(getLogPosition())
	
        for line in get_log(logfile):
	  #print '!!!line=',line
	  log_data.full_log_check(line)
	  setLogPosition(logfile.tell())

        print 'save data'
        log_data.save_to_database()

#get log file position
def getLogPosition():
 rows =list()
 with tools.init_session() as session:
   rows = session.query(schema.LogFileDetail).all()
   if rows != []:
     if rows[0].last_position != None:
       return rows[0].last_position
     else:
       setLogPosition(p=0)
   else:
     setLogPosition(p=0,z=True)
   return 0


#set log file position
def setLogPosition(p,z=False):
 #print '!!!p=',p,' z=',z
 if z:
  with tools.init_session() as session:
    try:
      last_p = schema.LogFileDetail(last_position=p)
      session.add(last_p)
    except Exception as e:
      message = "General Error: %s" % str(e)
      print message
 else:
  rows=list()
  with tools.init_session() as session:
    rows = session.query(schema.LogFileDetail).all()
    rows[0].last_position=p

# if __name__ == "__main__":
#     log_data = Logparse()

#     logfile = open(LOG_FILE)
#     for line in get_log(logfile):
#         log_data.full_log_check(line)
