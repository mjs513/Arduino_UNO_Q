#!/usr/bin/bash

arduino-app-cli app start user:ros_arduino_uno_Q

docker build -t ros_jazzy_ws python/.

docker run -it --net=host --name ros_jazzy_container -v $(pwd)/src:/workspace/src -v /var/run/arduino-router.sock:/var/run/arduino-router.sock ros_jazzy_ws
