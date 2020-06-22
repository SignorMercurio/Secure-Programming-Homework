from flask import abort, g
from flask_httpauth import HTTPBasicAuth
from model.User import User
from JSONResponse import res

httpAuth = HTTPBasicAuth()

@httpAuth.verify_password
def verify_password(username_or_token, password):
    # first try to authenticate by token
    if username_or_token is None or username_or_token == '':
        abort(res(code=-4, msg='User not logged in'))
    elif len(username_or_token) >= 100:
        token = username_or_token
        user = User.verify_auth_token(token)
        if not user:
            abort(res(code=-5, msg='Invalid token'))
        else:
            g.user = user
            return True
    else:
        username = username_or_token
        # try to authenticate with username/password
        user = User.query.filter_by(username=username).first()
        if not user or not user.verify_password(password):
            abort(res(code=-6, msg='Invalid username or password'))
        else:
            g.user = user
            return True
