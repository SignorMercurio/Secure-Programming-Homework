from db import db
from model.User import User
from flask import g, request, abort, Blueprint
from basicAuth import httpAuth
from JSONResponse import res

auth_bp = Blueprint('auth_bp', __name__, url_prefix='/')

#just fot test and init
@auth_bp.route('/api/users/reg', methods=['POST'])
def new_user():
    username = request.json.get('username')
    password = request.json.get('password')
    RSApubkey = request.json.get('rsaPk')
    DSApubkey = request.json.get('dsaPk')

    if username is None or password is None:
        abort(res(code=-1, msg='Missing arguments'))
    if User.query.filter_by(username=username).first() is not None:
        abort(res(code=-2, msg='User already exists'))
    user = User(username=username, RSApubkey=RSApubkey, DSApubkey=DSApubkey)
    user.hash_password(password)
    db.session.add(user)
    db.session.commit()
    return res(data={
        'username': user.username
    })


@auth_bp.route('/api/getPubkey', methods=['POST'])
def get_pubkey():
    username = request.json.get('username')
    type = request.json.get('type')

    user = User.query.filter_by(username=username).first()
    if not user:
        abort(res(code=-3, msg='User does not exist'))
    if type == 'RSA':
        return res(data={
            'pubkey': user.RSApubkey
        })
    else:
        return res(data={
            'pubkey': user.DSApubkey
        })

@auth_bp.route('/api/token')
@httpAuth.login_required
def get_auth_token():
    token = g.user.generate_auth_token(36000)
    return res(data={
        'token': token.decode('ascii'),
        'username': g.user.username,
        'duration': 36000
    })
