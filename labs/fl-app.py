#!/usr/bin/python

import RPi.GPIO as GPIO # Import Raspberry Pi GPIO library
from time import sleep # Import the sleep function from the time module
GPIO.setwarnings(False) # Ignore warning for now
GPIO.setmode(GPIO.BOARD) # Use physical pin numbering
GPIO.setup(21, GPIO.OUT, initial=GPIO.LOW) # Set pin 8 to be an output pin and set initial value to low (off)

from flask import Flask
from flask_restful import Resource, Api, reqparse

app = Flask(__name__)
api = Api(app)


class LedBasis(Resource):

    def get(self, basis):
        input = str(basis).lower()
        if input=="on":
            GPIO.output(21, GPIO.HIGH)
            return ("Led is on")
        else:
            GPIO.output(21, GPIO.LOW)
            return ("Led is off")
        

api.add_resource(LedBasis, '/led/<basis>')


if __name__ == '__main__':
    app.run(debug=True,host='0.0.0.0', port=80)
