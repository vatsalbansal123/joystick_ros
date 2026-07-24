# joystick_ros

This ROS 2 project allows you to control the `turtlesim` turtle using a physical analog joystick connected to an Arduino. The Arduino reads the analog values from the joystick and sends them over the serial port. A ROS 2 node then reads this serial data, converts it into velocity commands, and publishes them to the `/turtle1/cmd_vel` topic.

## System Architecture

The data flows as follows:

1.  **Joystick**: The physical X and Y axes are connected to an Arduino's analog pins.
2.  **Arduino**: Runs the `joystick_serial.ino` sketch, which reads the analog values from pins A0 and A1 and writes them to the serial port as a comma-separated string (e.g., `512,512\n`).
3.  **ROS 2 Node (`joystick_node`)**:
    *   Connects to the Arduino's serial port.
    *   Reads the incoming data line by line.
    *   Parses the X and Y values.
    *   Converts these values into a `geometry_msgs/msg/Twist` message (linear velocity from the Y-axis, angular velocity from the X-axis).
    *   Publishes the `Twist` message to the `/turtle1/cmd_vel` topic.
4.  **Turtlesim**: The `turtlesim_node` subscribes to `/turtle1/cmd_vel` and moves the turtle accordingly.

## Requirements

### Hardware
*   An Arduino board (e.g., Arduino Uno)
*   A 2-axis analog joystick module
*   Jumper wires

### Software
*   ROS 2 (Humble, Iron, etc.)
*   Arduino IDE
*   Python `pyserial` library

## Installation and Setup

### 1. Hardware Connection

Connect the joystick module to your Arduino as follows:
*   **GND** on the joystick -> **GND** on the Arduino
*   **+5V** on the joystick -> **5V** on the Arduino
*   **VRx** (X-axis output) -> **A0** on the Arduino
*   **VRy** (Y-axis output) -> **A1** on the Arduino

### 2. Arduino Setup

1.  Open the `src/joystick_serial.ino` file in the Arduino IDE.
2.  Connect your Arduino to your computer via USB.
3.  From the Arduino IDE, select your board type (e.g., `Tools > Board > Arduino AVR Boards > Arduino Uno`) and port (`Tools > Port`).
4.  Upload the sketch to your Arduino.

### 3. ROS 2 Package Setup

1.  Clone this repository into your ROS 2 workspace's `src` directory:
    ```bash
    cd ~/ros2_ws/src
    git clone https://github.com/vatsalbansal123/joystick_ros.git
    ```

2.  Install the necessary Python dependency:
    ```bash
    pip install pyserial
    ```

3.  Build the package from the root of your workspace:
    ```bash
    cd ~/ros2_ws
    colcon build --packages-select joy_controller
    ```

4.  Source your workspace's setup file:
    ```bash
    source install/setup.bash
    ```

## Usage

1.  Open a new terminal and run the turtlesim simulation:
    ```bash
    ros2 run turtlesim turtlesim_node
    ```

2.  Identify the serial port your Arduino is connected to. It is often `/dev/ttyACM0` or `/dev/ttyUSB0` on Linux.
    ```bash
    ls /dev/tty*
    ```

3.  Open a second terminal, source your workspace, and run the joystick controller node. Replace `/dev/ttyACM0` with your Arduino's serial port if it's different.
    ```bash
    source ~/ros2_ws/install/setup.bash
    ros2 run joy_controller joystick_node --ros-args -p port:=/dev/ttyACM0
    ```

You should now be able to control the turtle in the `turtlesim` window by moving the joystick. Moving the joystick up/down controls the linear velocity, and left/right controls the angular velocity.

### Node Parameters

The `joystick_node` accepts the following ROS 2 parameters:

*   `port`: The serial port of the Arduino. (Default: `/dev/ttyACM0`)
*   `baudrate`: The baud rate for the serial communication. This must match the rate set in the Arduino sketch. (Default: `115200`)