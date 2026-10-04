# Automatic Escalator Model

## Project Overview

Automatic Escalator Model is an Arduino-based embedded automation project that demonstrates automatic escalator operation using IR sensors, an L298N motor driver, a DC geared motor, and an emergency safety system. The escalator automatically starts when a person is detected at the entrance and stops after the passenger reaches the exit. An emergency button, LEDs, and buzzer provide additional safety and status indication.

This is a small, low-voltage educational prototype. It is not designed to carry people.

## Features

- Automatic motor start when the bottom IR sensor detects an object.
- Motor stop five seconds after the top IR sensor detects an object.
- Emergency button stops the motor immediately and prevents restart while held.
- Green, yellow, and red LEDs show system status.
- Buzzer sounds during an emergency.
- Non-blocking timing with `millis()` and simple sensor debouncing.
- Serial status output at 9600 baud.

## Hardware Components

- Arduino UNO
- L298N motor driver
- Low-voltage DC geared motor
- Two digital-output IR obstacle sensors
- Momentary emergency push button
- Green, red, and yellow LEDs
- One 220-330 ohm current-limiting resistor per LED
- Low-current active buzzer
- Suitable low-voltage DC motor supply/battery
- Jumper wires and breadboard
- Cardboard or acrylic frame, rollers, and rubber belt/chain

## Pin Configuration

| Function | Arduino UNO pin |
| --- | --- |
| Bottom entry IR sensor | D2 |
| Top exit IR sensor | D3 |
| Emergency button | D4 |
| L298N IN1 | D5 |
| L298N IN2 | D6 |
| L298N ENA (PWM) | D9 |
| Green LED | D10 |
| Red LED | D11 |
| Yellow LED | D12 |
| Buzzer | D13 |

The sketch assumes each IR sensor output is LOW when detection occurs. Change `SENSOR_ACTIVE_STATE` in the sketch to `HIGH` if your sensor outputs HIGH on detection. The emergency button uses `INPUT_PULLUP`: connect the button between D4 and GND; pressed is LOW.

## Circuit Connection

See [circuit_diagram.md](circuit_diagram.md) for the wiring diagram and exact connection table.

Important motor-driver notes:

- Remove the L298N ENA jumper to let Arduino D9 control motor speed with PWM.
- Connect the motor to L298N OUT1 and OUT2, not to Arduino pins.
- Power the motor from a suitable external DC supply connected to the driver's motor-power input and GND.
- Connect Arduino GND and L298N GND together.
- Follow the labeling/manual for the specific L298N board's logic 5V connection. Do not connect two 5V sources together.

## How It Works

1. In standby, the motor is off and the green LED is on.
2. A stable detection at the bottom sensor starts the motor in the configured direction at PWM speed 180.
3. A stable detection at the top sensor starts a five-second countdown. The motor then stops.
4. Pressing the emergency button immediately disables motor drive, turns on the red LED and buzzer, and cancels the stop countdown. The system returns to standby after the button is released and stable for 50 ms.
5. The yellow LED indicates sensor detection or an operating/waiting condition. Status is printed once per second.

The sensors detect objects crossing or blocking their beams; they do not count passengers. The controller only performs the simple entry-start and exit-timeout behavior described here.

## Installation

1. Install the Arduino IDE.
2. Open `Automatic_Escalator_Model.ino` from this folder.
3. Connect the Arduino UNO to the computer with USB.
4. Select **Tools > Board > Arduino AVR Boards > Arduino Uno**.
5. Select the correct serial port under **Tools > Port**.
6. Wire the sensors, driver, LEDs, buzzer, button, and external motor supply with power disconnected.

## How to Upload Code

1. Open the sketch in Arduino IDE.
2. Select the Arduino UNO board and its serial port.
3. Click **Verify** to compile the sketch.
4. Click **Upload**.
5. Open **Tools > Serial Monitor** and select **9600 baud**.
6. Test sensor and emergency-button behavior with the motor supply disconnected first. Connect motor power only after checking wiring and motor direction.

## Testing

| Test | Expected result |
| --- | --- |
| No sensor detection | Motor stays off; system is in standby. |
| Trigger bottom IR sensor | Motor starts in the configured direction. |
| Trigger top IR sensor while running | Motor stops after approximately five seconds. |
| Press emergency button | Motor drive turns off immediately; red LED and buzzer turn on. |
| Hold emergency button and trigger entry sensor | Motor remains off. |
| Release emergency button | Emergency clears after a short stable release; system returns to standby. |
| Disconnect Wi-Fi/network | No effect; this project does not use network features. |

## Safety Notes

- This is a low-voltage educational model only. Do not connect AC mains electricity.
- Never power the DC motor from an Arduino UNO GPIO or 5V pin.
- Use a suitable, correctly rated DC motor supply and observe its polarity and the motor/driver voltage limits.
- Keep the motor supply positive connected only to the L298N motor-power input. Join the supply ground, L298N GND, and Arduino GND.
- Test the emergency button and motor direction with the belt unloaded. The software emergency stop is not a certified safety system; for a physical demo, an emergency switch that interrupts motor power is preferable.
- Fit a current-limiting resistor in series with each LED. Use a low-current active buzzer on D13; use a transistor driver if the buzzer needs more current than an Arduino pin can safely supply.

## Future Improvements

- Add a separate, latching hardware emergency switch in the motor power path.
- Add a display for the current status and a configurable stop time.
- Improve passenger sensing with additional sensors or a beam arrangement.
- Add a guarded frame and better-supported rollers for a more durable demonstration.

## Developed By

Name - ATIFA JYOTI ///
University IoT/Embedded Systems Project

