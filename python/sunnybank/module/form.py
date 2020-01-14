from flask.ext.wtf import Form
from wtforms import StringField, PasswordField, BooleanField
from wtforms.validators import DataRequired


class LoginForm(Form):
    user_id = StringField('user_id', validators=[DataRequired()])
    passwd = PasswordField('passwd:', validators=[DataRequired()])
    remember_me = BooleanField('remember_me', default=False)


class TimeForm(Form):
    start_time = StringField('start_time', validators=[DataRequired()])
    end_time = StringField('end_time', validators=[DataRequired()])


class UserCreateForm(Form):
    user_id = StringField('user_id', validators=[DataRequired()])
    user_paw = StringField('user_paw:', validators=[DataRequired()])
    creator = StringField('creator', validators=[DataRequired()])
