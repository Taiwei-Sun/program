# Python library
import re
import time
import datetime
import json
import logging

from sunnybank.database import tools, schema
from sunnybank.config.enviroment import LOG_FILE

LOG = logging.getLogger('FlaskApp')

# Regular match formate
# F5
LOG_DATE_TIME = r'(?P<date_time>\w+\s+\d+ \d+:\d+:\d+) '
F5_DEVICE_NAME_AND_LOG_LEVEL = r'(?P<device>\w+) (?P<log_level>\w+) '
F5_SESSION_ID = r'(?P<pid>.+)\[\d+\]: .+:\d+: (?P<session_id>\w+): '
F5_NEW_SESSION_LOGIN = r'.* (?P<new_session_id>\w+): New '
F5_LOGIN_IP = r'session from client IP (?P<login_ip>\d+.\d+.\d+.\d+)'
F5_LOGIN_USER = r'.* \'session.logon.last.username\' set to (?P<user>\'\w+\')'
F5_ASSIGN_IP = r'.* Assigned PPP Dynamic IPv4: (?P<client_ip>\d+.\d+.\d+.\d+) '
F5_LOGIN_MAC = r'.*mac_address is (?P<loginMac>\w+:\w+:\w+:\w+:\w+:\w+)'
F5_LOG_DETAIL = r'(?P<detail>.*)'
F5_FULL_LOG = LOG_DATE_TIME + F5_DEVICE_NAME_AND_LOG_LEVEL + F5_SESSION_ID\
    + F5_LOG_DETAIL
F5_SESSION_LOGOUT = LOG_DATE_TIME + F5_DEVICE_NAME_AND_LOG_LEVEL\
    + F5_SESSION_ID + r'.* (?P<log_out>Session deleted due to user logout request\.)'

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
        session_id = self.get_logout_session(line)

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
        if res is not None and res.group('connection_type') != 'FTP':
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
        log_data['file'] = res.group('file')
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

    def get_login_user(self, line, sessionId):
        log_check = re.compile(F5_LOGIN_USER)
        res = log_check.match(line)

        if res is not None:
            self.session_data[sessionId]['user'] = res.group('user')

    def get_assign_ip(self, line, sessionId):
        log_check = re.compile(F5_ASSIGN_IP)
        res = log_check.match(line)

        if res is not None:
            self.session_data[sessionId]['clientIp'] = res.group('client_ip')
            self.session_data[sessionId]['clientAction'] = 'User get IP'

    def get_login_mac(self, line, sessionId):
        log_check = re.compile(F5_LOGIN_MAC)
        res = log_check.match(line)

        if res is not None:
            self.session_data[sessionId]['loginMac'] = res.group('loginMac')

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


def get_log(logfile):
    log_data = Logparse()
    today = datetime.datetime.now()

    while True:
        now = datetime.datetime.now()

        if now.day != today.day and now.hour > 0:
            break

        line = logfile.readline()
        if not line:
            time.sleep(0.1)
            log_data.save_to_database()
            continue

        yield line


def log_handler():
    log_data = Logparse()

    while True:
        LOG.info("Logparse thread start.")
        logfile = open(LOG_FILE)
        for line in get_log(logfile):
            log_data.full_log_check(line)

        print 'save data'
        log_data.save_to_database()


# if __name__ == "__main__":
#     log_data = Logparse()

#     logfile = open(LOG_FILE)
#     for line in get_log(logfile):
#         log_data.full_log_check(line)
