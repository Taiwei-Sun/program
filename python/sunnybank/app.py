import gevent.monkey
gevent.monkey.patch_all(thread=True, aggressive=True)

from flask import Flask, request, redirect, url_for
import logging
import logging.config
import os

from sunnybank.api.report import report_api
from sunnybank.api.auth import auth_api
from sunnybank.module import logparse
from sunnybank.config.enviroment import HOST, PORT, DEBUG, LOG_CONF
from sunnybank.module.sid_manager import SidManager

app = Flask(__name__)
app.config.update(
    DEBUG=DEBUG,
    WTF_CSRF_ENABLED=True,
    SECRET_KEY=os.urandom(24)
)

logging.config.fileConfig(LOG_CONF)
LOG = logging.getLogger('FlaskApp')


def before_request():
    if not request.endpoint:
        raise AttributeError('not a endpoint request')

    sid = request.cookies.get('SID', default=None, type=str)

    if not SidManager.get_instance().check_sid(sid):
        LOG.error("Unverify connected, please login first.")
        return redirect(url_for('auth_api.login'))

    print sid

if __name__ == '__main__':
    report_api.before_request(before_request)
    app.register_blueprint(report_api)
    app.register_blueprint(auth_api)

    sid_mgr = SidManager.get_instance()
    sid_mgr.resurrect()
    logparse_thread = gevent.spawn(logparse.log_handler)

    app.run(host=HOST, port=PORT, debug=False)
    logparse_thread.join()
