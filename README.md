# DualMode Stopwatch

DualMode Stopwatch is an AVR microcontroller-based project that functions as a digital stopwatch with additional features, such as increment/decrement buttons, a buzzer alarm, and a user-friendly interface using seven-segment displays.

---

## Features

1. **Stopwatch Modes:**
   - Count-up mode (default).
   - Count-down mode.

2. **User Input Controls:**
   - Push buttons to increment or decrement hours, minutes, and seconds.
   - Mode toggle button to switch between count-up and count-down modes.
   - Pause and reset functionality.

3. **Seven-Segment Displays:**
   - Real-time display of the current time (hours, minutes, seconds).

4. **Buzzer Alarm:**
   - Activated upon reaching the maximum time in count-up mode or zero in count-down mode.

5. **LED Indicators:**
   - RED LED for count-up mode.
   - YELLOW LED for count-down mode.

---

## Components Required

1. **Microcontroller:**
   - ATmega32.

2. **Display:**
   - 6 x 7-segment displays.

3. **Inputs:**
   - Push buttons for incrementing/decrementing and mode switching.

4. **Outputs:**
   - Buzzer.
   - LEDs (RED and YELLOW).

5. **Other:**
   - 74LS47 BCD to seven-segment decoder (optional for interfacing the displays).

---

## How It Works

1. **Initialization:**
   - Configures input/output pins for controlling the seven-segment displays, LEDs, and buzzer.
   - Sets up Timer1 for stopwatch timing and external interrupts for user input.

2. **Modes of Operation:**
   - **Count-Up:** Increments time until it reaches a maximum value (99:59:59), then activates the buzzer.
   - **Count-Down:** Decrements time until it reaches zero, then activates the buzzer.

3. **Push Button Controls:**
   - Increment/decrement hours, minutes, and seconds independently.
   - Pause and reset the stopwatch as needed.

4. **Visual Feedback:**
   - The seven-segment displays continuously show the stopwatch time.
   - LEDs indicate the current mode of operation (count-up or count-down).

---

## Code Overview

The code is written in C and utilizes the AVR libraries for hardware interfacing:

- **Timer Initialization:**
  - Timer1 is used in CTC (Clear Timer on Compare Match) mode with a 1024 prescaler for generating 1-second intervals.

- **Interrupts:**
  - INT0: Reset stopwatch.
  - INT1: Pause stopwatch.
  - INT2: Resume stopwatch.
  - TIMER1_COMPA_vect: Handles time increments and decrements.

- **Custom Functions:**
  - `TIMER1_INIT`: Configures Timer1.
  - `INT0_INIT`, `INT1_INIT`, `INT2_INIT`: Configures external interrupts.
  - `Seven_Segment_Displays`: Updates the seven-segment displays.
  - `Increment_Decrement_PUSH_BUTTONS`: Handles increment/decrement logic for buttons.

---

## Pin Configuration

| Port | Pin   | Description                          |
|------|-------|--------------------------------------|
| PORTA | PA0-PA5 | Power source for 7-segment displays |
| PORTB | PB0-PB7 | Push buttons for input             |
| PORTC | PC0-PC3 | Data lines for 7-segment decoder   |
| PORTD | PD0     | Buzzer output                      |
| PORTD | PD2     | Reset stopwatch input              |
| PORTD | PD3     | Pause stopwatch input              |
| PORTD | PD4     | Count-Up LED output (RED)          |
| PORTD | PD5     | Count-Down LED output (YELLOW)     |

---

## How to Run

1. **Setup Hardware:**
   - Assemble the circuit as per the pin configuration.
   - Connect the microcontroller to the necessary components.

2. **Load Code:**
   - Use an AVR programmer to upload the code to the ATmega32 microcontroller.

3. **Power On:**
   - The stopwatch will start in count-up mode by default.
   - Use the buttons to control time and modes.

---

## Circuit Diagram

Below is the Proteus simulation for the DualMode Stopwatch project:

![Proteus Simulation](<img width="1917" alt="proteus_simulation" src="https://github.com/user-attachments/assets/18ffa436-439a-4235-a0da-767720f36ae9">)

To explore or edit the circuit, open the included Proteus project file (`Proteus P2.pdsprj`) using Proteus Design Suite.

---

## Future Enhancements

- Add an OLED or LCD for enhanced time display.
- Introduce memory functionality to save time states.
- Expand to include alarm customization.

---

## License

This project is open-source under the MIT License. Feel free to modify and share.


---

## Author

Developed by **Hassan Darwish**.
