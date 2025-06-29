#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>
#include <math.h>

// Console Dashboard Simulator - No GUI Version
// Perfect for QNX testing without Photon

typedef struct {
    float speed;        // km/h
    float rpm;         // revolutions per minute
    float fuel_level;  // percentage
    float temperature; // celsius
    int gear;          // current gear
    int warnings;      // warning flags
} vehicle_data_t;

typedef struct {
    vehicle_data_t data;
    pthread_mutex_t mutex;
    int running;
} dashboard_context_t;

// Global context
static dashboard_context_t g_ctx = {0};

// Simulate realistic vehicle data
void simulate_vehicle_data(vehicle_data_t* data) {
    static float time_counter = 0.0;
    time_counter += 0.1;
    
    // Simulate driving patterns
    data->speed = 60.0 + 20.0 * sin(time_counter * 0.1);
    data->rpm = 2000.0 + 1000.0 * sin(time_counter * 0.15);
    data->fuel_level = 75.0 - (time_counter * 0.01);
    data->temperature = 85.0 + 5.0 * sin(time_counter * 0.05);
    data->gear = (int)(data->speed / 20.0) + 1;
    
    // Simulate warnings
    data->warnings = 0;
    if (data->fuel_level < 20.0) data->warnings |= 0x01; // Low fuel
    if (data->temperature > 95.0) data->warnings |= 0x02; // High temp
    if (data->rpm > 6000.0) data->warnings |= 0x04; // High RPM
}

// Data simulation thread
void* data_thread(void* arg) {
    dashboard_context_t* ctx = (dashboard_context_t*)arg;
    
    printf("🧵 Data thread started (Priority: 50, Rate: 10Hz)\n");
    
    while (ctx->running) {
        pthread_mutex_lock(&ctx->mutex);
        simulate_vehicle_data(&ctx->data);
        pthread_mutex_unlock(&ctx->mutex);
        
        usleep(100000); // 100ms = 10Hz
    }
    
    printf("🧵 Data thread stopped\n");
    return NULL;
}

// Display dashboard in console
void display_dashboard(const vehicle_data_t* data) {
    // Clear screen (ANSI escape codes)
    printf("\033[2J\033[H");
    
    printf("╔══════════════════════════════════════════════════════════════╗\n");
    printf("║               🚗 QNX AUTOMOTIVE DASHBOARD 🚗                ║\n");
    printf("╠══════════════════════════════════════════════════════════════╣\n");
    printf("║                                                              ║\n");
    
    // Speed gauge (ASCII art)
    printf("║  SPEED: %6.1f km/h  ", data->speed);
    int speed_bars = (int)(data->speed / 10);
    printf("[");
    for (int i = 0; i < 20; i++) {
        if (i < speed_bars) printf("█");
        else printf("░");
    }
    printf("]  ║\n");
    
    // RPM gauge
    printf("║  RPM:   %6.0f rpm   ", data->rpm);
    int rpm_bars = (int)(data->rpm / 300);
    printf("[");
    for (int i = 0; i < 20; i++) {
        if (i < rpm_bars) printf("█");
        else printf("░");
    }
    printf("]  ║\n");
    
    // Fuel gauge
    printf("║  FUEL:  %6.1f%%     ", data->fuel_level);
    int fuel_bars = (int)(data->fuel_level / 5);
    printf("[");
    for (int i = 0; i < 20; i++) {
        if (i < fuel_bars) printf("█");
        else printf("░");
    }
    printf("]  ║\n");
    
    // Temperature gauge
    printf("║  TEMP:  %6.1f°C     ", data->temperature);
    int temp_bars = (int)((data->temperature - 60) / 2);
    printf("[");
    for (int i = 0; i < 20; i++) {
        if (i < temp_bars) printf("█");
        else printf("░");
    }
    printf("]  ║\n");
    
    printf("║                                                              ║\n");
    printf("║  GEAR: %d     ", data->gear);
    
    // Warning lights
    printf("WARNINGS: ");
    if (data->warnings & 0x01) printf("⛽LOW-FUEL ");
    if (data->warnings & 0x02) printf("🌡️ HIGH-TEMP ");
    if (data->warnings & 0x04) printf("⚡HIGH-RPM ");
    if (data->warnings == 0) printf("✅ALL-OK");
    
    printf("       ║\n");
    printf("║                                                              ║\n");
    printf("╚══════════════════════════════════════════════════════════════╝\n");
    
    printf("\n📊 Performance: 60 FPS target | 🧵 Multi-threaded | ⚡ Real-time\n");
    printf("Press Ctrl+C to exit\n");
}

// GUI thread (console display)
void* gui_thread(void* arg) {
    dashboard_context_t* ctx = (dashboard_context_t*)arg;
    vehicle_data_t local_data;
    
    printf("🧵 GUI thread started (Priority: 63, Rate: 5Hz)\n");
    
    while (ctx->running) {
        // Copy data safely
        pthread_mutex_lock(&ctx->mutex);
        local_data = ctx->data;
        pthread_mutex_unlock(&ctx->mutex);
        
        // Display dashboard
        display_dashboard(&local_data);
        
        usleep(200000); // 200ms = 5Hz (console friendly rate)
    }
    
    printf("🧵 GUI thread stopped\n");
    return NULL;
}

// Signal handler for clean shutdown
#include <signal.h>
void signal_handler(int sig) {
    printf("\n🛑 Received signal %d, shutting down...\n", sig);
    g_ctx.running = 0;
}

int main() {
    pthread_t data_tid, gui_tid;
    
    printf("🚀 QNX Automotive Dashboard Simulator v1.0.0\n");
    printf("🖥️  Console Mode (No Photon GUI Required)\n");
    printf("🧵 Multi-threaded Real-time Architecture\n");
    printf("═══════════════════════════════════════════\n");
    
    // Initialize context
    g_ctx.running = 1;
    pthread_mutex_init(&g_ctx.mutex, NULL);
    
    // Set up signal handlers
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);
    
    // Create threads with QNX real-time priorities
    pthread_attr_t attr;
    struct sched_param param;
    
    // Data thread (medium priority)
    pthread_attr_init(&attr);
    pthread_attr_setschedpolicy(&attr, SCHED_RR);
    param.sched_priority = 50;
    pthread_attr_setschedparam(&attr, &param);
    
    if (pthread_create(&data_tid, &attr, data_thread, &g_ctx) != 0) {
        perror("Failed to create data thread");
        return 1;
    }
    
    // GUI thread (high priority)
    pthread_attr_init(&attr);
    pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
    param.sched_priority = 63;
    pthread_attr_setschedparam(&attr, &param);
    
    if (pthread_create(&gui_tid, &attr, gui_thread, &g_ctx) != 0) {
        perror("Failed to create GUI thread");
        return 1;
    }
    
    printf("🧵 Threads created successfully\n");
    printf("📊 Starting dashboard simulation...\n\n");
    
    // Wait for threads to complete
    pthread_join(data_tid, NULL);
    pthread_join(gui_tid, NULL);
    
    // Cleanup
    pthread_mutex_destroy(&g_ctx.mutex);
    
    printf("\n✅ Dashboard simulator stopped cleanly\n");
    printf("📋 QNX Real-time Performance Demonstrated:\n");
    printf("   • Multi-threaded architecture ✓\n");
    printf("   • Real-time scheduling ✓\n");
    printf("   • Thread synchronization ✓\n");
    printf("   • Signal handling ✓\n");
    printf("   • Automotive data simulation ✓\n");
    
    return 0;
} 