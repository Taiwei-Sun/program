import json
import logging
import uuid
from datetime import datetime, date
import hashlib

# Flask library
from flask import Flask
from flask import render_template, request, g, abort
from flask import redirect, url_for, jsonify, Response
from flask import Blueprint
from flask.helpers import make_response

# Sunny Bank
from sunnybank.database import schema, tools
from sunnybank.module.form import TimeForm, UserCreateForm
from sunnybank.module.logparse import Logparse
from sunnybank.module.logparse import LOG_FILE
from sunnybank.module.login import User
from sunnybank.module.sid_manager import SidManager

report_api = Blueprint('report_api', __name__)
data = dict()
LOG = logging.getLogger("FlaskApp")


@report_api.route('/')
@report_api.route('/Index')
def main_page():
    # log_json = f5log_view()

    user = request.cookies.get('USER', default=None, type=str)
    return render_template('f5log.html', user=user)


@report_api.route('/data/<data_id>')
def get_data(data_id):
    global data
    print data_id

    if data_id not in data:
        return None

    log_data = data[data_id]
    del data[data_id]

    return log_data


def get_default_f5_statistic():
    data = {
        'user': None,
        'loginTime': None,
        'logoutTime': None,
        'sessionId': None,
        'loginMac': None,
        'loginIp': None,
        'clientIp': None
    }

    return data


def get_statistic(start_time=None, end_time=None):
    statistics = list()
    stat_data = dict()
    log_type = 'F5'
    global data

    if start_time is None or end_time is None:
        today = True
    else:
        start_time = datetime.strptime(start_time, "%m/%d/%Y")
        end_time = datetime.strptime(end_time, "%m/%d/%Y")
        today = False

        if end_time < start_time:
            today = True

    with tools.init_session() as session:
        if today:
            all_rows = session.query(schema.LogData)\
                .filter_by(log_type=log_type)\
                .filter(
                    schema.LogData.date_time >= date.today(),schema.LogData.detail.contains('PPP tunnel'))
        else:
            all_rows = session.query(schema.LogData)\
                .filter_by(log_type=log_type)\
                .filter(schema.LogData.date_time.between(
                    start_time, end_time),schema.LogData.detail.contains('PPP tunnel'))

        for row in all_rows:
            if row.f5_session_id not in stat_data:
                stat_data[row.f5_session_id] = get_default_f5_statistic()
                stat_data[row.f5_session_id]['sessionId'] = row.f5_session_id

            if row.f5_user is not None:
                stat_data[row.f5_session_id]['user'] = row.f5_user

            if row.f5_user_action is not None:
                if row.f5_user_action == 'User login':
                    stat_data[row.f5_session_id][
                        'loginTime'] = str(row.date_time)
                elif row.f5_user_action == 'User logout':
                    stat_data[row.f5_session_id][
                        'logoutTime'] = str(row.date_time)

            if row.f5_user_mac is not None:
                stat_data[row.f5_session_id]['loginMac'] = row.f5_user_mac

            if row.f5_login_ip is not None:
                stat_data[row.f5_session_id]['loginIp'] = row.f5_login_ip

            if row.f5_client_ip is not None:
                stat_data[row.f5_session_id]['clientIp'] = row.f5_client_ip

        for session_id, value in stat_data.items():
            statistics.append(value)

    index = uuid.uuid1().hex
    data[index] = json.dumps(statistics)

    return index


def get_log(start_time=None, end_time=None, log_type='F5'):
    logs = list()
    global data

    if start_time is None or end_time is None:
        today = True
    else:
        start_time = datetime.strptime(start_time, "%m/%d/%Y")
        end_time = datetime.strptime(end_time, "%m/%d/%Y")
        today = False

        if end_time < start_time:
            today = True

    with tools.init_session() as session:
        if today:
            print 'Get today data.'
            all_rows = session.query(schema.LogData).filter_by(log_type=log_type).filter(schema.LogData.date_time >= date.today(),schema.LogData.detail.contains('PPP tunnel'))

        else:
            print 'Get time range data.'
            all_rows = session.query(schema.LogData)\
                .filter_by(log_type=log_type)\
                .filter(schema.LogData.date_time.between(
                    start_time, end_time),schema.LogData.detail.contains('PPP tunnel'))

        for row in all_rows:
            logs.append(row.get_dict())

    index = uuid.uuid1().hex
    data[index] = json.dumps(logs)

    return index


def get_user():
    users = list()
    global data

    with tools.init_session() as session:
        all_rows = session.query(schema.User)

        for row in all_rows:
            users.append(row.get_dict())

    index = uuid.uuid1().hex
    data[index] = json.dumps(users)

    return index


@report_api.route('/F5stat/get', methods=['GET', 'POST'])
def get_f5_stat():
    user = request.cookies.get('USER', default=None, type=str)
    form = TimeForm(request.form)

    index = get_statistic(form.start_time.data,
                          form.end_time.data)
    data = {'data_id': index}

    return render_template(
        'f5Statistics.html', user=user, form=form, action=2,
        index=data)


@report_api.route('/F5log', methods=['GET', 'POST'])
def f5log():
    user = request.cookies.get('USER', default=None, type=str)
    form = TimeForm(request.form)

    log_type = 'F5'
    index = get_log(form.start_time.data,
                    form.end_time.data,
                    log_type)
    data = {'data_id': index}

    return render_template(
        'f5log.html', user=user, form=form, action=0,
        index=data)


@report_api.route('/FTPlog', methods=['GET', 'POST'])
def ftplog():
    user = request.cookies.get('USER', default=None, type=str)
    form = TimeForm(request.form)

    log_type = 'FTP'
    index = get_log(form.start_time.data,
                    form.end_time.data,
                    log_type)
    data = {'data_id': index}

    return render_template(
        'ftplog.html', user=user, form=form, action=1,
        index=data)


@report_api.route('/Users', methods=['GET', 'POST'])
def get_users():
    user = request.cookies.get('USER', default=None, type=str)

    with tools.init_session() as session:
        if request.method == "POST":
            form = UserCreateForm(request.form)
            name = form.user_id.data
            paw = form.user_paw.data
            creator = form.creator.data

	    rows=list()
	    rows=session.query(schema.User).filter_by(name=name).all()
	    if rows == []:
             try:
                user_data = schema.User(name=name,paw=hashlib.sha1(paw).hexdigest(),creator=creator)
                session.add(user_data)

             except Exception as e:
                message = "General Error: %s" % str(e)
                print message
	    else:
             rows[0].paw=hashlib.sha1(paw).hexdigest()

    index = get_user()
    data = {'data_id': index}

    return render_template('users.html', user=user, index=data)


@report_api.route('/Users/delete', methods=["POST"])
def del_users():
    user = request.cookies.get('USER', default=None, type=str)
    form = UserCreateForm(request.form)
    name = form.user_id.data

    if name != 'admin':
     with tools.init_session() as session:
      session.query(schema.User).filter_by(name=name).delete()

    index = get_user()
    data = {'data_id': index}

    return render_template('users.html', user=user, index=data)
