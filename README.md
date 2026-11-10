# MiKann-FOC

基于 **STM32F103C8T6** 的永磁同步电机 FOC 控制与调试工程。工程使用 STM32 HAL、裸机周期任务、AS5600 磁编码器、OLED 状态显示和按键校准，当前默认演示 **7 对极电机的电压模式位置闭环**。

> 这是面向电机控制学习与实验的工程，不是可直接用于量产的安全控制器。当前代码没有相电流采样、过流保护、母线电压检测和故障停车流程。首次上电、修改控制参数或带负载运行时，请先断开电机动力电源并确认驱动板接线。

## 当前功能

- STM32F103C8T6，HSE 8 MHz 经 PLL 倍频至 72 MHz。
- TIM1 三路基础 PWM 输出，频率 72 kHz。
- TIM2 产生 10 kHz 调度节拍，主循环按周期执行软件任务。
- AS5600 通过模拟输出（ADC1_IN7）读取机械角。
- 7 对极电机的电角度换算、位置式 PID、低通滤波和正弦电压调制。
- 128×64 SSD1306 I2C OLED 状态显示。
- 按键短按触发电角度零点校准，板载 LED 以 1 Hz 翻转。
- 预留 AS5600 I2C 读取、串口遥测、速度闭环、阻尼/棘轮手感模拟和页面框架。

### 控制边界

当前默认闭环是：

```text
AS5600 机械角
    -> 机械角换算 / 电角度对齐
    -> 位置 PID -> Uq
    -> 反 Park + 反 Clarke
    -> 三相 SPWM 占空比
    -> 电机驱动板
```

代码尚未实现电流闭环。`Ud` 参数虽然保留在接口中，但没有参与逆 Park 变换；所谓“力矩”接口实际输出的是电压幅值。因此，当前工程适合验证角度读取、转子对齐、位置控制和手感算法，不应把它视为完整的电流矢量控制实现。

## 硬件与接线

### 已验证/已配置的引脚

| STM32 引脚 | 功能 | 用途 |
| --- | --- | --- |
| PA8 | TIM1_CH1 | 电机驱动 PWM 1 / IN1 |
| PA9 | TIM1_CH2 | 电机驱动 PWM 2 / IN2 |
| PA10 | TIM1_CH3 | 电机驱动 PWM 3 / IN3 |
| PA7 | ADC1_IN7 | AS5600 模拟角度输出 |
| PB6 / PB7 | I2C1 SCL / SDA | OLED |
| PB10 / PB11 | I2C2 SCL / SDA | AS5600 I2C 备用接口，默认未启用 |
| PA2 / PA3 | USART2 TX / RX | 串口通信，默认发送任务未启用 |
| PA15 | GPIO 输入，下拉 | 蓝牙/上位机链路状态 `BTE` |
| PB8 | GPIO 输入，下拉 | 按键 `KEY`，按下时为高电平 |
| PC13 | GPIO 输出 | 状态 LED |
| PA13 / PA14 | SWDIO / SWCLK | ST-Link/OpenOCD 调试烧录 |

### 接线注意事项

- PWM 三相差序必须与驱动板和电机相序一致；`IN1/IN2/IN3` 顺序接反会导致换向异常。
- MCU 与编码器、驱动板必须共地，GPIO 逻辑电平不得超过 3.3 V。
- 默认母线电压在 `FOC/FOC_Util.h` 中设置为 `12.6 V`。实际电源不同时必须同步修改，否则会直接影响输出电压和占空比。
- AS5600 默认路径是模拟输出，而不是 I2C。若切换为 I2C，需要启用 `AS5600_ReadRawAngle_I2C()` 并处理读取失败。
- 当前 TIM1 未配置互补输出、死区、刹车输入和自动输出，必须由外部驱动板提供与硬件匹配的保护。

## 软件运行流程

```text
main()
  HAL_Init / SystemClock_Config / 外设初始化
  TIM2 启动 100 us 中断
  TIM1 三路 PWM 启动
  Task_Init()
    Display_Init()
    FOC_Init()
  while (1)
    TIM2 标志置位 -> TaskExe() -> 执行到期任务
```

TIM2 的中断只负责置位 `TaskExeFlag`，实际业务运行在主循环中。任务表位于 `User/task.c`，表中的 `period` 以 100 us 为单位：

| 任务 | 周期 |
| --- | ---: |
| `time_task` | 100 us |
| `AS5600_Update` | 500 us |
| `FOC_Task` | 5 ms |
| `Velocity_SinCos_Update` | 50 ms |
| `Display_Task` | 10 ms |
| `KeySL_Task` | 20 ms |
| `count_task` | 1 s |

默认 `FOC_Task()` 将目标角固定为 **45°**，并调用 `angleControl_loop(45.0f)`。速度开环、位置闭环、速度闭环和手感模拟入口都在 `FOC/FOC.c`、`FOC/FOC_Simulation.c` 中，可在 `FOC/FOC_App.c` 切换。

## 编译与烧录

### 推荐方式：VS Code + EIDE

仓库已经包含 `.eide/eide.yml` 和 `.vscode/tasks.json`，这是目前能够编译完整应用模块的构建入口。

1. 安装 Visual Studio Code。
2. 安装 **EIDE** 扩展，以及 EIDE 使用的 ARM GCC 工具链（`arm-none-eabi-gcc`）。
3. 使用 VS Code 打开 `Mikann-FOC.code-workspace`。
4. 确认 EIDE 的编译器路径、芯片型号 `STM32F103C8Tx` 和下载器配置。
5. 执行 `Tasks: Run Task` 中的 `build`；连接烧录器后可执行 `flash` 或 `build and flash`。

当前 EIDE 配置使用 **OpenOCD + ST-Link（SWD）**，目标文件输出到：

```text
build/Debug/Mikann-FOC.elf
build/Debug/Mikann-FOC.hex
build/Debug/Mikann-FOC.map
```

最近一次本地构建日志显示：RAM 使用约 5,168 B（20 KB 的 25.23%），Flash 使用约 24,360 B（64 KB 的 37.17%）。具体数值会随编译器和代码变化。

### Makefile 状态

根目录的 `Makefile` 是 STM32CubeMX 生成的基础版本，但目前只包含 `Core` 和 HAL 源文件，未加入 `User`、`FOC`、`Encoder`、`Display`、`Communication`、`Key` 模块，也缺少这些模块的头文件路径。直接执行 `make` 不能可靠构建完整固件；在修复 Makefile 前，请使用 EIDE 工程构建。

### STM32CubeMX

`MiKann-FOC.ioc` 保存了芯片、时钟、引脚和外设配置。重新生成代码时：

- 保留 `USER CODE BEGIN/END` 之间的内容。
- 确认 CubeMX 生成后没有覆盖 `Core/Src/main.c` 中的任务启动代码和中断回调。
- CubeMX 可能重新生成或覆盖 Makefile，重新生成后需再次检查自定义源文件和 include 路径。

## 上电与校准

1. 先断开电机动力电源，只给 MCU、AS5600 和 OLED 供电。
2. 编译并烧录固件，确认 OLED 能显示转角、角度、速度和加速度栏位。
3. 手动转动电机轴，确认角度变化方向与预期一致，并且跨过 0°/360° 时圈数连续。
4. 在电机轴可自由转动、驱动板已正确接线的条件下，短按 PB8 上的按键。
5. 校准函数会对电机施加约 1 s 的固定电压，使转子对齐，然后记录电角度零点。

注意：按键短按会进入 `Zero_Electric_Init()`，其中包含阻塞式 `HAL_Delay(1000)`。校准期间主循环、显示刷新和通信都会暂停，电机会产生明显力矩。该零点只保存在 RAM 中，掉电或复位后需要重新校准。

## OLED 页面

主页当前显示：

- 累计圈数；
- 0～360° 机械角；
- 正弦/余弦微分法测得的角速度；
- 速度变化量（当前字段名仍为 `accel`）；
- 1 s 计数值。

`Display/Pages/Page_Example.c` 是新页面模板。新增页面时需要同时修改 `Display.h` 中的 `Page_enum`、`Pages.h`、`Display_Subtrate()` 和 `Display_Task()`。

## 串口数据

USART2 配置为 `115200 8N1`，引脚为 PA2/PA3。`Communication/UART_Send_Task()` 仍处于实验状态，并且已从任务表中注释掉。

设计意图中的帧格式为：

```text
CC 33 <angle low> <angle high> 66 99
```

但当前实现只有 4 字节缓冲区，角度高字节会被结束字节覆盖，且没有第二个结束字节。启用串口遥测前需要先修正缓冲区和打包逻辑。`PA15` 用作链路状态输入，只有在 `BTE` 为高电平时才会发送。

## 目录结构

```text
.
├── Core/                 # CubeMX 生成的启动、时钟、中断和 HAL 外设初始化
├── Drivers/              # STM32F1 HAL 与 CMSIS
├── User/                 # 周期任务、类型定义、位带操作
├── FOC/                  # 控制算法入口、电压调制、PID、数学工具
│   └── Math/
├── Encoder/              # AS5600 角度、圈数和速度估计
├── Display/              # OLED 驱动与页面框架
├── Communication/        # USART 数据帧与链路状态
├── Key/                  # 按键短按/长按状态机
├── MiKann-FOC.ioc        # STM32CubeMX 配置
├── .eide/                # EIDE 构建配置
├── Makefile              # CubeMX 生成的 Makefile，当前不包含完整应用
└── STM32F103XX_FLASH.ld  # 链接脚本
```

## 后续方向

- 将母线电压、极对数、控制周期、方向、限幅和 PID 参数集中到电机配置文件中。
- 增加相电流采样，补齐 Clarke/Park、Id/Iq 电流环和故障保护。
- 将时间基准与业务调度从协作式主循环中解耦，降低任务超时造成的周期漂移。
- 提供 `target_angle` 和串口命令接口，替代 `FOC_App.c` 中固定 45° 的测试目标。
- 修复并验证 UART 遥测帧，加入 CRC、明确字节序和超时恢复。
- 为数学函数、角度跨越和 PID 控制器增加主机端单元测试或固定向量测试。

## 许可证

仓库目前没有提供 `LICENSE` 文件。除第三方 STM32 HAL/CMSIS 代码遵循其目录内许可证外，项目自有代码的复用和分发条件尚未明确。
