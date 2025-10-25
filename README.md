# 🔬 Microprocessors & Microcontrollers Lab Projects (Spring 1402-1403)

## 🌟 Project Overview

This repository contains a comprehensive set of projects and exercises completed for the Microprocessor and Microcontroller Laboratory course. These labs cover a complete learning path from classic **8086 architecture** to modern **ARM Cortex-M microcontrollers (STM32F401)**.

The projects include implementing computational algorithms at the assembly level, developing simple interactive games, designing hardware interface circuits in a simulator, and engaging in embedded programming in environments like Keil MDK.

### 🛠️ Technologies and Tools Used

| Domain | Technology/Chip | Programming Language | Simulation & Development Tools |
| :--- | :--- | :--- | :--- |
| **Classic Architecture** | Intel 8086, 8255 PPI, 2764/6264 Memory | Assembly (x86) | EMU8086, Proteus (for circuit design) |
| **Modern Embedded** | STM32F401RET (ARM Cortex-M3/4) | Assembly (ARM), C | Keil MDK, Proteus (for STM32 simulation) |
| **Key Concepts** | Address Decoding, DOS/BIOS Interrupts, GPIO, Timers, Register-Level Programming | - | - |

---

## 🎯 Laboratory Projects (Labs)

The projects are organized weekly, focusing on a specific concept in computer architecture or embedded systems:

### 1. Lab 1: Prime Palindromic Numbers (8086 Assembly)
* **Objective:** Practice 8086 Assembly programming and use of Procedures.
* **Implementation:** Find and store the first **10 Prime Palindromic numbers** in memory using EMU8086. Utilized separate procedures to check for primality and palindromic property.

### 2. Lab 2: Arithmetic Game (8086 Assembly + DOS/BIOS)
* **Objective:** Utilize **DOS and BIOS Interrupts** for user interaction and timing.
* **Implementation:** Designed a simple addition/subtraction game in 8086 Assembly. Features include: taking user name/seed, displaying score, and implementing a **20-second time limit** with a visual **Progress Bar** that changes color (green, yellow, red) over time.

### 3. Lab 3: Memory Interface Design (8086 Hardware - Proteus)
* **Objective:** Understand **Address Decoding** and memory interfacing with the 8086.
* **Implementation:** Circuit design in Proteus to connect **ROM (2764)** and **RAM (6264)** memory blocks (8Kx16 each) to the 8086 processor, and specify their address spaces.

### 4. Lab 4: I/O Interface Design (8086 Hardware - Proteus + 8255)
* **Objective:** Understand **I/O Address Decoding** and work with the **8255 PPI** chip and a matrix keypad.
* **Implementation:** Designed a circuit in Proteus using the 8086, 8255, a **Matrix Keypad**, **Three 7-Segment Displays**, and two LEDs. The user inputs a 3-digit number, which is displayed on the 7-Segments, while the two LEDs indicate whether the number is **Prime** and **Palindromic**.

### 5. Lab 5: Prime Palindromic Numbers (ARM Assembly - Keil MDK)
* **Objective:** Practice **ARM Assembly** programming (Cortex-M3/4 architecture).
* **Implementation:** Re-implementation of the Lab 1 project (finding 10 Prime Palindromic numbers) using **ARM Assembly** for the **STM32F401RET** microcontroller, with results verified in the Keil Debugger's Memory and Watch windows.

### 6. Lab 7: STM32 User Interface (C Programming - Nucleo)
* **Objective:** **Register-Level C programming** for the STM32F401 microcontroller.
* **Implementation:** Displaying Prime Palindromic numbers on a **7-Segment Display**.
    * Digits are shown sequentially (0.5 seconds per digit) with a 2-second blanking period between numbers.
    * Used **Two Push Buttons** to navigate (increment/decrement) between the Prime Palindromic numbers.
    * Circuit was designed in Proteus and successfully implemented on the physical **Nucleo-64** board.

---

## 📂 Project File Structure

The project files are structured by the development domain and language:

```

├── README.md
├── 8086\_Assembly/
│   ├── Lab1\_PrimePalindrome/
│   ├── Lab2\_ArithmeticGame/
│   └── Lab4\_KeypadIO/
├── STM32\_C/
│   ├── Lab5\_ARMAssembly/      \# Source code in ARM Assembly (using Keil environment)
│   ├── Lab7\_7SegInterface/    \# Source code in C (Register-Level STM32)
│   └── main.c                 \# Example C file for STM32
├── Hardware\_Simulations/
│   ├── Lab3\_MemoryInterface/
│   ├── Lab4\_IOInterface/
│   └── \*.pdsprj              \# Proteus project files (e.g., lab6.pdsprj, stm32.pdsprj)
└── Reports/
└── ... \# Lab reports and documentation

```

---

## 👥 Contributors

**[MahdisSep]** 
**[MobinaShahbazi]** 
