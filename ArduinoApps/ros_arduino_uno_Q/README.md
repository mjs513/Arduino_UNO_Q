# 🦾 ros_arduino_uno_Q

## Table of Contents
- [About](#about)
- [Demostration](#demostration)
- [Architecture Overview](#architecture-overview)
- [Run package](#run-package)
- [License](#license)

## About

This project demonstrates how to turn the Arduino UNO Q into a fully ROS-enabled embedded platform by combining:
* ROS 2 Jazzy running inside a Linux container on the Qualcomm MPU
* The Arduino Bridge RPC mechanism, which links the Linux MPU with the onboard STM32 microcontroller
* A minimal ROS 2 subscriber that listens to a boolean topic and remotely toggles the LED on the STM32 side
* Custom Arduino firmware that exposes the set_led_state() RPC function

The goal is to treat the Arduino UNO Q not just as a microcontroller, but as a mini embedded robot computer capable of running ROS 2 natively while still accessing hardware in real time via the MCU.

This approach allows seamless integration with ROS-based robotics workflows, bringing the UNO Q into modern robotics ecosystems.

## Demostration


https://github.com/user-attachments/assets/c57c8a9a-bfd8-4649-a59d-0abdea663d25



## Architecture Overview

The UNO Q features a dual-processor architecture:

Qualcomm MPU (Linux Debian 13)
– Runs Docker containers
– Executes ROS 2 nodes
– Publishes/subscribes to ROS topics

STM32 Real-Time MCU
– Controls GPIO and peripherals
– Exposes functions to Linux through RPC

This project uses that architecture to create a seamless ROS-native interface:
a ROS 2 node subscribes to `/led_status` (type `std_msgs/Bool`), and when messages arrive, it calls the RPC method `set_led_state()` on the MCU.

## Run Package
Follow these steps to run the full ROS 2 + RPC LED control pipeline on the Arduino UNO Q:

1) Update the UNO Q firmware

Follow the official update procedure described in [Flashing a New Image to the UNO Q](https://docs.arduino.cc/tutorials/uno-q/update-image/)

2) Initialize the Linux environment on the UNO Q

Set up Debian 13 on the Qualcomm MPU as described in this guide: [UNO Q as a Single-Board Computer](https://docs.arduino.cc/tutorials/uno-q/single-board-computer/)

3) Clone this repository inside the `ArduinoApps` directory

On the UNO Q Linux side, navigate to the Arduino apps folder and clone the project:

```bash
cd ~/ArduinoApps
git clone https://github.com/miguelgonrod/ros_arduino_uno_Q.git
cd ros_arduino_uno_Q
```

4) Upload the `.ino` firmware using Arduino AppLab and run the Docker container with ROS 2 Jazzy

Use the provided `run.sh` script, this one will load the provided `.ino` file from the sketch/ directory, and upload it to the UNO Q, also will build and start the container:

```bash
cd python
chmod +x run.sh
./run.sh
```
This firmware exposes the RPC function:

```cpp
set_led_state(bool state)
```

This launches a minimal ROS 2 Jazzy environment with the Arduino RPC bridge pre-installed.

5) Open a tmux session inside the container

Since you will run multiple ROS commands inside the container, start a tmux session:

```bash
tmux
```

6) In the first tmux pane: run the ROS 2 subscriber node

Inside the Docker container:
```bash
ros2 run ros_led led
```


You should see:
```bash
[INFO] [my_ros2_node]: Node started
```

7) In a second tmux pane: publish the LED commands

To open a second tmux pane push in your keyboard: `ctrl b` and then `c`

Turn LED OFF:
```bash
ros2 topic pub /led_status std_msgs/Bool "data: false" --once
```

Turn LED ON:
```bash
ros2 topic pub /led_status std_msgs/Bool "data: true" --once
```

You should see the LED change state immediately, and the logs in the subscriber pane will confirm the RPC call.

## License

ros_arduino_uno_Q is available under the BSD-3-Clause license. See the LICENSE file for more details.
