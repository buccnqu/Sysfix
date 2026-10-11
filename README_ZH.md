# Sysfix

[English](README.md) | [简体中文](README_ZH.md) | [繁體中文](README_ZH_TW.md)

轻量级 Windows 命令行系统修复工具，用于诊断并修复常见的网络问题和系统卡顿。

## 功能

- **断网诊断**：检测网线未插稳、Wi-Fi 未连接、已连接但无 Internet 的状态，并自动检测 DNS 异常，可一键切换至 `8.8.8.8` / `114.114.114.114`。
- **系统卡顿清理**：清空 `%TEMP%` 和 `%WINDIR%\Temp` 临时文件目录，并清空回收站。
- **中英文双语界面**：启动时可自由切换。
- **兼容性**：支持 Windows 7 及以上版本（x86 / x64）。

## 下载

- **官方网站（全球 CDN）**：[点击前往官网下载](https://sysfix-officewebite.netlify.app/) — 官网点击下载按钮即可获取最新 ZIP 包。
- **GitHub Releases**：[点击下载最新版本](../../releases/latest)

压缩包内含两个可执行文件：
- `Sysfix_v0.0.1b1_x86.exe`：32 位版本。推荐，兼容 32 位和 64 位 Windows。
- `Sysfix_v0.0.1b1_x64.exe`：64 位版本。仅适用于 64 位 Windows。

如果不确定系统位数，请选择 **x86** 版本。

## 使用方法

1. 下载并解压 ZIP 文件。
2. 右键 exe 文件，选择 **以管理员身份运行**。
3. 选择语言（English / 中文）。
4. 在主菜单选择需要修复的功能。

> 提示：修改 DNS 和清理系统临时文件需要管理员权限。

## 许可证

本项目基于 **GNU General Public License v3.0** 发布。
详情请参阅 [LICENSE](LICENSE) 文件。

Copyright (C) 2026 buccnqu
