# Circuit Diagram and Wiring Guide

## Text Diagram

```text
                   +----------------------+
                   |     Arduino UNO      |
                   |                      |
IR Sensor 1 OUT --->| D2                   |
IR Sensor 2 OUT --->| D3                   |
Emergency button -->| D4                   |
                   | D5, D6, D9           |----> L298N IN1, IN2, ENA
                   | D10, D11, D12        |----> Green, Red, Yellow LEDs
                   | D13                  |----> Active buzzer
                   +----------+-----------+
                              |
                         Common GND
                              |
                   +----------v-----------+
                   |        L298N         |<---- External low-voltage DC supply
                   | IN1, IN2, ENA        |
                   | OUT1, OUT2           |
                   +----------+-----------+
                              |
                         DC geared motor
                              |
                       Rollers and belt
                              |
                       Escalator model
```

## Exact Connection Table

| Part | Connection |
| --- | --- |
| IR Sensor 1 (bottom) | VCC to Arduino 5V; GND to Arduino GND; OUT to D2. |
| IR Sensor 2 (top) | VCC to Arduino 5V; GND to Arduino GND; OUT to D3. |
| Emergency push button | One terminal to D4; the other terminal to GND. The sketch uses `INPUT_PULLUP`, so pressed is LOW. |
| L298N IN1 | Arduino D5. |
| L298N IN2 | Arduino D6. |
| L298N ENA | Arduino D9 (PWM). Remove the ENA jumper on the L298N module for PWM control. |
| DC geared motor | Connect the two motor wires to L298N OUT1 and OUT2. Swap these two wires if the belt moves in the wrong direction. |
| Motor supply positive | Connect to the L298N motor-power input (often labeled `12V` or `Vs`); use a supply suitable for the motor and driver. |
| Motor supply negative | Connect to L298N GND. |
| Common ground | Connect L298N GND to Arduino GND. Sensor grounds connect to Arduino GND. |
| L298N logic 5V | Follow the exact module's markings/manual. If using Arduino 5V as logic power, disable/remove the module's 5V regulator jumper as its manual specifies. Never connect two 5V sources together. |
| Green LED | Arduino D10 to a 220-330 ohm resistor, then LED anode (long leg); LED cathode (short leg) to GND. |
| Red LED | Arduino D11 to a 220-330 ohm resistor, then LED anode; LED cathode to GND. |
| Yellow LED | Arduino D12 to a 220-330 ohm resistor, then LED anode; LED cathode to GND. |
| Active buzzer | Positive terminal to D13 and negative terminal to GND, only for a low-current buzzer within the Arduino pin rating. Use a transistor driver for a higher-current buzzer. |

## Power and Build Checks

1. Keep the motor supply disconnected while wiring.
2. Do not connect the motor to Arduino 5V or GPIO pins. The motor is powered by the separate DC supply through the L298N.
3. Confirm the supply voltage and polarity against the motor and L298N board ratings before connecting it.
4. Join Arduino GND and L298N GND so the control signals have a common reference.
5. Check that the ENA jumper is removed before using D9 PWM.
6. Keep the belt and rollers clear of fingers and loose wires during testing. Test motor direction with the belt unloaded first.
7. Do not connect AC mains electricity.
