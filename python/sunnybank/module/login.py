import ldap
import logging
from sunnybank.config.enviroment import ADDRESS

from sunnybank.database import tools, schema
import hashlib

# print conn.simple_bind_s('scott', '1qaz@WSX')
LOG = logging.getLogger("FlaskApp")


class User(object):

    def __init__(self, name, passwd):
        self.name = name
        self.passwd = passwd

    def do_authentication(self):
     if self.local_auth():
      return True
     if self.AD_auth():
      return True

     return False

    def local_auth(self):
     with tools.init_session() as session:
      rows= session.query(schema.User).filter_by(name=self.name).filter(schema.User.paw == hashlib.sha1(self.passwd).hexdigest()).all()
      if rows == []:
       return False
      else:
       return True

        

    def AD_auth(self):
        conn = ldap.initialize('ldap://' + ADDRESS)
        conn.protocol_version = 3
        conn.set_option(ldap.OPT_REFERRALS, 0)

        try:
            result = conn.simple_bind_s(self.name, self.passwd)

        except ldap.INVALID_CREDENTIALS:
            message = "Invalid credentials, check login value first."
            LOG.error(message)

            return False

        except ldap.SERVER_DOWN:
            message = "Ldap server is down, check server first."
            LOG.error(message)

            return False

        except ldap.LDAPError, e:
            if type(e.message) == dict and 'desc' in e.message:
                message = "Other LDAP error: " + e.message['desc']
                LOG.error(message)

                return False
            else:
                message = "Other LDAP error: " + str(e)
                LOG.error(message)

                return False
        finally:
            conn.unbind_s()

        message = "Login successed."
        LOG.error(message)

        return result
