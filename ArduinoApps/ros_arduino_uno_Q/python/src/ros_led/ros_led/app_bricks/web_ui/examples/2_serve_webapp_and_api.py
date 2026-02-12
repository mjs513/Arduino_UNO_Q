# SPDX-FileCopyrightText: Copyright (C) ARDUINO SRL (http://www.arduino.cc)
#
# SPDX-License-Identifier: MPL-2.0

# EXAMPLE_NAME = "Serve a web application and its API"
from ros_led.app_utils import App
from ros_led.app_bricks.web_ui import WebUI


ui = WebUI()
ui.expose_api("GET", "/hello", lambda: {"message": "Hello, world!"})

App.run()  # This will block until the app is stopped
