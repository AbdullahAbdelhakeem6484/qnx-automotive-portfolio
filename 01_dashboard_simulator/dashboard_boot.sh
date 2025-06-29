 
#!/bin/sh 
echo "QNX Dashboard Simulator Boot" 
echo "Starting Dashboard Applications..." 
./console_dashboard & 
sleep 2 
./gui_dashboard 
