
class ExternalException(Exception):
    ''' External Exception ==> xxxException '''

    def __init__(self, msg=None):
        Exception.__init__(self, msg)
        self.msg = msg


class AuthException(ExternalException):
    default_status_code = 401


class AuthorizationException(ExternalException):
    default_status_code = 403


class AccountingException(ExternalException):
    default_status_code = 403
