from flask import Flask

from .views import blueprint

app = Flask(__name__)
app.register_blueprint(blueprint)

print "1 ",__name__
