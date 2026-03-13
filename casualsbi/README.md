# CasualSBI - Hardware Abstraction Layer

CasualSBI 是一个跨架构的硬件抽象层（HAL）项目，目标是支持 ARM 和 RISC-V 处理器，向上层提供统一的硬件操作接口。

## 项目目标

- **多架构支持**: 支持 ARM (32/64位) 和 RISC-V (32/64位) 处理器
- **统一接口**: 提供一致的硬件操作 API
- **平台抽象**: 封装不同平台的硬件差异
- **易于移植**: 清晰的架构分层，方便移植到新平台

## 当前状态

✅ 已实现:
- **RISC-V 64位 (rv64)** 完整支持
- **ARM 64位 (AArch64)** 完整支持
- **QEMU Virt 平台** 支持
- **UART 控制台驱动** (NS16550A/PL011)
- **硬件抽象层接口** (HAL)
- **格式化输出** (printf)
- **跨架构构建系统**

🚧 计划中:
- RISC-V 32位支持
- 中断控制器 (PLIC/GIC) 支持
- 定时器中断支持
- 内存管理
- 设备树解析

## 项目结构

```
casualsbi/
├── Makefile                  # 主构建文件（支持多架构）
├── README.md                 # 项目说明
├── include/                  # 头文件目录
│   ├── casualsbi.h          # 主头文件，定义HAL接口
│   ├── stdint.h             # 标准整数类型
│   ├── stdbool.h            # 布尔类型
│   ├── stdarg.h             # 可变参数
│   ├── platform.h           # 平台特定定义
│   └── arch/
│       ├── riscv/
│       │   └── arch.h       # RISC-V架构定义
│       └── arm/
│           └── arch.h       # ARM架构定义
├── arch/                     # 架构特定代码
│   ├── riscv/rv64/          # RISC-V64实现（完整）
│   │   ├── start.S          # 启动代码
│   │   ├── trap.S           # 异常处理
│   │   └── init.c           # 架构初始化
│   └── arm/rv64/            # ARM64实现（框架）
│       ├── start.S          # 启动代码
│       └── init.c           # 架构初始化
├── platform/                 # 平台特定代码
│   └── qemu-virt/
│       ├── uart.c           # UART驱动
│       ├── platform.c       # 平台初始化（跨架构）
│       ├── linker-riscv.ld  # RISC-V链接脚本
│       └── linker-arm.ld    # ARM链接脚本
├── lib/                      # 通用库
│   ├── main.c               # 主入口（跨架构）
│   ├── stdio.c              # 标准I/O
│   └── string.c             # 字符串操作
└── docs/                     # 文档目录
```

## 构建要求

- RISC-V 64位 GCC 工具链 (`riscv64-linux-gnu-gcc` 或 `riscv64-unknown-elf-gcc`)
- QEMU RISC-V 模拟器 (`qemu-system-riscv64`)
- Make

### 安装依赖 (Ubuntu/Debian)

```bash
# RISC-V工具链
sudo apt-get install gcc-riscv64-linux-gnu

# QEMU
sudo apt-get install qemu-system-riscv

# 或者使用 riscv-unknown-elf-gcc (更纯净的裸机工具链)
sudo apt-get install gcc-riscv64-unknown-elf
```

## 使用方法

### 编译

**RISC-V 64-bit (默认):**
```bash
cd casualsbi
make
# 或明确指定
make ARCH=riscv
```

**ARM 64-bit (AArch64):**
```bash
cd casualsbi
make ARCH=arm
```

如果需要使用不同的交叉编译工具链:

```bash
# RISC-V
make ARCH=riscv CROSS_COMPILE=riscv64-unknown-elf-

# ARM
make ARCH=arm CROSS_COMPILE=aarch64-linux-gnu-
```

### 运行

```bash
make run
```

这会在 QEMU Virt 平台上启动 CasualSBI。

### 调试

```bash
make debug
```

然后在新终端中:

```bash
riscv64-linux-gnu-gdb build/riscv-rv64-qemu-virt/casualsbi.elf \
    -ex 'target remote :1234' \
    -ex 'break sbi_main' \
    -ex 'continue'
```

### 清理

```bash
make clean
```

## 硬件抽象层接口

CasualSBI 提供以下统一接口:

### 控制台接口
```c
void sbi_putc(char c);        // 输出单个字符
char sbi_getc(void);          // 读取单个字符
bool sbi_console_avail(void); // 检查输入是否可用
void sbi_puts(const char *s); // 输出字符串
void sbi_printf(const char *fmt, ...); // 格式化输出
```

### 定时器接口
```c
uint64_t sbi_get_time(void);      // 获取当前时间
void sbi_set_timer(uint64_t stime); // 设置定时器
```

### 平台控制接口
```c
void sbi_shutdown(void);  // 关机
void sbi_reboot(void);    // 重启
```

## 添加新架构/平台

### 添加新架构 (如 ARM)

1. 创建 `include/arch/arm/arch.h` - 架构特定定义
2. 创建 `arch/arm/<subarch>/start.S` - 启动代码
3. 创建 `arch/arm/<subarch>/init.c` - 架构初始化
4. 在 `Makefile` 中添加架构支持

### 添加新平台

1. 创建 `platform/<platform-name>/` 目录
2. 实现 `platform.c` - 平台初始化
3. 实现设备驱动 (如 UART)
4. 创建 `linker.ld` - 链接脚本

## 演示功能

当前实现包含一个简单的演示程序:

1. 启动时打印系统信息（自动识别架构）
2. 显示格式测试（测试 printf 的各种格式）
3. 3秒倒计时后自动关机

### 运行效果 (RISC-V)

```
========================================
  CasualSBI Hardware Abstraction Layer
========================================

Architecture: RISC-V 64-bit
Platform: QEMU Virt Machine
Version: 0.1.0

Current time: 0x...

Format test:
  Integer: 42
  Negative: -123
  Unsigned: 4294967295
  Hex: 0xabcd
  Long hex: 0xdeadbeefcafebabe
  Pointer: 0x0000000080000000
  String: Hello from CasualSBI!
  Char: X

System initialized successfully!
Entering main loop...

Demo complete. Shutting down in 3 seconds...
3... 2... 1... Goodbye!
```

### 运行效果 (ARM64)

```
========================================
  CasualSBI Hardware Abstraction Layer
========================================

Architecture: ARM 64-bit (AArch64)
Platform: QEMU Virt Machine
Version: 0.1.0

Current time: 0x...

Format test:
  Integer: 42
  Negative: -123
  Unsigned: 4294967295
  Hex: 0xabcd
  Long hex: 0xdeadbeefcafebabe
  Pointer: 0x0000000040000000
  String: Hello from CasualSBI!
  Char: X

System initialized successfully!
Entering main loop...

Demo complete. Shutting down in 3 seconds...
3... 2... 1... Goodbye!
```

## 许可证

MIT License - 详见 LICENSE 文件

## 贡献

欢迎提交 Issue 和 PR！
