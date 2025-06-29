# 🚀 QNX Momentics IDE 8.0 - Dashboard Simulator Setup

## 📋 **IDE Project Configuration**

### **1. Import Project**
1. **File** → **Import** → **General** → **Existing Projects into Workspace**
2. **Browse** → Select `01_dashboard_simulator` folder
3. **Check** "Copy projects into workspace" (optional)
4. **Finish**

### **2. Build Configuration**
1. **Right-click** project → **Properties**
2. **C/C++ Build** → **Settings**
3. **Tool Settings** → **QCC Compiler**
4. **Command**: `qcc`
5. **All options**: `-Vgcc_ntox86_64 -Wall -g -O2`

### **3. Target Configuration**
1. **Right-click** project → **Properties**  
2. **QNX C/C++** → **Build Variants**
3. **Add** → **x86_64**
4. **Architecture**: `x86_64`
5. **Variant**: `gcc_ntox86_64`

## 🎯 **Run Configurations**

### **Console Dashboard Run Config**
1. **Run** → **Run Configurations**
2. **C/C++ QNX Application** → **New**
3. **Name**: `Console Dashboard`
4. **Project**: `01_dashboard_simulator`
5. **C/C++ Application**: `console_dashboard`
6. **Connection**: Local QNX Target
7. **Arguments**: (none)
8. **Apply** → **Run**

### **GUI Dashboard Run Config**
1. **Run** → **Run Configurations** 
2. **C/C++ QNX Application** → **New**
3. **Name**: `GUI Dashboard`
4. **Project**: `01_dashboard_simulator`
5. **C/C++ Application**: `gui_dashboard`
6. **Connection**: Local QNX Target
7. **Arguments**: (none)
8. **Apply** → **Run**

## 🖥️ **QNX Desktop Environment (QDE) Setup**

### **1. Start QNX Desktop**
```cmd
# From QNX installation directory
C:\QNX\qnx800\host\win64\x86_64\usr\bin\qde.exe
```

### **2. Create QNX Target**
1. **Target Navigator** → **Right-click** → **New Target**
2. **Target Name**: `QNX Dashboard Target`
3. **Host Name**: `localhost`
4. **Target Type**: `QNX Target`
5. **Connection**: `IP`
6. **Finish**

### **3. Deploy to Target**
1. **Right-click** project → **Deploy As** → **QNX C/C++ Application**
2. **Target**: Select your QNX target
3. **Deploy**

## 🔧 **Build Steps in IDE**

### **Method 1: IDE Build**
1. **Right-click** project → **Build Project**
2. **Console** shows build progress
3. **Problems** tab shows any errors

### **Method 2: Makefile Build**
1. **Right-click** project → **Build Targets** → **Build...**
2. **Create** → **Name**: `all`
3. **Build command**: `make all`
4. **Build**

## 🚀 **Running Dashboard in IDE**

### **Local Host Execution**
```bash
# Terminal in IDE
cd /path/to/project
./console_dashboard &
./gui_dashboard
```

### **QNX Target Execution**
1. **Target Navigator** → **Connect** to target
2. **Right-click** executable → **Run As** → **QNX C/C++ Application**
3. **Select** target connection
4. **Run**

## 🎥 **Demo Recording in IDE**

### **Screen Recording Setup**
1. **Window** → **Show View** → **Console**
2. **Window** → **Show View** → **Target Navigator**
3. **Window** → **Show View** → **Project Explorer**
4. **Resize** windows for optimal recording

### **Demo Flow**
1. **Show** project structure in Project Explorer
2. **Build** project (Right-click → Build)
3. **Run** console dashboard
4. **Run** GUI dashboard  
5. **Show** console output with real-time data
6. **Demonstrate** multi-threading performance

## 🐛 **Troubleshooting**

### **Build Errors**
```bash
# Check QNX environment
echo $QNX_HOST
echo $QNX_TARGET

# Verify compiler
qcc --version
```

### **Runtime Errors**
```bash
# Check target connection
ping target_ip

# Verify executables
file console_dashboard gui_dashboard
```

### **IDE Issues**
1. **Refresh** project (F5)
2. **Clean/Rebuild** project
3. **Check** QNX perspective is active
4. **Verify** target is connected

## ✅ **Success Criteria**

- ✅ Project builds without errors
- ✅ Console dashboard runs and shows real-time data
- ✅ GUI dashboard displays colorful interface
- ✅ Multiple threads executing properly
- ✅ Performance metrics visible in console
- ✅ Target connection established
- ✅ Deploy to QNX target successful

---

**🎬 Ready to demonstrate QNX automotive dashboard in professional IDE environment!** 