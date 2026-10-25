# ESP32 + L298N: Button-Controlled DC Motor

Lab task: build the driver circuit and run Example 7 (button-controlled run/coast).

Fill in the `[FILL IN]` items (motor details and datasheet link) before submitting.

## What it does

An ESP32 (ESP-WROOM-32 DevKit V1) drives a small DC motor through an L298N dual H-bridge module.

- **Hold the push button:** the motor runs forward, fast and strongly.
- **Release the button:** the motor coasts to a stop.
- Every change is printed to the Serial Monitor (`Motor ON` / `Motor OFF (coast)`).

## Hardware

| Part | Details |
|---|---|
| Microcontroller | ESP32 DevKit V1 (ESP-WROOM-32), powered over USB |
| Motor driver | L298N dual H-bridge module |
| Motor | Small DC motor with a blue fan blade. Part number: [FILL IN] |
| Motor supply | 4 x AAA carbon batteries (1.5 V each) in series, about 6 V nominal |
| Input | Push button, active LOW (internal pull-up) |

## Pin mapping

| ESP32 | Connects to | Purpose |
|---|---|---|
| GPIO23 (D23) | L298N ENA | Enable: HIGH = run, LOW = coast |
| GPIO18 (D18) | L298N IN1 | Direction input 1 |
| GPIO19 (D19) | L298N IN2 | Direction input 2 |
| GPIO27 (D27) | Push button leg 1 | Button input |
| GND | Push button leg 2 | Button return |
| VIN (5 V) | L298N 5V terminal | Logic supply |
| GND | L298N GND terminal | Common ground |

Power and motor connections on the L298N:

| L298N terminal | Connects to |
|---|---|
| 12V (VM) | Battery + |
| GND | Battery - and ESP32 GND (same terminal) |
| 5V | ESP32 VIN |
| OUT1, OUT2 | Motor |

IN3, IN4 and ENB are not used.

## Jumper settings

- 5V-EN jumper: removed (the 5 V logic supply comes from the ESP32 VIN pin).
- ENA jumper: removed (ENA is driven by GPIO23).
- ENB jumper: left on (not used).

## Behavior

| Button | ENA | IN1 | IN2 | Motor |
|---|---|---|---|---|
| Held | HIGH | HIGH | LOW | Runs forward |
| Released | LOW | LOW | LOW | Coasts (free-running stop) |

## How to run

1. Wire everything with the battery disconnected.
2. In the Arduino IDE, install the ESP32 board package by Espressif Systems.
3. Select the board (ESP32-WROOM-DA Module or ESP32 Dev Module) and the COM port.
4. Open `motor_button.ino` and upload it.
5. Open the Serial Monitor at 115200 baud and press the EN button on the ESP32. You should see `Ready. Hold the button to run the motor.`
6. Connect the battery last, then hold the push button.

## Results

Serial Monitor output:

```
Ready. Hold the button to run the motor.
Motor ON
Motor OFF (coast)
Motor ON
Motor OFF (coast)
```

Observed behavior: the motor turns on only while the button is held and spins fast. When the button is released, ENA goes LOW and the motor coasts to a stop. Every press and release printed exactly one message.

![Assembled circuit](media/wiring_top.jpg)

![Serial Monitor output](media/serial_monitor.png)

The optional potentiometer PWM extension was not done.

## Terminology (task vs. L298N)

The task uses names from DRV8833-style breakouts. On the L298N module they map as follows:

| Task term | L298N module |
|---|---|
| AIN1, AIN2 | IN1, IN2 |
| nSLEEP | ENA (closest match, no true sleep mode) |
| VM | 12V terminal (motor supply) |
| Common ground | GND terminal shared by battery and ESP32 |

## Files

```
motor_button.ino        source code
README.md               this file
laboratory_report.md    laboratory report
media/
  wiring_top.jpg        photo of the actual wiring
  serial_monitor.png    Serial Monitor screenshot
  demo.mp4              video of button press and release (add your own)
```

## Datasheets

- Breakout chip (ST L298): https://www.st.com/resource/en/datasheet/l298.pdf
- Motor datasheet: [FILL IN]
  - Rated voltage: [FILL IN]
  - Stall current: [FILL IN]
