import urllib2

from flask import Flask
from flask_restful import Resource, Api, reqparse

app = Flask(__name__)
api = Api(app)


class LedBasis(Resource):

    def get(self, basis):
		input = str(basis).lower()
		if input=="on":
			response = urllib2.urlopen('https://unreduced-warthog-8220.dataplicity.io/led/on')
			return ("LED is on")
		else:
			response = urllib2.urlopen('https://unreduced-warthog-8220.dataplicity.io/led/off')
			return ("LED is off")



api.add_resource(LedBasis, '/led/<basis>')


if __name__ == '__main__':
    app.run(host='10.142.0.3', debug=True, port=5002)
