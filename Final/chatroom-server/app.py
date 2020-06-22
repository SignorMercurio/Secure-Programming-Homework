import os
from flask import Flask
import config
from db import db
from flask_cors import CORS
from flask_socketio import SocketIO, send

# initialization
app = Flask(__name__)
app.config.from_object(config)

# extensions
db.init_app(app)
socketio = SocketIO(app, cors_allowed_origins="*")
CORS(app)

@socketio.on('msg')
def response(json):
    print(str(json))
    send(json, json=True, broadcast=True)

from api.auth import auth_bp
# register all blueprint
app.register_blueprint(auth_bp)

if __name__ == '__main__':
    if not os.path.exists('db.sqlite'):
        with app.app_context():
            db.create_all()
    socketio.run(app, debug=False, host="localhost", port=9095)
