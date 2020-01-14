import json
import logging
from datetime import datetime

# Flask library
from flask import Flask, flash
from flask import render_template, request, g, abort
from flask import redirect, url_for, jsonify, Response
from flask import Blueprint
from flask.helpers import make_response

# Sunny Bank
from sunnybank.database import schema, tools
from sunnybank.module.form import LoginForm, TimeForm
from sunnybank.module.logparse import Logparse
from sunnybank.module.logparse import LOG_FILE
from sunnybank.module.login import User
from sunnybank.module.sid_manager import SidManager
from sunnybank.api.report import get_log

auth_api = Blueprint('auth_api', __name__)
SID_MANAGER = SidManager.get_instance()
LOG = logging.getLogger("FlaskApp")


@auth_api.route('/login', methods=['GET', 'POST'])
def login():
    form = LoginForm(request.form)

    if request.method == 'POST' and form.validate():
        user = User(form.user_id.data, form.passwd.data)
        keep_time = (60 * 60 * 24 if form.remember_me.data else 60 * 60)

        if ('admin', 'admin123') == (user.name, user.passwd) \
                or user.do_authentication():
            role = 'admin'

            time_form = TimeForm()
            index = get_log()
            data = {'data_id': index}
            main_page = render_template(
                'f5log.html', user=user.name, form=time_form,
                action=0, index=data)
            sid = SID_MANAGER.add_sid(user.name, role, keep_time)
            response = make_response(main_page)

            response.set_cookie('USER', value=user.name, max_age=keep_time)
            response.set_cookie('SID', value=sid, max_age=keep_time)

            return response
    messages = 'Login failed, check account and password!'
    flash(messages)
    LOG.error(messages)

    return render_template('login.html', form=form)


@auth_api.route('/logout', methods=['GET'])
def logout():
    SID_MANAGER.check_sid(request.cookies.get('SFC_SID',
                          default=None, type=str))
    form = LoginForm(request.form)

    return render_template('login.html', form=form)
