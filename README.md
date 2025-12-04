# 🕹 LandTiger Pac-Man (LPC1768, ARM Cortex-M3)

LandTiger Pac-Man is a bare-metal implementation of a Pac-Man–style arcade game for the **LandTiger LPC1768** development board (ARM Cortex-M3).

The game draws a maze on the TFT GLCD, moves Pac-Man and a ghost in real time, handles scoring and a countdown timer, plays sound using the DAC and timers, and reads input from the on-board joystick, buttons, ADC (potentiometer), and optionally the resistive touch panel.

The project is written entirely in **C + ARM assembly (startup)** and runs **without an operating system**, directly on the microcontroller.

---

## Features

- **Pac-Man–like gameplay**
  - Pac-Man moves continuously in the last chosen direction.
  - Pills on the map, score tracking, remaining lives, and a game timer.
  - Ghost movement handled by dedicated logic in `ghost.c`.
- **Real-time game loop via interrupts**
  - Multiple hardware timers (Timer0, Timer1, Timer2, Timer3) for:
    - Game countdown and ghost mode transitions.
    - Pac-Man / ghost movement updates.
    - DAC waveform output for audio.
    - Note duration timing for music.
  - **RIT (Repetitive Interrupt Timer)** for joystick & button polling and UI events.
- **Graphics on GLCD**
  - Maze, characters, HUD, and messages drawn on the LandTiger TFT using `GLCD.c`.
  - Font support via `AsciiLib`.
  - Optional frequency display / debug graphics via `HzLib`.
- **Input**
  - On-board joystick for Pac-Man direction control.
  - On-board buttons for actions like pause/resume.
  - **ADC** (P1.31 / AD0.5) for analog input (e.g., controlling sound / LED bargraph).
  - Optional **touch panel** input via ADS7843 on SSP1 with calibration.
- **Audio and music**
  - DAC output driven by a sine lookup table and timers.
  - Simple music system with a `NOTE` struct and predefined melodies (play/win/lose themes).
  - ADC-controlled note selection in a mini “synth” mode (potentiometer → note + LEDs).
- **LED indicators**
  - 8 on-board LEDs used as a bargraph for ADC value and for status/debug output.

---

## Hardware & Dependencies

- **Board:** LandTiger LPC1768 (NXP LPC1768 ARM Cortex-M3)
- **Peripherals used:**
  - TFT GLCD
  - Joystick
  - Push buttons (INT0 / others)
  - 8× LEDs
  - DAC (audio output)
  - ADC (potentiometer on AD0.5 / P1.31)
  - RIT
  - Timer0, Timer1, Timer2, Timer3
  - SSP1 (for touch controller ADS7843, optional)
- **Toolchain / IDE:**
  - Any ARM toolchain supporting LPC1768 + CMSIS.
  - Typical setup: Keil MDK-ARM, IAR, or arm-none-eabi-gcc with appropriate startup and linker scripts.

---
