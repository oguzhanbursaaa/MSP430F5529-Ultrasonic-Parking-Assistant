# Smart Parking Sensor with MSP430

A simple, bare-metal C project for the **MSP430F5529 LaunchPad**. This project mimics a car's parking assistant by using an ultrasonic sensor to measure distance and providing real-time visual and auditory feedback.

## 🚀 Features
* **Bare-Metal C:** Direct register-level programming without external libraries.
* **Hardware Timers:** Uses `Timer_A1` in Input Capture mode to measure distance precisely without blocking the CPU.
* **PWM Audio:** Uses `Timer_A0` to generate PWM signals for the passive buzzer.
* **Safe Logic Levels:** Implements a voltage divider to safely connect the 5V sensor to the 3.3V microcontroller.

## 🛠️ Hardware Requirements
* MSP430F5529 LaunchPad
* HC-SR04 Ultrasonic Distance Sensor
* 1x Passive Buzzer
* 3x LEDs (Green, Yellow, Red)
* 3x 220 Ω Resistors (for LEDs)
* 2x 10 kΩ Resistors (for the voltage divider on the Echo pin)
* Breadboard and Jumper Wires

## 🔌 Pin Configuration & Wiring

| Component | MSP430 Pin | Additional Info |
| :--- | :--- | :--- |
| **HC-SR04 Trig** | `P1.6` | Direct connection |
| **HC-SR04 Echo** | `P2.0` | **IMPORTANT:** Connect via 10k voltage divider to drop 5V to 2.5V! |
| **Green LED** | `P3.0` | Connect with 220Ω resistor to GND |
| **Yellow LED** | `P3.1` | Connect with 220Ω resistor to GND |
| **Red LED** | `P3.2` | Connect with 220Ω resistor to GND |
| **Buzzer (+)** | `P1.2` | PWM Output |

*(Make sure to share a common GND between the MSP430, the 5V power supply, and all components).*

## 📏 How It Works (Distance Logic)
The system calculates the distance continuously and updates the feedback:
* **> 30 cm (Safe Zone):** All LEDs OFF, Buzzer OFF.
* **20 cm - 30 cm (Far):** Green LED ON, Buzzer beeps slowly.
* **10 cm - 20 cm (Middle):** Yellow LED ON, Buzzer beeps faster.
* **< 10 cm (Close/Danger):** Red LED ON, Buzzer emits a continuous sound.

## 💻 How to Run
1. Clone this repository.
2. Open **Code Composer Studio (CCS)** or **IAR Embedded Workbench**.
3. Create a new empty project for the `MSP430F5529`.
4. Copy the contents of `main.c` from this repository into your project.
5. Build and flash the code to your LaunchPad.
6. Provide a 5V source to the HC-SR04, and you are good to go!
