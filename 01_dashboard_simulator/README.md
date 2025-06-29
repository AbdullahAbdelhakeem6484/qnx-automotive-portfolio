# 🚗 QNX Automotive Dashboard Simulator

![QNX Dashboard](https://img.shields.io/badge/QNX-Dashboard-blue.svg)
![Beginner](https://img.shields.io/badge/Level-Beginner-green.svg)
![Real-Time](https://img.shields.io/badge/Real--Time-System-brightgreen.svg)

## 📋 Project Overview

A professional automotive dashboard simulator built with **QNX SDP 8.0**, demonstrating real-time systems development, multi-threaded architecture, and automotive GUI design principles. This project showcases both console and graphical dashboard implementations with realistic vehicle data simulation.

## ✨ Features

### 🎮 **Dual Interface Support**
- **Console Dashboard**: ASCII-based interface for terminal environments
- **GUI Dashboard**: Rich graphical interface with ANSI color support
- **Real-time Updates**: 20-30 FPS refresh rate with smooth animations
- **Multi-threaded Architecture**: Separate threads for GUI rendering and data processing

### 🚗 **Vehicle Data Simulation**
- **Engine RPM**: Realistic engine revolutions (800-7000 RPM)
- **Vehicle Speed**: Speed simulation with acceleration/deceleration
- **Engine Temperature**: Thermal dynamics with overheating warnings
- **Fuel Level**: Fuel consumption simulation
- **Warning Systems**: Engine, oil, battery, and temperature alerts

### ⚡ **Performance Features**
- **Real-time Threading**: SCHED_FIFO and SCHED_RR scheduling policies
- **Memory Optimized**: <20MB total memory usage
- **Fast Startup**: <5 seconds initialization
- **Responsive**: <15ms input response time

## 🚀 Quick Start

### Prerequisites
- QNX SDP 8.0 (Community Edition supported)
- QEMU (for virtualization testing)
- Build tools: make, qcc

### Build & Run

```bash
# Build both console and GUI versions
make all

# Run console dashboard
./console_dashboard

# Run GUI dashboard  
./gui_dashboard

# Clean build artifacts
make clean
```

### Build Configurations

```bash
# Debug build with symbols
make debug

# Release build (optimized)
make release

# Automotive build (safety-critical)
make automotive

# Performance build (high-speed)
make performance
```

## 🏗️ Architecture

### System Design
```
┌─────────────────────────────────────────────────────────────────┐
│                    QNX Dashboard Simulator                      │
├─────────────────────────────────────────────────────────────────┤
│  GUI Layer          │  Data Processing     │  System Layer      │
│  ┌─────────────┐   │  ┌─────────────┐     │  ┌─────────────┐   │
│  │ Rendering   │   │  │ Simulation  │     │  │ Threading   │   │
│  │ Input       │   │  │ Validation  │     │  │ Memory      │   │
│  │ Display     │   │  │ Filtering   │     │  │ Scheduling  │   │
│  └─────────────┘   │  └─────────────┘     │  └─────────────┘   │
└─────────────────────────────────────────────────────────────────┘
```

### Threading Model
- **GUI Thread**: 60Hz rendering (Priority: 63, SCHED_FIFO)
- **Data Thread**: 100Hz simulation (Priority: 50, SCHED_RR)  
- **Monitor Thread**: 10Hz monitoring (Priority: 30, SCHED_OTHER)

## 📁 Project Structure

```
01_dashboard_simulator/
├── src/                    # Source code
│   ├── main.c             # Application entry point
│   ├── console_dashboard.c # Console interface
│   ├── gui_dashboard.c    # GUI interface  
│   ├── data_simulator.c   # Vehicle data simulation
│   └── gui_manager.c      # GUI management
├── include/               # Header files
│   ├── dashboard.h        # Main dashboard definitions
│   ├── config.h          # Configuration settings
│   ├── data_simulator.h  # Data simulation API
│   └── gui_manager.h     # GUI management API
├── resources/            # Configuration & assets
│   └── config/
│       └── dashboard.conf # Dashboard configuration
├── scripts/              # Build & test scripts
├── Makefile             # Build system
└── README.md           # This file
```

## 🔧 Configuration

### Dashboard Settings (`resources/config/dashboard.conf`)
```ini
# Display Settings
[display]
width=1024
height=768
fullscreen=false
theme=automotive_dark

# Performance Settings  
[performance]
target_fps=30
data_rate=100
memory_limit=128MB

# Vehicle Settings
[vehicle]
max_speed=250
max_rpm=7000
fuel_capacity=60
```

## 🧪 Testing

### Testing Methods
1. **QEMU Virtualization**: Launch with QEMU for virtual testing
2. **QNX Momentics IDE**: Import as QNX project for development
3. **Raspberry Pi 4**: ARM cross-compilation for hardware testing
4. **QNX Virtualization**: Native QNX testing environment

### Performance Results
| Metric | Target | Achieved |
|--------|--------|----------|
| Build Time | <30s | <5s |
| Executable Size | <50KB | 18KB |
| Memory Usage | <128MB | <20MB |
| Frame Rate | 30 FPS | 20-30 FPS |
| Response Time | <50ms | <15ms |

## 🚀 Installation & Deployment

### QNX Environment Setup
```bash
# Set QNX environment variables
export QNX_HOST=/opt/qnx800/host/linux/x86_64
export QNX_TARGET=/opt/qnx800/target/qnx8

# Add QNX tools to PATH
export PATH=$QNX_HOST/usr/bin:$PATH
```

### QEMU Testing
```bash
# Launch dashboard in QEMU
qemu-system-x86_64 -m 512 -display gtk -name "QNX Dashboard"

# Build QNX bootable image
make qnx_image
```

### Raspberry Pi 4 Deployment
```bash
# Cross-compile for ARM
make TARGET=aarch64le

# Deploy to Pi
scp gui_dashboard pi@raspberrypi:/tmp/
ssh pi@raspberrypi "/tmp/gui_dashboard"
```

## 🔧 Development

### Build System Features
- Multiple build configurations (debug, release, automotive, performance)
- Cross-compilation support for x86_64 and ARM architectures  
- Automated testing and validation
- Memory leak detection and performance profiling
- Code quality checks and static analysis

## 🎯 Learning Outcomes

After completing this project, you will understand:
- **QNX Real-time Programming**: Deterministic system design and scheduling
- **Multi-threading**: Concurrent programming with thread safety
- **GUI Development**: Professional automotive interface design
- **Performance Optimization**: Real-time system tuning and optimization
- **Cross-platform Development**: QEMU virtualization and hardware deployment

## 📚 Technical Specifications

### System Requirements
- **OS**: QNX SDP 8.0 or later
- **Memory**: 256MB RAM minimum
- **Storage**: 100MB available space
- **CPU**: x86_64 or ARM architecture

### Performance Specifications
- **Real-time Constraints**: Hard real-time scheduling guarantees
- **Latency**: <1ms data processing latency
- **Throughput**: 100Hz vehicle data simulation
- **Reliability**: 99.9% uptime reliability target

## 🐛 Troubleshooting

### Common Issues

**Build Failures**
```bash
# Clean and rebuild
make clean && make

# Verify QNX environment
echo $QNX_HOST $QNX_TARGET
```

**Runtime Issues**
```bash
# Run with debug output
./gui_dashboard --debug

# Check system resources
pidin info
```

**GUI Not Displaying**
```bash
# Check display environment
echo $DISPLAY

# Run console version as fallback
./console_dashboard
```

## 📄 License

This project is licensed under the MIT License.

## 🤝 Contributing

Contributions are welcome! Please submit pull requests for any improvements.

---

**Built with ❤️ for the QNX Automotive Community**
