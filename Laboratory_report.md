# Laboratory Report: Button-Controlled DC Motor with ESP32 and L298N

**Name:** [FILL IN]
**Course and section:** [FILL IN]
**Instructor:** [FILL IN]
**Date:** October 25, 2026

---

## 1. Objective

The goal of this laboratory was to build a motor driver circuit with an ESP32 and an L298N dual H-bridge module, run Example 7, and check that a push button controls the motor: the motor runs while the button is held and coasts when it is released. I also had to identify the motor and driver limits from their datasheets and explain the roles of AIN1, AIN2, nSLEEP, VM and common ground.

## 2. Materials

| Item | Details |
|---|---|
| Microcontroller | ESP32 DevKit V1 (ESP-WROOM-32), powered over USB |
| Motor driver | L298N dual H-bridge module (red board with heat sink) |
| Motor | Small DC motor with a blue two-blade fan blade attached |
| Motor supply | 4 x AAA carbon (zinc-carbon) batteries, 1.5 V each, connected in series (about 6 V nominal) |
| Input | Momentary push button on a breadboard |
| Other | Breadboard, jumper wires, USB cable |
| Software | Arduino IDE with the ESP32 board package (Espressif Systems) |

## 3. Circuit description and wiring

### 3.1 Connections

| From | To | Purpose |
|---|---|---|
| ESP32 GPIO23 | L298N ENA | Enable: HIGH = run, LOW = coast |
| ESP32 GPIO18 | L298N IN1 | Direction input 1 |
| ESP32 GPIO19 | L298N IN2 | Direction input 2 |
| ESP32 GPIO27 | Push button, leg 1 | Button input (INPUT_PULLUP) |
| Push button, leg 2 | ESP32 GND | Button return |
| ESP32 VIN (5 V) | L298N 5V terminal | Logic supply for the driver |
| ESP32 GND | L298N GND terminal | Common ground |
| Battery pack + | L298N 12V terminal | Motor supply (VM) |
| Battery pack - | L298N GND terminal | Supply return |
| Motor wires | L298N OUT1 and OUT2 | Motor outputs |

IN3, IN4 and ENB are not used.

### 3.2 Jumper settings

- **5V-EN jumper removed.** The motor supply is only about 6 V, which is too low for the module's on-board 5 V regulator to work properly. The 5 V for the driver's logic comes from the ESP32 VIN pin instead, and the 5V terminal is used as an input.
- **ENA jumper removed.** ENA is driven by GPIO23 so the program can switch the output stage off. This is what makes the motor coast.
- **ENB jumper left on.** Channel B is not used.

### 3.3 Wiring evidence

![Figure 1: Assembled circuit. ESP32 on the breadboard (USB powered), L298N module with its power LED on, push button on the breadboard, and the motor with the blue fan blade connected to the OUT terminals.](media/wiring_top.jpg)

*Figure 1: The assembled circuit.*

## 4. Datasheet findings

### 4.1 Motor (Task 1)

| Item | Value |
|---|---|
| Motor | Small DC motor from a mini fan, with a blue fan blade (Figure 1) |
| Part number | [FILL IN: read it from the motor can] |
| Rated voltage (datasheet) | [FILL IN] V |
| Stall current (datasheet) | [FILL IN] A |
| Datasheet link | [FILL IN] |

The supply and driver were chosen against these two values. The battery pack gives about 6 V nominal, and the L298N drops about 2 V across its output stage, so the motor sees roughly 4 V. The stall current must stay below the L298N limit of 2 A per channel. [FILL IN: one sentence comparing your datasheet values with 4 V and 2 A.]

### 4.2 Breakout (Task 2)

| Item | Value |
|---|---|
| Breakout model | L298N dual H-bridge motor driver module (ST L298 chip) |
| Motor supply (VM, the 12V terminal) | The chip allows up to 46 V. The on-board capacitors on my module are marked 35 V, so I treated 35 V as the module limit. I used about 6 V. |
| Logic supply (VSS, the 5V terminal) | 5 V (the chip allows about 4.5 to 7 V), supplied from ESP32 VIN |
| Output current per channel | 2 A DC, 3 A non-repetitive peak |
| Voltage drop across the bridge | About 2 V typical, so the motor sees less than the supply |
| Logic input HIGH threshold | At least 2.3 V, so the ESP32's 3.3 V outputs can drive IN1, IN2 and ENA directly |
| Datasheet | ST L298: https://www.st.com/resource/en/datasheet/l298.pdf |
| Module vendor page | [FILL IN if you have one] |

## 5. Software

The sketch `motor_button.ino` (full listing in Appendix A) uses these pins:

| Constant | GPIO | Role |
|---|---|---|
| ENA | 23 | L298N enable |
| IN1 | 18 | L298N input 1 |
| IN2 | 19 | L298N input 2 |
| BTN | 27 | Push button, INPUT_PULLUP |

The button is read in `loop()`. The pin uses the internal pull-up, so a press reads LOW. When the state changes:

- **Pressed:** IN1 = HIGH, IN2 = LOW, ENA = HIGH. The motor runs forward.
- **Released:** ENA = LOW, IN1 = LOW, IN2 = LOW. The output stage is disabled and the motor coasts.

A message is printed only when the state changes, and a 20 ms delay acts as a simple debounce.

## 6. Procedure

1. I wired the circuit with the battery pack disconnected and checked every connection against Section 3.1.
2. I uploaded the sketch from the Arduino IDE (board: ESP32-WROOM-DA Module, port: COM4, 115200 baud).
3. I opened the Serial Monitor and pressed the EN (reset) button on the ESP32 to see the start-up message.
4. I connected the battery pack last.
5. I pressed and released the push button several times and recorded what the motor and the Serial Monitor did.

Upload result:

```
Wrote 272240 bytes (157227 compressed) at 0x00010000 in 2.8 seconds (772.7 kbit/s).
Verifying written data...
Hash of data verified.
Hard resetting via RTS pin...
```

## 7. Observed behavior (Task 3)

### 7.1 Serial Monitor output

Right after a reset:

```
Ready. Hold the button to run the motor.
Motor ON
Motor OFF (coast)
Motor ON
Motor OFF (coast)
Motor ON
```

After several presses:

```
Motor OFF (coast)
Motor ON
Motor OFF (coast)
Motor ON
Motor OFF (coast)
Motor ON
Motor OFF (coast)
Motor ON
Motor OFF (coast)
```

![Figure 2: Serial Monitor at 115200 baud on COM4 showing Motor ON and Motor OFF (coast) alternating with each press and release.](media/serial_monitor.png)

*Figure 2: Serial Monitor output.*

### 7.2 Expected vs observed

| Button | ENA | IN1 | IN2 | Expected | Observed |
|---|---|---|---|---|---|
| Not pressed | LOW | LOW | LOW | Motor stopped | The motor stayed off |
| Held | HIGH | HIGH | LOW | Motor runs forward | The motor turned on as soon as I pressed the button and spun fast and strongly for as long as I held it. The Serial Monitor printed Motor ON. |
| Released | LOW | LOW | LOW | Motor coasts to a stop | The motor turned off when I released the button, and the Serial Monitor printed Motor OFF (coast). |

The motor only runs while the button is pressed. Each press printed exactly one Motor ON line and each release printed one Motor OFF (coast) line, so the button and the code behave as intended. On release, ENA goes LOW and disables the output stage, so the motor is not braked and slows down freely. That is the difference between coasting and braking on this driver (see Section 8).

Demo video: `media/demo.mp4`

### 7.3 Problems encountered

| Problem | Cause | Fix |
|---|---|---|
| The Serial Monitor showed only boot text after the upload | The Ready message is printed once at reset, and I opened the monitor after it | I pressed the EN button with the monitor open |
| The motor ran straight after reset and the Serial Monitor printed Motor ON without a press | Both button wires were on legs of the 4-pin button that are connected inside the button, so GPIO27 always read LOW | I moved one wire to the diagonal leg so the button only connects when pressed |

## 8. Roles of the signals (Task 4)

The task uses the pin names of a DRV8833-style breakout. The L298N module does not have pins with those exact names, so I mapped each role to its L298N equivalent.

| Task term | L298N pin | Role |
|---|---|---|
| AIN1, AIN2 | IN1, IN2 | Logic inputs that select the direction of the motor current. IN1 HIGH with IN2 LOW drives the motor forward, and the opposite drives it backward. |
| nSLEEP | ENA (closest match) | Enables the output stage. HIGH = outputs active. LOW = outputs disabled, so the motor coasts. The L298N has no low-power sleep mode, so this is only an approximate match. |
| VM | 12V terminal (VS) | The motor supply. All motor current flows from here through the bridge to the motor. It is separate from the 5 V logic supply. |
| Common ground | GND terminal | Joins battery negative, the driver GND and the ESP32 GND. The ESP32's 3.3 V logic signals need the same 0 V reference as the driver, otherwise the inputs have no reference and the motor does not respond reliably. |

L298N truth table (from the ST datasheet):

| ENA | IN1 | IN2 | Motor |
|---|---|---|---|
| HIGH | HIGH | LOW | Forward |
| HIGH | LOW | HIGH | Reverse |
| HIGH | LOW | LOW | Fast stop (brake) |
| HIGH | HIGH | HIGH | Fast stop (brake) |
| LOW | X | X | Free-running stop (coast) |

I drove ENA from GPIO23 for this reason. If I had left the ENA jumper on, releasing the button would set both inputs LOW with ENA HIGH, and the motor would brake instead of coasting.

## 9. Optional: potentiometer PWM control (Task 5)

I did not do the optional potentiometer PWM extension, so I do not have a measured duty cycle for the start of rotation.

If I extended the lab, I would connect a potentiometer to an ADC1 pin such as GPIO34, remove the fixed HIGH on ENA, and drive ENA with PWM from the ESP32. I would raise the duty slowly from 0% and record the first duty at which the shaft turns. That value would be a measured result and would not be zero, because the motor needs enough voltage to overcome static friction and the L298N drops about 2 V of the supply.

## 10. Discussion

The circuit worked as designed. Holding the button made the motor spin fast, and releasing it stopped the motor. The Serial Monitor confirmed that the ESP32 read the button correctly: one Motor ON and one Motor OFF (coast) message for every press and release.

Three points stood out during the lab:

1. **Common ground.** The driver only responds to the ESP32 signals because the battery negative, the L298N GND and the ESP32 GND are joined at the GND terminal.
2. **Coast versus brake.** On the L298N, setting both inputs LOW with ENA HIGH brakes the motor. Driving ENA from a GPIO and setting it LOW is what produces a real coast.
3. **Voltage loss.** The L298N drops about 2 V across the bridge, so the 6 V nominal supply gives the motor only about 4 V. The carbon AAA cells I used also have higher internal resistance than alkaline cells, so their voltage falls under load. Even so, the small motor spun fast with this supply.

The task refers to AIN1, AIN2, nSLEEP and VM, which are pins of a DRV8833-style breakout. I used an L298N module and mapped the pins as shown in Section 8. The L298N has no sleep mode, so ENA is only the closest match for nSLEEP.

## 11. Conclusion

I built the ESP32 and L298N driver circuit, uploaded the sketch, and verified the button-controlled run and coast behavior. The motor runs only while the push button is held and coasts when it is released. I identified the roles of IN1, IN2, ENA, the 12V motor supply and the common ground, and related them to AIN1, AIN2, nSLEEP, VM and common ground from the task. I did not perform the optional PWM extension.

## 12. Submission contents

| Item | File or section |
|---|---|
| Source code | `motor_button.ino` (also Appendix A) |
| Short project description | `README.md` |
| Motor datasheet link | Section 4.1 |
| Breakout datasheet link | Section 4.2 |
| Wiring evidence | Figure 1, `media/wiring_top.jpg` |
| Observed behavior | Figure 2, `media/serial_monitor.png`, and Section 7 |
| Demo video | `media/demo.mp4` |

## References

- ST L298 datasheet: https://www.st.com/resource/en/datasheet/l298.pdf
- Motor datasheet: [FILL IN]
- L298N module vendor page: [FILL IN if available]

## Appendix A: Source code

```cpp
// ESP32 (ESP-WROOM-32 DevKit V1) + L298N: hold the button to run the motor
//
// WIRING
//   ESP32 GPIO23 -> L298N ENA        (remove the ENA jumper)
//   ESP32 GPIO18 -> L298N IN1
//   ESP32 GPIO19 -> L298N IN2
//   Button leg 1 -> ESP32 GPIO27
//   Button leg 2 -> ESP32 GND        (use diagonal legs on a 4-pin button)
//   ESP32 VIN    -> L298N 5V         (remove the 5V-EN jumper)
//   ESP32 GND    -> L298N GND        (same terminal as battery -)
//   Battery +    -> L298N 12V
//   Battery -    -> L298N GND
//   Motor        -> L298N OUT1 and OUT2
//   IN3, IN4 unused. ENB jumper can stay on.
//   ESP32 must be powered by USB so VIN supplies 5V.

#include <Arduino.h>

const uint8_t ENA = 23;
const uint8_t IN1 = 18;
const uint8_t IN2 = 19;
const uint8_t BTN = 27;

bool lastPressed = false;

void setup() {
  Serial.begin(115200);

  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(BTN, INPUT_PULLUP);   // pressed = LOW, no external resistor needed

  // Start stopped (coasting)
  digitalWrite(ENA, LOW);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

  Serial.println("Ready. Hold the button to run the motor.");
}

void loop() {
  const bool pressed = (digitalRead(BTN) == LOW);

  if (pressed != lastPressed) {
    lastPressed = pressed;

    if (pressed) {
      digitalWrite(IN1, HIGH);   // forward
      digitalWrite(IN2, LOW);
      digitalWrite(ENA, HIGH);   // enable the bridge: motor runs
      Serial.println("Motor ON");
    } else {
      digitalWrite(ENA, LOW);    // bridge disabled: motor coasts
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);
      Serial.println("Motor OFF (coast)");
    }
  }

  delay(20);   // also acts as a simple debounce
}
```
