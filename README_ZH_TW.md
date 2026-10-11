# Sysfix

[English](README.md) | [简体中文](README_ZH.md) | [繁體中文](README_ZH_TW.md)

輕量級 Windows 命令列系統修復工具，用於診斷並修復常見的網路問題和系統卡頓。

## 功能

- **斷網診斷**：檢測網路線未插穩、Wi-Fi 未連接、已連接但無 Internet 的狀態，並自動檢測 DNS 異常，可一鍵切換至 `8.8.8.8` / `114.114.114.114`。
- **系統卡頓清理**：清空 `%TEMP%` 和 `%WINDIR%\Temp` 暫存檔案目錄，並清空資源回收筒。
- **中英文雙語介面**：啟動時可自由切換。
- **相容性**：支援 Windows 7 及以上版本（x86 / x64）。

## 下載

- **官方網站（全球 CDN）**：[點擊前往官網下載](https://sysfix-officewebite.netlify.app/) — 官網點擊下載按鈕即可獲取最新 ZIP 包。
- **GitHub Releases**：[點擊下載最新版本](../../releases/latest)

壓縮檔內含兩個可執行檔：
- `Sysfix_v0.0.1b1_x86.exe`：32 位元版本。推薦，相容 32 位元和 64 位元 Windows。
- `Sysfix_v0.0.1b1_x64.exe`：64 位元版本。僅適用於 64 位元 Windows。

如果不確定系統位元數，請選擇 **x86** 版本。

## 使用方法

1. 下載並解壓 ZIP 檔案。
2. 右鍵點擊 exe 檔案，選擇 **以系統管理員身分執行**。
3. 選擇語言（English / 中文）。
4. 在主選單選擇需要修復的功能。

> 提示：修改 DNS 和清理系統暫存檔案需要系統管理員權限。

## 授權條款

本專案基於 **GNU General Public License v3.0** 發布。
詳情請參閱 [LICENSE](LICENSE) 文件。

Copyright (C) 2026 buccnqu
