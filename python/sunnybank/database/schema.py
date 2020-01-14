# Python library
import json

# SQLAlchemy library
from sqlalchemy.ext.declarative import declarative_base
from sqlalchemy import Column, Integer, String, text
from sqlalchemy import ForeignKey, Boolean
from sqlalchemy.types import TIMESTAMP, TEXT, DateTime
from sqlalchemy.orm import relationship

DB_BASE = declarative_base()


class LogData(DB_BASE):
    __tablename__ = 'LogData'

    # Table columns
    id = Column(Integer, primary_key=True, autoincrement=True, nullable=False)
    log_level = Column(String(32))
    log_type = Column(String(32), nullable=False)
    device = Column(String(32))
    detail = Column(TEXT, nullable=False)
    date_time = Column(TIMESTAMP, nullable=False)
    f5_user = Column(String(255))
    f5_session_id = Column(String(32))
    f5_login_ip = Column(String(32))
    f5_client_ip = Column(String(32))
    f5_user_action = Column(String(255))
    f5_user_mac = Column(String(255))
    ftp_user = Column(String(255))
    ftp_user_ip = Column(String(32))
    ftp_user_computer = Column(String(50))
    ftp_user_access_file = Column(String(255))
    ftp_user_action = Column(String(32))

    def get_dict(self):
        if self.log_type == 'F5':
            data = {
                'dateTime': str(self.date_time),
                'logLevel': self.log_level,
                'user': self.f5_user,
                'loginMac': self.f5_user_mac,
                'sessionId': self.f5_session_id,
                'clientIp': self.f5_client_ip,
                'loginIp': self.f5_login_ip,
                'clientAction': self.f5_user_action,
                'detail': self.detail
            }

        else:
            data = {
                'dateTime': str(self.date_time),
                'ftpUser': self.ftp_user,
                'user': self.f5_user,
                'userIp': self.ftp_user_ip,
                'computer': self.ftp_user_computer,
                'action': self.ftp_user_action,
                'file': self.ftp_user_access_file,
                'detail': self.detail
            }

        return data


# User object
class User(DB_BASE):
    __tablename__ = 'User'

    # Table columns
    id = Column(Integer, primary_key=True, autoincrement=True, nullable=False)
    name = Column(String(255))
    paw = Column(String(41))
    creator = Column(String(255))
    create_time = Column(TIMESTAMP,
                         default=text('CURRENT_TIMESTAMP'),
                         onupdate=text('CURRENT_TIMESTAMP'),
                         server_default=text('CURRENT_TIMESTAMP '
                                             'ON UPDATE CURRENT_TIMESTAMP'),
                         nullable=False)

    def get_dict(self):
        data = {
            'createTime': str(self.create_time),
            'user': self.name,
            'password': self.paw,
            'creator': self.creator
        }

        return data


# Log file object
class LogFileDetail(DB_BASE):
    __tablename__ = 'LogFileDetail'

    id = Column(Integer, primary_key=True, autoincrement=True, nullable=False)
    log_file_date = Column(String(16))
    last_position = Column(Integer)
