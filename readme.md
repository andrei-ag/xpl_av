# 🛡️ Explosion Antivirus

[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)
[![Language](https://img.shields.io/badge/ASM-99.7%25-orange)](https://github.com/andrei-ag/xpl_av)
[![Version](https://img.shields.io/badge/version-012BETA-red)](https://github.com/andrei-ag/xpl_av/releases)

**Educational antivirus demonstrating PE file disinfection, custom x86 emulation, and polymorphic virus detection.**
> ⚠️ **Important:** This software is for **educational and research purposes only**. It is designed to demonstrate antivirus techniques and should not be used on production systems. Use it only in isolated environments (e.g., virtual machines) with backup copies of files.

## Project Philosophy: Why This Exists

Explosion Antivirus is **not** a commercial product. It never was, and it never will be. Its purpose is purely **educational**.\
This project was built on a few core beliefs:

### 1. Education over Commerce
The primary goal has always been to **demonstrate** how antivirus technologies work, not to sell a product. The code is written to be read, studied, and learned from. Every major component is documented in both Russian and English.

### 2. Proof of Concept over Product
At its heart, this is a **proof of concept**. The goal was to show
>that advanced techniques like x86 emulation, polymorphic virus detection, and PE file disinfection are not black magic reserved for large corporations. **One developer, writing in assembly, could build them from scratch.**

### 3. Emulation over Signatures
> Universal technologies are valued over specific solutions.

The custom x86 emulator is the soul of this project. The ability to emulate any code, to create a sandbox, is far more important than a long list of virus signatures. The emulator exists to educate; the signature database exists only to test the emulator.

### 4. Openness over Secrecy
>Knowledge should be free.
>
This code is open, and the license (GNU GPL) guarantees it remains so. This repository serves as a living textbook for low-level programming, reverse engineering, and security research.

### 5. Quality over Speed
>Open source does not mean low quality.
>
This project was written in **assembly language**—not because it's the easiest path, but because it demonstrates a deep, uncompromising understanding of how a computer works at the lowest level. The code is meant to be a work of engineering and education.

>If you want a tool to clean your PC, this is not for you.

>If you want to learn how an antivirus works under the hood, you are in the right place.

## 🎯 Key Features

### 🧠 Custom Code Emulator (x86 Assembly)
- Implements a **full x86 instruction emulator** written from scratch in assembly (99.7% of the codebase).
- Capable of emulating polymorphic decryptors found in viruses like **Win32/Driller** (average decryptor size ~9 KB and uses complex anti-emulation API calls), and **Win9X/Prizzy** (which uses FPU/MMX instructions for junk code), **Win32/Deadcode** (which uses PEB), **W95/Marburg**, **W32/Krized**, **W32/Thorin**.
- Includes a **loop detector** to avoid getting stuck in long decryption loops.
- **Emulates 31 Windows API calls** (e.g., `GetTickCount`, `GetVersion`, `GetCommandLineA`, `IsBadReadPtr`) to bypass common anti-emulation tricks.

### 🩺 Virus Disinfection (Rare in Open Source)
The antivirus can not only detect but also **remove virus code and restore infected PE files**. Disinfection routines are implemented for:
- ✅ **Win32/Parite.b**
- ✅ **W32/Krized [4029]** (infects `KERNEL32.DLL`)
- ✅ **Win32/Funlove [4099]**
- ✅ **W95/Marburg [8582]**

### 🔍 PE File Analysis & Dumping
- Deep analysis of **Portable Executable (PE)** structure (headers, sections, import/export tables).
- Option to create **memory dumps** (`/d` key) of decrypted virus bodies for further analysis.
- Debug output for detected loops (`/di` key).

### 🖥️ Console Interface
- Command-line driven with flexible scanning options.
- Supports scan reports (`/rc`, `/ra`), recursive scanning (`/*`), and disinfection (`/c`).

## 💎 What Makes This Project Unique

Compared to most open-source antivirus projects, Explosion Antivirus has several distinctive features:

| Aspect | Explosion Antivirus | Most Open-Source AVs |
| :--- | :--- | :--- |
| **Code Emulation** | ✅ Custom x86 emulator in ASM | ❌ or use external libs (Unicorn) |
| **Polymorphic Virus Detection** | ✅ W32/Driller, Win9X/Prizzy, W95/Marburg | ❌ Mostly signature-based |
| **Disinfection (Curing)** | ✅ Parite, Krized, Funlove, Marburg | ❌ Detection only |
| **API Emulation** | ✅ 31 Windows API functions | ❌ Rare |
| **Language** | Assembly (99.7%) | C/C++/Python |

---

## 🚀 Getting Started

### Build Requirements
- **Flat Assembler (fasm)** version 1.67.29 or compatible.
- All necessary FASM include files are provided in the `FASM_INC/` directory.

### Build Instructions
```
fasm.exe xpl.asm
```

## 📂 Repository Structure

| Directory/File | Description |
| :--- | :--- |
| `DATA/` | Static data for the antivirus |
| `DET/` | Detection routines (signatures, unpackers) |
| `DOCS/` | Documentation (Russian) |
| `EMUL/` | Core emulator and API emulation |
| `FASM_INC/` | Flat Assembler include files |
| `FILE/` | PE loading, import parsing, dumping |
| `INCLUDE/` | Common include files |
| `SECTIONS/` | PE section structure definitions |
| `det/cure/` | Virus-specific disinfection routines |
| `disasm/` | Custom x86 disassembler |
| `LICENSE.txt` | GNU General Public License v3 |
| `XPL.ASM` | Main source file |

## 📚 Related Publications & Historical Notes

The following articles were written by me to explain the core technologies implemented in Explosion Antivirus. They provide theoretical background and practical insights into the design of the emulator, code analyzer, and anti-emulation techniques. The articles greatly benefited from the help and support of the **UINC.ru team**, with special thanks to **Dr.Golova** for valuable contributions.

The recommended reading order follows the logical flow from basic emulation architecture to advanced analysis and vulnerability testing.

1.  **"Code Emulation"** (15 February 2004)  
    *Architecture of the x86 emulator: virtual stack, virtual registers, «sandboxed» instruction execution (`run_instr`), and full emulation of complex instructions (`call`, `ret`, conditional jumps).*  
    [Archived version](https://web.archive.org/web/20051215194308/http://uinc.ru:80/articles/47/)

2.  **"Code Analyzers in Antivirus Software"** (24 February 2004)  
    *Delta value detection (search for `call $+5`/`pop reg` routines), signature matching with wildcards (`'?'`), and a packer detector (e.g., UPX).*  
    [Archived version](https://web.archive.org/web/20051215213447/http://uinc.ru:80/articles/45/)

3.  **"Vulnerabilities of Code Emulators"** (6 April 2004)  
    *Anti-emulation tricks (delta value, initial EAX value, `idiv32` tests, API calls) with real-world testing results against popular antivirus engines of the time.*  
    [Archived version](https://web.archive.org/web/20051215200738/http://uinc.ru:80/articles/48/)

These articles formed the theoretical foundation for many components of Explosion Antivirus and were originally published on *UInC.ru* (now preserved via the Wayback Machine). The complete source code of the antivirus is available in this repository as a practical implementation of the described techniques.

## 🚀 Usage
XPL.EXE { KEYS } { PATH }

| Key | Description |
| :--- | :--- |
| `/rc` | Create scan report (`xplosion.rep`) |
| `/ra` | Append to existing report |
| `/*` | Scan all fixed drives |
| `/c` | **Cure infected files** (disinfection mode) |
| `/d` | Create memory dumps of scanned PE files |
| `/di` | Display loop detection information (debug) |
| `/t-` | Disable emulation timer (may cause hangs) |

**Examples:**

### Scan a directory and cure infected files
```
XPL.EXE /c C:\samples\
```
### Scan all drives and create a report
```
XPL.EXE /* /rc
```

## 📄 License
This project is licensed under the GNU General Public License v3. A copy of the license is included in the repository (LICENSE.txt). An unofficial Russian translation is also provided for convenience.

## 🙏 Acknowledgements & Historical Note

The original version of this antivirus dates back to 2008 and has been maintained as an educational project.

The disinfection routines were originally written for specific virus families that were prevalent in the 2000s.

The project is a tribute to the golden era of low-level virus engineering and serves as a learning resource for reverse engineers and security researchers.

First versions (001 and 010) were published on [https://www.sac.sk](https://www.sac.sk) (use search text '*Explosion Antivirus*').

© 2004–2008 Most Needful Things [MNT]. Re-released for preservation, 2026.

