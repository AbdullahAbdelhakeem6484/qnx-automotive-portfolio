# 🧪 QNX Automotive Portfolio - Testing Guide

![Testing](https://img.shields.io/badge/Testing-QEMU%20%7C%20RaspberryPi4-green.svg)
![Validation](https://img.shields.io/badge/Validation-Automated-blue.svg)

## 📋 Overview

This guide provides comprehensive instructions for testing all QNX Automotive Portfolio projects on QEMU (virtualization) and Raspberry Pi 4 (real hardware) platforms.

## 🎯 Testing Platforms

### Platform 1: QEMU Virtualization
- **Purpose**: Development, debugging, and initial validation
- **Advantages**: Rapid iteration, debugging capabilities, consistent environment
- **Target**: x86_64 and ARM virtualization
- **Use Cases**: Algorithm validation, system integration testing

### Platform 2: Raspberry Pi 4
- **Purpose**: Real hardware validation and demonstration
- **Advantages**: Actual hardware interfaces, GPIO, real-time performance
- **Target**: ARM Cortex-A72 quad-core
- **Use Cases**: Hardware validation, demo scenarios, performance testing

## 🚀 Quick Start Testing

### Step 1: Environment Setup
```bash
# Clone the repository
git clone https://github.com/AbdullahAbdelhakeem6484/qnx-automotive-portfolio.git
cd qnx-automotive-portfolio

# Setup QEMU testing environment
cd testing/qemu/setup
./qemu_setup.sh --install-all

# Setup Raspberry Pi 4 environment
cd ../raspberry_pi/setup
./rpi_setup.sh --prepare-all
```

### Step 2: Project Testing
```bash
# Test Project 1: Dashboard Simulator
cd ../../common/test_framework
python test_runner.py --project dashboard --platform qemu
python test_runner.py --project dashboard --platform rpi

# Test Project 2: Image Builder
python test_runner.py --project image_builder --platform qemu
python test_runner.py --project image_builder --platform rpi

# Test Project 3: Real-Time Processing
python test_runner.py --project real_time --platform qemu
python test_runner.py --project real_time --platform rpi

# Test Project 4: Hypervisor
python test_runner.py --project hypervisor --platform qemu
```

### Step 3: Results Analysis
```bash
# Generate comprehensive test report
python report_generator.py --all-projects --output automotive_portfolio_report.html

# View performance metrics
python metrics_collector.py --analyze --visualize
```

## 🎯 Testing Success Criteria

### Project 1: Dashboard Simulator
- ✅ Boot time < 15 seconds
- ✅ GUI frame rate ≥ 60 FPS
- ✅ Memory usage < 256 MB
- ✅ Real-time updates < 100ms

### Project 2: Custom Image Builder
- ✅ Image build success rate = 100%
- ✅ Boot time < 30 seconds
- ✅ Service startup < 5 seconds
- ✅ Hardware driver functionality = 100%

### Project 3: Real-Time Data Processing
- ✅ Processing latency < 1ms
- ✅ Deadline adherence ≥ 99.9%
- ✅ IPC communication < 100μs
- ✅ Fault recovery < 1 second

### Project 4: Automotive Hypervisor
- ✅ VM switch time < 50μs
- ✅ Resource isolation = 100%
- ✅ Multi-OS boot success = 100%
- ✅ Real-time guarantees maintained

## 📊 Performance Benchmarks

### Expected Performance Metrics

| Project | Platform | Boot Time | Memory Usage | CPU Usage | Real-Time Performance |
|---------|----------|-----------|--------------|-----------|----------------------|
| Dashboard | QEMU | <20s | <256MB | <50% | <100ms response |
| Dashboard | RPi4 | <15s | <128MB | <40% | <50ms response |
| Image Builder | QEMU | <30s | <512MB | <30% | Service startup <5s |
| Image Builder | RPi4 | <25s | <256MB | <25% | Service startup <3s |
| Real-Time | QEMU | <25s | <1GB | <70% | <1ms processing |
| Real-Time | RPi4 | <20s | <512MB | <60% | <0.5ms processing |
| Hypervisor | QEMU | <60s | <2GB | <80% | <50μs VM switch |
| Hypervisor | RPi4 | <45s | <1GB | <70% | Resource isolation |

## 🔧 Troubleshooting Guide

### Common QEMU Issues
```bash
# Issue: QEMU won't start
sudo modprobe kvm-intel
sudo usermod -a -G kvm $USER

# Issue: Network problems
./testing/qemu/setup/network_setup.sh --fix

# Issue: Performance problems
./testing/qemu/setup/qemu_setup.sh --optimize
```

### Common Raspberry Pi Issues
```bash
# Issue: Boot problems
./testing/raspberry_pi/setup/rpi_setup.sh --check-boot

# Issue: GPIO access
sudo usermod -a -G gpio $USER
./testing/raspberry_pi/setup/gpio_setup.sh --fix-permissions

# Issue: Performance problems
./testing/raspberry_pi/setup/rpi_setup.sh --optimize
```

---

*Comprehensive testing framework for professional QNX automotive development*

## 👥 Contributors

- **Abdullah Abdelhakeem** - abdullah.abdelhakeem657@gmail.com
- **Sara Elsaied** - Sara.ahmed.elsaied@gmail.com 