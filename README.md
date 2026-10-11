# Sysfix

A lightweight command-line system repair tool for Windows.  
It helps diagnose and fix common network problems and system slowness.

## Features

- **Network repair**
  - Detects whether the Ethernet cable is unplugged or Wi-Fi is disconnected.
  - Checks Internet connectivity.
  - Diagnoses DNS issues and can automatically set DNS to `8.8.8.8` (preferred) and `114.114.114.114` (alternate).
  - Suggests router / broadband checks if DNS is not the problem.
- **System cleanup**
  - Cleans temporary files from `%TEMP%` and `%WINDIR%\Temp`.
  - Empties the Recycle Bin.
- **Bilingual interface**
  - English and Chinese (selectable at startup).
- **Compatibility**
  - Works on Windows 7 and later (x86 / x64).

## Download

- **GitHub Releases**  
  [Download the latest version](../../releases/latest)

- **������ (for users in China)**  
  [Download from ������](your-lanzou-link)  
  Password: `xxxx`

The release package contains two executables:

| File | Description |
|------|-------------|
| `Sysfix_v0.0.1b1_x86.exe` | 32-bit build. Recommended �� works on both 32-bit and 64-bit Windows. |
| `Sysfix_v0.0.1b1_x64.exe` | 64-bit build. Only for 64-bit Windows. |

If you are not sure which one to use, choose the **x86** version.

## Usage

1. Download and extract the ZIP file.
2. Right-click `Sysfix_v0.0.1b1_x86.exe` (or the x64 version) and select **Run as administrator**.
3. Choose your language (English / ����).
4. Select a repair option from the main menu:
   - `1` �� Repair network problems
   - `2` �� Fix system slowness (clean temp files and Recycle Bin)
   - `0` �� Exit
5. Follow the on-screen instructions.

> **Note:** Administrator privileges are required to modify DNS settings and clean system temp files.

## System Requirements

- Windows 7 or later (x86 / x64)
- Administrator rights (recommended for full functionality)

## Build from Source

The source code is written in C++ and supports both MSVC and MinGW-w64.

### MSVC (Visual Studio 2022)

Open a Developer Command Prompt and run:

```cmd
cl /nologo /W3 /O2 /EHsc /utf-8 /MT /D_WIN32_WINNT=0x0601 /D_WINVER=0x0601 Sysfix.cpp /link /SUBSYSTEM:CONSOLE
[README.txt](https://github.com/user-attachments/files/33293973/README.txt)
