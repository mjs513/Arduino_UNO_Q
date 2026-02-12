import time
from arduino.app_utils import *

def bridgePrint(text):
  print(text)

def loop():
  """This function is called repeatedly by the App framework."""
  # You can replace this with any code you want your App to run repeatedly.

print("Hello world!")
Bridge.provide("bridge_print", bridgePrint)

App.run(user_loop=loop)