# QNX Dashboard Simulator - Momentics IDE Guide

## 🎯 Running the Dashboard in QNX Momentics IDE

This guide shows you how to run the QNX Automotive Dashboard Simulator in QNX Momentics IDE.

## ✅ Current Status

- **Windows Demo**: ✅ Available and running
- **QNX Host Tools**: ✅ Installed and working
- **QNX Momentics IDE**: ✅ Available
- **Project Structure**: ✅ Complete
- **Source Code**: ✅ QNX-compatible

## 🚀 Method 1: Import and Run in IDE

### Step 1: Import Project
1. Open **QNX Momentics IDE**
2. Go to `File` → `Import...`
3. Select `General` → `Existing Projects into Workspace`
4. Browse to: `D:\QNX_Workspace\01_dashboard_simulator`
5. Check `Copy projects into workspace` (optional)
6. Click `Finish`

### Step 2: Configure Build
1. Right-click on the project in Project Explorer
2. Select `Properties`
3. Go to `C/C++ Build` → `Settings`
4. Under `Tool Settings`:
   - **Compiler**: Use QNX gcc (ntox86_64-gcc)
   - **Linker**: Add `-lm` for math library
   - **Architecture**: x86_64

### Step 3: Build Project
1. Right-click project → `Build Project`
2. Or press `Ctrl+B`
3. Check console for build output
4. Verify no errors

### Step 4: Run Configuration
1. Right-click project → `Run As` → `Run Configurations...`
2. Create new `QNX C/C++ Application`
3. **Main Tab**:
   - Project: `01_dashboard_simulator`
   - Application: `gui_dashboard` (or `console_dashboard`)
4. **Target Tab**:
   - Select target: `Local` (for x86_64) or configure remote QNX target
5. Click `Run`

## 🖥️ Method 2: Console Dashboard (Text-based)

For a simpler test, run the console version:

```bash
# In QNX Terminal or IDE Console
cd /path/to/project
./console_dashboard
```

## 🔧 Method 3: Debug Mode

1. Right-click project → `Debug As` → `Debug Configurations...`
2. Create new `QNX C/C++ Application`
3. Configure same as Run Configuration
4. Set breakpoints in source code
5. Click `Debug`

## 📊 Expected Output

When running successfully, you should see:

```
🚗 QNX AUTOMOTIVE DASHBOARD v1.0 🚗
====================================

┌─ PRIMARY INSTRUMENTS ─────────────────┐
│ SPEED (km/h): 45.2  [●●●●●○○○○○○○○○○○○○○○]
│ RPM        : 2150.0 [●●●●○○○○○○○○○○○○○○○○]
│ FUEL (%)   : 78.5   [●●●●●●●●●●●●●●●○○○○○]
│ TEMP (°C)  : 89.3   [●●●●●●●●○○○○○○○○○○○○]
└───────────────────────────────────────┘

┌─ VEHICLE STATUS ──────────────────────┐
│ GEAR: 3     FRAME: 1247     TIME: 14:32:15
└───────────────────────────────────────┘

┌─ WARNING SYSTEM ──────────────────────┐
│  ○ LOW FUEL      ○ HIGH TEMP
│  ○ HIGH RPM      ○ SPEED LIMIT
│  ✅ ALL SYSTEMS NORMAL
└───────────────────────────────────────┘
```

## 🎮 Interactive Controls

- **ESC**: Exit dashboard
- **SPACE**: Pause/Resume
- **R**: Reset counters
- **Q**: Quit

## 🐛 Troubleshooting

### Build Errors
- **Missing headers**: Ensure QNX includes are in path
- **Linker errors**: Add `-lm` flag for math functions
- **Architecture mismatch**: Verify x86_64 target

### Runtime Issues
- **Segmentation fault**: Check memory allocation
- **Display issues**: Verify terminal supports ANSI colors
- **Permission denied**: Check executable permissions

### QNX Target Issues
- **Target not found**: Configure QNX target in IDE
- **Connection failed**: Check network/serial connection
- **File not found**: Verify binary copied to target

## 🔄 Alternative: Windows Demo

If you encounter QNX-specific issues, run the Windows demo:

```bash
# Compile Windows version
gcc -o gui_dashboard_windows.exe windows_dashboard.c -lm

# Run demo
./gui_dashboard_windows.exe
```

## 📋 Project Structure

```
01_dashboard_simulator/
├── src/
│   ├── main.c              # Entry point
│   ├── gui_dashboard.c     # GUI implementation
│   ├── console_dashboard.c # Console version
│   ├── data_simulator.c    # Vehicle data simulation
│   └── gui_manager.c       # GUI management
├── include/
│   ├── dashboard.h         # Main header
│   ├── data_simulator.h    # Data structures
│   └── gui_manager.h       # GUI definitions
├── resources/
│   └── config/
│       └── dashboard.conf  # Configuration
└── Makefile               # Build configuration
```

## 🎯 Success Criteria

✅ **Project imports successfully**
✅ **Builds without errors**
✅ **Runs and displays dashboard**
✅ **Real-time data updates**
✅ **Interactive controls work**
✅ **Professional automotive GUI**

## 🚗 Next Steps

1. **Customize Dashboard**: Modify gauges, colors, layout
2. **Add Features**: GPS, navigation, media controls
3. **Hardware Integration**: Connect to real vehicle data
4. **Performance Tuning**: Optimize for real-time constraints
5. **Testing**: Comprehensive automotive testing scenarios

---

**🎉 You now have a fully functional QNX Automotive Dashboard running in Momentics IDE!** 