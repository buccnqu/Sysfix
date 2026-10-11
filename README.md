# Sysfix

A lightweight command-line system repair tool for Windows. It helps diagnose and fix common network problems and system slowness.

## Features

- **Network repair**: Detects disconnected Ethernet/Wi-Fi, checks Internet access, and diagnoses DNS issues. Can automatically switch DNS to `8.8.8.8` / `114.114.114.114`.
- **System cleanup**: Cleans temporary files from `%TEMP%` and `%WINDIR%\Temp`, and empties the Recycle Bin.
- **Bilingual interface**: English and Chinese, selectable at startup.
- **Compatibility**: Works on Windows 7 and later (x86 / x64).

## Download

- **Official Website (Global CDN)**: [Download from Netlify](https://sysfix-officewebite.netlify.app/) — just click the download button to get the latest ZIP.
- **GitHub Releases**: [Download the latest version](../../releases/latest)

The release package contains two executables:
- `Sysfix_v0.0.1b1_x86.exe` — 32-bit build. Recommended, works on both 32-bit and 64-bit Windows.
- `Sysfix_v0.0.1b1_x64.exe` — 64-bit build. Only for 64-bit Windows.

If you are not sure which one to use, choose the **x86** version.

## Usage

1. Download and extract the ZIP file.
2. Right-click the exe and select **Run as administrator**.
3. Choose your language (English / 中文).
4. Select a repair option from the main menu.

> Note: Administrator privileges are required to modify DNS settings and clean system temp files.

## License

This project is licensed under the **GNU General Public License v3.0**.
See the [LICENSE](LICENSE) file for details.

Copyright (C) 2026 buccnqu
