# Final Project: Door Locker Security System

Welcome to the GitHub repository for the Door Locker Security System project! This  project is designed to implement a secure door locking and unlocking system using two ATmega32 Microcontrollers. The system features a layered architecture and incorporates various components like HMI_ECU (Human Machine Interface), Control_ECU, LCD, Keypad, DC-Motor, EEPROM, I2C, UART, Timer, and Buzzer drivers. This readme will guide you through the project's specifications and requirements.

![Door Lock Project](https://drive.google.com/uc?id=1eSbhn4_VdklzxSSG4xooJ58N9n2Lk8Nn)

[Project Simulation Link](https://www.linkedin.com/posts/ahmed-abd-elgleel-95a675252_embeddedsystems-innovation-engineeringexcellence-activity-7126581113010929664-mvUN?utm_source=share&utm_medium=member_desktop)

## System Requirements

### Hardware Requirements
- Two ATmega32 Microcontrollers with a frequency of 8MHz.
- 2x16 LCD for the HMI_ECU.
- 4x4 keypad for user interaction.
- DC-Motor for locking and unlocking the door.
- EEPROM for storing the system password.
- Buzzer for audio feedback.
- PIR sensor for detecting motion.

### Layered Architecture
The project should be designed and implemented based on a layered architecture model with the following components:
- Mc1 (HMI_ECU): Responsible for user interaction through the LCD and keypad.
- Mc2 (Control_ECU): Handles all processing, password verification, door control, and alarm activation.

### System Sequence

#### Step 1 - Create a System Password
- Display "Please Enter Password" on the LCD.
- Enter a 5-digit password, displaying '*' for each digit.
- Confirm the password by re-entering it.
- If the passwords match, save the password in EEPROM and proceed to Step 2.
- If the passwords do not match, repeat Step 1.

#### Step 2 - Main Options
- Display the main system options on the LCD.

#### Step 3 - Open Door
- Enter the password on the keypad and press enter.
- Compare the entered password with the one stored in EEPROM.
- If they match, unlock the door, detect motion, and lock the door after people enter.
- If the passwords do not match, repeat the password entry process.

#### Step 4 - Change Password
- Enter the current password and press enter.
- Compare the entered password with the one stored in EEPROM.
- If they match, repeat Step 1 to change the password.

#### Step 5 - Password Mismatch Handling
- If the passwords do not match in Step 3 or Step 4 for the third consecutive time:
  - Activate the buzzer for 1 minute.
  - Display an error message on the LCD for 1 minute.
  - Lock the system, and no keypad inputs will be accepted during this time.
  - Return to Step 2.

### Driver Requirements
- Use the GPIO driver implemented in the previous projects.
- Use the LCD driver implemented in the previous projects with 8-bits or 4-bits data mode.
- Connect the LCD to the HMI_ECU.
- Use the same DC_Motor driver implemented in the fan controller project.
- Connect the DC-Motor to the CONTROL_ECU.
- Use the external EEPROM driver controlled by I2C, connected to the CONTROL_ECU.
- Use the I2C driver implemented in the course.
- Use the same UART driver implemented in the course.
- Implement a full Timer driver for TIMER with the configuration technique, supporting both normal and compare modes.
- Use the same Timer driver in both ECUs.
- Implement a full Buzzer driver connected to the CONTROL_ECU.

### IDE and Simulation
- This project was developed using the Eclipse IDE.
- Simulation and testing were performed in Proteus.

### Video Simulation
- You can find a video demonstrating the simulation of the Door Locker Security System inside this repository. The video provides a visual walkthrough of how the system operates and interacts with the various components.

## Thank You & Good Luck

Thank you for checking out the Door Locker Security System project.
