#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>
#include <math.h>
#include <string.h>
#include <termios.h>
#include <fcntl.h>

// Simple GUI Dashboard Simulator for QNX
// Uses ANSI escape codes for colorful GUI-like display

typedef struct {
    float speed;        // km/h
    float rpm;         // revolutions per minute
    float fuel_level;  // percentage
    float temperature; // celsius
    int gear;          // current gear
    int warnings;      // warning flags
    struct timeval timestamp;
} vehicle_data_t;

typedef struct {
    vehicle_data_t data;
    pthread_mutex_t mutex;
    int running;
    int frame_count;
    double fps;
} dashboard_context_t;

// Global context
static dashboard_context_t g_ctx = {0};

// ANSI Color codes for GUI
#define RESET       "\033[0m"
#define BLACK       "\033[30m"
#define RED         "\033[31m"
#define GREEN       "\033[32m"
#define YELLOW      "\033[33m"
#define BLUE        "\033[34m"
#define MAGENTA     "\033[35m"
#define CYAN        "\033[36m"
#define WHITE       "\033[37m"
#define BOLD        "\033[1m"
#define BRIGHT      "\033[1;37m"
#define BG_BLACK    "\033[40m"
#define BG_RED      "\033[41m"
#define BG_GREEN    "\033[42m"
#define BG_BLUE     "\033[44m"
#define BG_CYAN     "\033[46m"

// Simulate realistic vehicle data with physics
void simulate_vehicle_data(vehicle_data_t* data) {
    static float time_counter = 0.0;
    static float velocity = 0.0;
    static float acceleration = 0.0;
    
    time_counter += 0.1;
    
    // Simulate realistic driving with acceleration/deceleration
    acceleration = 2.0 * sin(time_counter * 0.05) + 0.5 * sin(time_counter * 0.1);
    velocity += acceleration * 0.1;
    if (velocity < 0) velocity = 0;
    if (velocity > 200) velocity = 200;
    
    data->speed = velocity;
    data->rpm = 800.0 + (data->speed * 45.0) + 500.0 * sin(time_counter * 0.2);
    data->fuel_level = 85.0 - (time_counter * 0.008);
    data->temperature = 85.0 + 8.0 * sin(time_counter * 0.03) + (data->speed * 0.1);
    data->gear = (int)(data->speed / 25.0) + 1;
    if (data->gear > 6) data->gear = 6;
    
    // Simulate warnings based on realistic conditions
    data->warnings = 0;
    if (data->fuel_level < 15.0) data->warnings |= 0x01; // Low fuel
    if (data->temperature > 100.0) data->warnings |= 0x02; // High temp
    if (data->rpm > 6500.0) data->warnings |= 0x04; // High RPM
    if (data->speed > 180.0) data->warnings |= 0x08; // Speed warning
    
    gettimeofday(&data->timestamp, NULL);
}

// Create a circular gauge using ASCII
void draw_circular_gauge(float value, float max_val, const char* color, const char* label) {
    int segments = 20;
    int filled = (int)((value / max_val) * segments);
    
    printf("%s%s%s: %6.1f ", color, BOLD, label, value);
    printf("[");
    for (int i = 0; i < segments; i++) {
        if (i < filled) {
            if (i < segments * 0.6) printf("%s●%s", GREEN, color);
            else if (i < segments * 0.8) printf("%s●%s", YELLOW, color);
            else printf("%s●%s", RED, color);
        } else {
            printf("○");
        }
    }
    printf("]%s\n", RESET);
}

// Draw warning indicator
void draw_warning(int active, const char* symbol, const char* text, const char* color) {
    if (active) {
        printf("  %s%s%s %s %s", BG_RED, WHITE, BOLD, symbol, text);
        printf("%s", RESET);
    } else {
        printf("  %s○ %s%s", color, text, RESET);
    }
}

// Display GUI dashboard
void display_gui_dashboard(const vehicle_data_t* data, double fps) {
    // Clear screen and position cursor
    printf("\033[2J\033[H");
    
    // Title bar with colors
    printf("%s%s%s", BG_BLUE, WHITE, BOLD);
    printf("╔════════════════════════════════════════════════════════════════════════════╗\n");
    printf("║                    🚗 QNX AUTOMOTIVE DASHBOARD v1.0 🚗                    ║\n");
    printf("║                        Real-time GUI Simulation                           ║\n");
    printf("╚════════════════════════════════════════════════════════════════════════════╝");
    printf("%s\n\n", RESET);
    
    // Main dashboard display
    printf("%s%s", CYAN, BOLD);
    printf("┌─ PRIMARY INSTRUMENTS ──────────────────────────────────────────────────────┐\n");
    printf("%s", RESET);
    
    // Speed gauge (large and prominent)
    printf("│ ");
    draw_circular_gauge(data->speed, 200.0, CYAN, "SPEED (km/h)");
    
    // RPM gauge  
    printf("│ ");
    draw_circular_gauge(data->rpm, 8000.0, MAGENTA, "RPM        ");
    
    // Fuel gauge
    printf("│ ");
    draw_circular_gauge(data->fuel_level, 100.0, BLUE, "FUEL (%)   ");
    
    // Temperature gauge
    printf("│ ");
    draw_circular_gauge(data->temperature, 120.0, RED, "TEMP (°C)  ");
    
    printf("└────────────────────────────────────────────────────────────────────────────┘\n\n");
    
    // Secondary information
    printf("%s%s", GREEN, BOLD);
    printf("┌─ VEHICLE STATUS ───────────────────────────────────────────────────────────┐\n");
    printf("%s", RESET);
    
    printf("│ %sGEAR:%s %s%d%s     ", YELLOW, RESET, BOLD, data->gear, RESET);
    printf("%sFPS:%s %s%.1f%s     ", CYAN, RESET, BOLD, fps, RESET);
    
    struct tm* timeinfo;
    time_t t = data->timestamp.tv_sec;
    timeinfo = localtime(&t);
    printf("%sTIME:%s %s%02d:%02d:%02d%s\n", GREEN, RESET, BOLD, 
           timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, RESET);
    
    printf("└────────────────────────────────────────────────────────────────────────────┘\n\n");
    
    // Warning lights panel
    printf("%s%s", RED, BOLD);
    printf("┌─ WARNING SYSTEM ───────────────────────────────────────────────────────────┐\n");
    printf("%s", RESET);
    printf("│");
    
    draw_warning(data->warnings & 0x01, "⛽", "LOW FUEL    ", YELLOW);
    draw_warning(data->warnings & 0x02, "🌡️", "HIGH TEMP   ", RED);
    printf("\n│");
    draw_warning(data->warnings & 0x04, "⚡", "HIGH RPM    ", MAGENTA);
    draw_warning(data->warnings & 0x08, "🚨", "SPEED LIMIT ", RED);
    
    if (data->warnings == 0) {
        printf("  %s%s✅ ALL SYSTEMS NORMAL%s", GREEN, BOLD, RESET);
    }
    
    printf("\n└────────────────────────────────────────────────────────────────────────────┘\n\n");
    
    // Performance information
    printf("%s%s", WHITE, BOLD);
    printf("📊 PERFORMANCE: %s%.1f FPS%s | ", GREEN, fps, WHITE);
    printf("🧵 THREADS: %sActive%s | ", GREEN, WHITE);
    printf("⚡ REAL-TIME: %sQNX Neutrino%s | ", CYAN, WHITE);
    printf("🎯 TARGET: %s60 FPS%s\n", YELLOW, RESET);
    
    printf("\n%s%s[ESC] Exit | [SPACE] Pause | [R] Reset | QNX Dashboard Simulator%s\n", 
           WHITE, BOLD, RESET);
}

// Data simulation thread
void* data_thread(void* arg) {
    dashboard_context_t* ctx = (dashboard_context_t*)arg;
    
    printf("🧵 Data thread started (Priority: 50, Rate: 10Hz)\n");
    sleep(1);
    
    while (ctx->running) {
        pthread_mutex_lock(&ctx->mutex);
        simulate_vehicle_data(&ctx->data);
        pthread_mutex_unlock(&ctx->mutex);
        
        usleep(100000); // 100ms = 10Hz
    }
    
    printf("🧵 Data thread stopped\n");
    return NULL;
}

// GUI rendering thread
void* gui_thread(void* arg) {
    dashboard_context_t* ctx = (dashboard_context_t*)arg;
    vehicle_data_t local_data;
    struct timeval start_time, current_time;
    
    printf("🧵 GUI thread started (Priority: 63, Rate: 30Hz)\n");
    sleep(1);
    
    gettimeofday(&start_time, NULL);
    
    while (ctx->running) {
        // Copy data safely
        pthread_mutex_lock(&ctx->mutex);
        local_data = ctx->data;
        pthread_mutex_unlock(&ctx->mutex);
        
        // Calculate FPS
        gettimeofday(&current_time, NULL);
        double elapsed = (current_time.tv_sec - start_time.tv_sec) + 
                        (current_time.tv_usec - start_time.tv_usec) / 1000000.0;
        ctx->frame_count++;
        ctx->fps = ctx->frame_count / elapsed;
        
        // Display GUI dashboard
        display_gui_dashboard(&local_data, ctx->fps);
        
        usleep(33333); // 33ms ≈ 30Hz (smooth but not too fast for visibility)
    }
    
    printf("🧵 GUI thread stopped\n");
    return NULL;
}

// Signal handler for clean shutdown
#include <signal.h>
void signal_handler(int sig) {
    printf("\n🛑 Received signal %d, shutting down GUI dashboard...\n", sig);
    g_ctx.running = 0;
}

// Check for keyboard input (non-blocking)
int kbhit() {
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if(ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}

int main() {
    pthread_t data_tid, gui_tid;
    
    printf("\033[2J\033[H"); // Clear screen
    printf("%s%s", CYAN, BOLD);
    printf("🚀 QNX AUTOMOTIVE DASHBOARD SIMULATOR v1.0.0\n");
    printf("🎨 GUI Mode with Real-time Graphics\n");
    printf("🧵 Multi-threaded Real-time Architecture\n");
    printf("⚡ QNX Neutrino Real-time OS\n");
    printf("═══════════════════════════════════════════\n");
    printf("%s", RESET);
    
    printf("🔧 Initializing dashboard systems...\n");
    
    // Initialize context
    g_ctx.running = 1;
    g_ctx.frame_count = 0;
    g_ctx.fps = 0.0;
    pthread_mutex_init(&g_ctx.mutex, NULL);
    
    // Set up signal handlers
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);
    
    // Create threads with QNX real-time priorities
    pthread_attr_t attr;
    struct sched_param param;
    
    printf("🧵 Creating data simulation thread...\n");
    // Data thread (medium priority)
    pthread_attr_init(&attr);
    pthread_attr_setschedpolicy(&attr, SCHED_RR);
    param.sched_priority = 50;
    pthread_attr_setschedparam(&attr, &param);
    
    if (pthread_create(&data_tid, &attr, data_thread, &g_ctx) != 0) {
        perror("Failed to create data thread");
        return 1;
    }
    
    printf("🎨 Creating GUI rendering thread...\n");
    // GUI thread (high priority)
    pthread_attr_init(&attr);
    pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
    param.sched_priority = 63;
    pthread_attr_setschedparam(&attr, &param);
    
    if (pthread_create(&gui_tid, &attr, gui_thread, &g_ctx) != 0) {
        perror("Failed to create GUI thread");
        return 1;
    }
    
    printf("✅ All threads created successfully\n");
    printf("🎬 Starting GUI dashboard simulation...\n\n");
    sleep(2);
    
    // Main thread handles input (simple)
    while (g_ctx.running) {
        if (kbhit()) {
            int ch = getchar();
            if (ch == 27) { // ESC key
                printf("\n🛑 ESC pressed, shutting down...\n");
                g_ctx.running = 0;
                break;
            } else if (ch == ' ') { // Space key
                printf("\n⏸️  Paused - press any key to continue...\n");
                getchar();
            } else if (ch == 'r' || ch == 'R') { // Reset
                printf("\n🔄 Resetting dashboard...\n");
                g_ctx.frame_count = 0;
            }
        }
        usleep(50000); // 50ms
    }
    
    // Wait for threads to complete
    pthread_join(data_tid, NULL);
    pthread_join(gui_tid, NULL);
    
    // Cleanup
    pthread_mutex_destroy(&g_ctx.mutex);
    
    printf("\n%s%s", GREEN, BOLD);
    printf("✅ GUI Dashboard Simulator Stopped Successfully\n");
    printf("📋 QNX Real-time Performance Summary:\n");
    printf("   • Multi-threaded GUI architecture ✓\n");
    printf("   • Real-time scheduling (SCHED_FIFO/RR) ✓\n");
    printf("   • Thread synchronization with mutexes ✓\n");
    printf("   • Colorful GUI with ANSI graphics ✓\n");
    printf("   • Automotive data simulation ✓\n");
    printf("   • Performance monitoring (%.1f FPS) ✓\n", g_ctx.fps);
    printf("   • Signal handling and clean shutdown ✓\n");
    printf("%s\n", RESET);
    
    return 0;
} 