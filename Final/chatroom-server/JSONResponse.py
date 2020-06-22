from flask import jsonify

def res(code=0, msg='success', data={}):
    return jsonify({
        "code": code,
        "msg": msg,
        "data": data
    })