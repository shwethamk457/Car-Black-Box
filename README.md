# 🚗 Car Black Box — Event Data Recorder
An embedded systems project implementing a vehicle Event Data Recorder (EDR) using the **PIC18F4580 microcontroller**, designed to continuously log vehicle parameters and detect accident events.

## 📌 Project Overview
The Car Black Box is an embedded firmware project that records critical vehicle data such as speed, gear position, and timestamp in real time. In the event of an accident or abnormal driving condition, the recorded data can be viewed, downloaded, or cleared through a menu-driven LCD interface.
This project simulates the functionality of automotive-grade Event Data Recorders (EDR) used in vehicles, aviation, railways, and industrial systems.

## 🛠️ Hardware Components
| Component | Model | Interface |
|---|---|---|
| Microcontroller | PIC18F4580 | — |
| Real Time Clock | DS1307 | I2C |
| Display | 16x2 Character LCD | Parallel |
| Input | 4x4 Matrix Keypad | GPIO |
| Storage | Internal EEPROM | Built-in |
| Speed Sensor | Potentiometer (ADC) | ADC Channel 4 |
| UART Output | USB to Serial / PC | UART |

## 📐 System Architecture
┌─────────────────────────────────────────────────┐
│                  PIC18F4580                      │
│                                                  │
│  ADC ◄──── Speed Sensor (Potentiometer)          │
│  I2C ◄──► DS1307 RTC (Timestamp)                │
│  GPIO ◄─── Matrix Keypad (User Input)            │
│  GPIO ───► 16x2 LCD (Display)                   │
│  UART ───► PC Terminal (Download Log)            │
│  EEPROM ── Internal (Data Storage)               │
└─────────────────────────────────────────────────┘

## 📂 Project File Structure
car-black-box/
│
├── main.c               # Entry point — init and main loop
├── black_box.c          # Core vehicle data processing & menu state machine
├── menu_data.c          # Menu functions: view, clear, download, set time
├── message_handler.h    # Macro definitions and function declarations
│
├── drivers/
│   ├── adc.c / adc.h           # ADC driver for speed reading
│   ├── clcd.c / clcd.h         # Character LCD driver
│   ├── i2c.c / i2c.h           # I2C master driver
│   ├── ds1307.c / ds1307.h     # RTC driver
│   ├── EEPROM.c / EEPROM.h     # Internal EEPROM read/write
│   ├── matrix_keypad.c / .h    # Matrix keypad driver
│   └── uart.c / uart.h         # UART driver for data download
│
└── README.md

## ⚙️ Development Tools
| Tool | Purpose |
|---|---|
| MPLAB X IDE | Project development environment |
| XC8 Compiler | PIC C compiler |
| PICkit 3 / 4 | Programmer and debugger |
| Proteus 8 | Circuit simulation |
| USB to UART | Serial terminal for log download |

## 🗂️ EEPROM Data Storage Format
Each event occupies **12 bytes** in internal EEPROM. Up to **10 events** are stored in a circular buffer.
Address Offset   Data Stored
─────────────────────────────
offset + 0       Speed tens digit
offset + 1       Speed units digit
offset + 2       Gear char 1 (e.g. 'G')
offset + 3       Gear char 2 (e.g. '3')
offset + 4       Hour tens
offset + 5       Hour units
offset + 6       ':' separator
offset + 7       Minute tens
offset + 8       Minute units
offset + 9       ':' separator
offset + 10      Second tens
offset + 11      Second units

Special Address:
0x7F             → Stores current event_index (count)

## 🖥️ LCD Menu Structure
POWER ON
    │
    ▼
┌─────────────────┐
│ DASHBOARD       │  flag = 0
│ SPD  GR  TIME   │
│ 45   G3  10:30  │
└─────────────────┘
    │ SWITCH11
    ▼
┌─────────────────┐
│ MENU            │  flag1 = 1 to 4
│ * View Log      │
│   Clear Log     │
└─────────────────┘
    │
    ├──► View Log     (flag = 1) — Browse stored events
    ├──► Clear Log    (flag = 2) — Erase all EEPROM logs
    ├──► Download Log (flag = 3) — Send data via UART
    └──► Set Time     (flag = 4) — Configure RTC time

## 🔘 Switch Key Mapping
| Switch | Dashboard | Menu | View Log | Set Time |
|---|---|---|---|---|
| SWITCH1 | Gear UP | Scroll DOWN | Next log entry | Increment field |
| SWITCH2 | Gear DOWN | Scroll UP | Previous log entry | Change field |
| SWITCH3 | Reverse gear | — | — | — |
| SWITCH11 | Open Menu | SELECT item | — | Save to RTC |
| SWITCH12 | — | Back / Exit | Back to menu | Back to menu |

## 📊 State Machine — flag Values
flag = 0  →  DASHBOARD (default)
flag = 1  →  VIEW LOG
flag = 2  →  CLEAR LOG
flag = 3  →  DOWNLOAD LOG
flag = 4  →  SET TIME

## 📤 UART Download Format
When **Download Log** is selected, all 10 stored events are sent via UART to the PC terminal in the following format:
HH:MM:SS_GR_SS
HH:MM:SS_GR_SS
...
**Example output:**
10:30:25_G3_45
10:31:10_G4_60
10:32:05_G5_72

**UART Settings:**
- Baud Rate : 9600
- Data Bits : 8
- Parity    : None
- Stop Bits : 1
  
## 🔧 Key Macros — message_handler.h
#define DASHBOARD     (flag == 0)
#define VIEW_LOG      (flag == 1)
#define CLEAR_LOG     (flag == 2)
#define DOWNLOAD_LOG  (flag == 3)
#define SET_TIME      (flag == 4)

## 🌀 Gear Position Mapping
char *gear[] = {"GN","G1","G2","G3","G4","G5","GR","COL"};

Index 0 → GN  (Neutral)
Index 1 → G1  (1st Gear)
Index 2 → G2  (2nd Gear)
Index 3 → G3  (3rd Gear)
Index 4 → G4  (4th Gear)
Index 5 → G5  (5th Gear)
Index 6 → GR  (Reverse)
Index 7 → COL (Collision / Accident)

## 🔄 Data Flow
Speed Sensor (ADC)
        │
        ▼
Read ADC → scale to 0–99 km/h
        │
        ▼
Compare with previous speed & gear
        │
  Changed? ──NO──► Continue monitoring
        │
       YES
        │
        ▼
event_index++
Store to EEPROM (speed + gear + timestamp)
        │
        ▼
Display on LCD Dashboard

## 🧠 Key Embedded Concepts Used
- **ADC** — Analog speed sensor reading
- **I2C Protocol** — DS1307 RTC communication
- **UART Protocol** — Log download to PC terminal
- **Internal EEPROM** — Non-volatile data storage
- **Circular Buffer** — 10-event overwrite storage
- **State Machine** — Menu navigation using flag variables
- **Matrix Keypad Scanning** — STATE_CHANGE debounce logic
- **BCD Conversion** — DS1307 register decoding for time display
- **Interrupt-driven blink** — Cursor blinking in Set Time screen

## ⚠️ Known Limitations
- Maximum 10 events stored (circular buffer overwrites oldest)
- Speed input is simulated via potentiometer (not actual vehicle sensor)
- No GPS module — location tracking not included
- 12-hour RTC format used in current version

## 🚀 How to Build and Flash
# Step 1: Open project in MPLAB X IDE
File → Open Project → select car-black-box.X

# Step 2: Select compiler
XC8 v2.x or later

# Step 3: Build project
Production → Build Main Project (Ctrl+F11)

# Step 4: Connect PICkit3/4 to PIC18F4580

# Step 5: Flash firmware
Production → Make and Program Device

## 📈 Future Enhancements
- [ ] GPS module (NEO-6M) for location tracking
- [ ] GSM module (SIM800L) for SMS accident alerts
- [ ] Accelerometer (MPU-6050) for impact detection
- [ ] CAN Bus interface using PIC18F4580 built-in CAN controller
- [ ] SD card storage for unlimited event logging
- [ ] Android app for wireless log download via Bluetooth
- [ ] Cloud upload via Wi-Fi (ESP8266)
