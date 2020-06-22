from db import db
from flask import current_app
import jwt
import time


class User(db.Model):
    __tablename__ = 'users'
    id = db.Column(db.Integer, primary_key=True)
    username = db.Column(db.String(32), index=True)
    password = db.Column(db.String(128))
    RSApubkey = db.Column(db.String(800))
    DSApubkey = db.Column(db.String(800))

    def hash_password(self, password):
        self.password = password

    def verify_password(self, password):
        return self.password == password

    def generate_auth_token(self, expires_in=36000):
        return jwt.encode(
            {'id': self.id, 'exp': time.time() + expires_in},
            current_app.config['SECRET_KEY'], algorithm='HS256')

    @staticmethod
    def verify_auth_token(token):
        try:
            data = jwt.decode(token, current_app.config['SECRET_KEY'], algorithms=['HS256'])
        except:
            print("verify_auth_token Exception")
            return
        return User.query.get(data['id'])
