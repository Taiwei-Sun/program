#LOG_FILE = '/var/log/messages'
#LOG_FILE = '/root/python/sunnybank/log_data/test_log2'
LOG_FILE = 'log_data/messages20170314'
LOG_TIME = 'logtime'
HOST = '0.0.0.0'
PORT = 8080
DEBUG = True
ADDRESS = "192.168.20.6"


# Database setting
DB_USER = 'root'
DB_PASS =  'root123'
DB_CONNECT_STRING = 'mysql+mysqldb://%s:%s@localhost/Sunny' % (DB_USER, DB_PASS)

# Log configuration file
LOG_CONF = './config/log.conf'
