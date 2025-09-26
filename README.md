# Gravity Clock

Gravity Clock is an embedded project based on the **STC8 microcontroller**.  
It uses gravity switches to detect the orientation of the clock and automatically controls countdown timers, alarms, and sleep/wake behavior.

---

## Features

- **Orientation detection**  
  1.Uses 2 gravity switches to detect 4 different directions.  
  2.Each direction has its own countdown timer.  

- **Countdown timer**  
  1.When the device is tilted to a direction, the corresponding countdown starts.  
  2.Countdown values can be set individually for each direction using buttons.  

- **Alarm**  
  1.When a countdown reaches zero, an alarm sounds.  
  2.After the alarm ends, the device automatically enters sleep mode.  

- **Sleep/Wake function**  
  1.Device sleeps after alarm finishes to save power.  
  2.Shaking the device or tilting it to a new direction wakes it up and resumes the current direction's countdown.  

- **EEPROM storage**  
  1.Countdown times for each direction can be set with buttons.  
  2.Values are stored in EEPROM to retain settings after power-off.
  
- **OLED display**  
  1.Shows countdown time for each direction.  
  2.Supports 4-direction display, adapts to current orientation.  

**Beeper**  
1. Uses a passive beeper for alarm notifications.
---

## Hardware

- **Microcontroller:** STC8 Series MCU  
- **Gravity Switches:** 2 units (detect 4 directions)  
- **Buzzer:** For alarm notifications (passive)  
- **Buttons:** For setting countdown times  
- **OLED display:** 64*128 Optional, shows current countdown and status (I2C protocol)
<img height="350" alt="image" src="https://github.com/user-attachments/assets/dc2c6437-cf1a-441b-9ab0-086706605c56" />


---

## Software
- Written in **C** for STC8 microcontroller.
- The software is designed with a **layered architecture** and follows a **state machine** approach.  
- **Layered design:** Separates low-level device drivers, system abstraction APIs, and high-level control logic for clarity and modularity.  
- **State machine:** The main loop executes business logic based on the current state
  
- Software architecture is organized into three layers:  
  1. **Device Driver Layer:** Handles direct register access and peripheral interactions.  
  2. **System Abstraction Layer:** Provides higher-level APIs for timers, I2C, UART, GPIO, etc.  
  3. **Control Logic Layer:** Implements the main application logic and state machine.  

- Software execution follows a **state-driven main loop**, where each state executes its corresponding business logic sequentially.

<img height="400" alt="image" src="https://github.com/user-attachments/assets/c3580c46-c49d-458d-ae09-3a6c2d63a404" />

---

## License

This project is open-source under the **MIT License**. You are free to use, modify, and distribute it with attribution.

---

