import time

from arduino.app_utils import App, Bridge

def btn_a_pressed(btn_state: int):
  print("BTN A: %d" % (btn_state))

def btn_b_pressed(btn_state: int):
  print("BTN B: %d" %  (btn_state))
  
def btn_c_pressed(btn_state: int):
  print("BTN C: %d" % (btn_state))

# Used only for sync purposes.
def linuxStarted():
  return True
  
print("Hello world!")
Bridge.provide("btn_a_pressed", btn_a_pressed)
Bridge.provide("btn_b_pressed", btn_b_pressed)
Bridge.provide("btn_c_pressed", btn_c_pressed)
Bridge.provide("linux_started", linuxStarted)

def loop():
    """This function is called repeatedly by the App framework."""
    # You can replace this with any code you want your App to run repeatedly.
    time.sleep(10)


# See: https://docs.arduino.cc/software/app-lab/tutorials/getting-started/#app-run
App.run(user_loop=loop)
