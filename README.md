# esp32_p1 — ESP32 学习项目

ESP32 单片机开发学习：环境搭建 → WiFi 操作 → 网络代理（配合 proxy_opencode 实现桶/数据转发）。

## 阶段规划

1. **环境搭建** — 开发框架、工具链、烧录验证 ✅
2. **WiFi** — STA/AP 模式、连接管理、扫描、省电
3. **网络代理桶** — ESP32 作为数据转发节点，对接 proxy_opencode

## 目录结构

```
esp32_p1/    PlatformIO 工程主体 (src/main.cpp)
examples/    示例代码
docs/        学习笔记与环境记录
```

## 环境（已就绪，2026-10-27 验证编译通过）

- 开发板：**ESP32-C3-DevKitC-02**（RISC-V 单核 160MHz，4MB Flash，USB-Serial/JTAG 免驱）
- 框架：**VS Code + PlatformIO**（espressif32 8.0.0 / Arduino core 3.3.12）
- 工具链：toolchain-riscv32-esp 14.2.0（C3 用 RISC-V，xtensa 工具链为平台全家桶自带）
- 烧录：esptool 5.4.0，串口 COM5

## 快速开始

```bash
# 在 esp32_p1/ 目录下
pio run            # 编译
pio run -t upload  # 烧录（按住 BOOT 键进入下载模式，首次需要）
pio device monitor # 串口监视器 115200
```

## 备注

- Arduino IDE 与 PlatformIO 工具链相互独立，可卸载 Arduino IDE 省 1.5GB 磁盘
- PlatformIO CLI 已内置于 VS Code 插件（`~/.platformio/penv/Scripts/pio.exe`），无需单独安装
