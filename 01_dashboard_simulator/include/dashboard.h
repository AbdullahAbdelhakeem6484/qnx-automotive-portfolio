/**
 * ===============================================================================
 * QNX Automotive Dashboard Simulator - Main Header File
 * ===============================================================================
 * File: dashboard.h
 * Description: Core definitions, data structures, and function prototypes
 * Version: 1.0.0
 * Copyright: 2025 QNX Automotive Projects Portfolio
 * ===============================================================================
 */

#ifndef DASHBOARD_H
#define DASHBOARD_H

/* ===============================================================================
 * System Includes
 * =============================================================================== */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <pthread.h>
#include <time.h>
#include <math.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <stdbool.h>
#include <stdint.h>

/* QNX Photon Includes - Disabled for this demo (using ANSI terminal graphics) */
// #include <Ph.h>  
// #include <Pt.h>
// #include <Ap.h>
// #include <photon/PxImage.h>
// #include <photon/PgArc.h>

/* For this demo, we use standard terminal interface instead of Photon */
typedef void* PtWidget_t;  /* Mock Photon widget type */
typedef struct { int x, y; } PhPoint_t;
typedef struct { int w, h; } PhDim_t;
typedef unsigned int PgColor_t;

/* ===============================================================================
 * Project Configuration
 * =============================================================================== */
#define DASHBOARD_VERSION_MAJOR    1
#define DASHBOARD_VERSION_MINOR    0
#define DASHBOARD_VERSION_PATCH    0
#define DASHBOARD_VERSION_STRING   "1.0.0"

#define PROJECT_NAME              "QNX Dashboard Simulator"
#define PROJECT_DESCRIPTION       "Automotive Dashboard Simulator for QNX"
#define PROJECT_AUTHOR            "QNX Automotive Projects Portfolio"

/* ===============================================================================
 * Build Configuration
 * =============================================================================== */
#ifdef DEBUG
    #define DEBUG_PRINT(fmt, ...) \
        printf("[DEBUG] %s:%d " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__)
    #define VERBOSE_LOGGING 1
#else
    #define DEBUG_PRINT(fmt, ...) do {} while(0)
    #define VERBOSE_LOGGING 0
#endif

#ifdef SAFETY_CRITICAL
    #define SAFETY_CHECK(condition, action) \
        do { if (!(condition)) { action; } } while(0)
#else  
    #define SAFETY_CHECK(condition, action) do {} while(0)
#endif

/* ===============================================================================
 * Application Constants
 * =============================================================================== */

/* Window and Display Settings */
#define WINDOW_WIDTH              1024
#define WINDOW_HEIGHT             768
#define WINDOW_TITLE              PROJECT_NAME " v" DASHBOARD_VERSION_STRING

/* Real-Time Performance Settings */
#define GUI_UPDATE_RATE_HZ        60      /* 60 FPS GUI updates */
#define DATA_UPDATE_RATE_HZ       100     /* 100 Hz data processing */
#define MONITOR_UPDATE_RATE_HZ    10      /* 10 Hz system monitoring */

#define GUI_UPDATE_INTERVAL_MS    (1000 / GUI_UPDATE_RATE_HZ)
#define DATA_UPDATE_INTERVAL_MS   (1000 / DATA_UPDATE_RATE_HZ)
#define MONITOR_UPDATE_INTERVAL_MS (1000 / MONITOR_UPDATE_RATE_HZ)

/* Thread Priorities (QNX Real-Time) */
#define GUI_THREAD_PRIORITY       63      /* Highest - Real-time GUI */
#define DATA_THREAD_PRIORITY      50      /* Medium - Data processing */
#define MONITOR_THREAD_PRIORITY   30      /* Low - System monitoring */

/* Vehicle Data Ranges and Limits */
#define MIN_SPEED                 0.0f
#define MAX_SPEED                 300.0f   /* km/h */
#define SAFETY_MAX_SPEED          250.0f   /* Safety limit */

#define MIN_RPM                   0.0f
#define MAX_RPM                   8000.0f
#define SAFETY_MAX_RPM            7500.0f   /* Redline warning */
#define IDLE_RPM                  800.0f

#define MIN_FUEL_LEVEL            0.0f
#define MAX_FUEL_LEVEL            100.0f   /* Percentage */
#define LOW_FUEL_WARNING          15.0f    /* Warning threshold */

#define MIN_TEMPERATURE           -40.0f   /* Celsius */
#define MAX_TEMPERATURE           120.0f
#define OVERHEAT_WARNING          95.0f    /* Warning threshold */

/* Performance and Safety Thresholds */
#define MAX_FRAME_TIME_MS         16.67f   /* 60 FPS target */
#define MAX_DATA_LATENCY_MS       10.0f    /* Data processing limit */
#define MAX_MEMORY_USAGE_MB       64       /* Memory limit */

/* GUI Configuration */
#define GAUGE_COUNT               4         /* Number of main gauges */
#define WARNING_LIGHT_COUNT       8         /* Number of warning lights */
#define MAX_THEME_NAME_LENGTH     32
#define MAX_CONFIG_VALUE_LENGTH   256

/* ===============================================================================
 * Enumeration Types
 * =============================================================================== */

/**
 * Application states for state machine management
 */
typedef enum {
    STATE_INITIALIZING = 0,
    STATE_RUNNING,
    STATE_PAUSED,
    STATE_ERROR,
    STATE_SHUTTING_DOWN,
    STATE_COUNT
} dashboard_state_t;

/**
 * Data simulation modes
 */
typedef enum {
    SIM_MODE_REALISTIC = 0,    /* Physics-based simulation */
    SIM_MODE_DEMO,             /* Predetermined sequences */
    SIM_MODE_MANUAL,           /* User-controlled input */
    SIM_MODE_REPLAY,           /* Recorded data playback */
    SIM_MODE_COUNT
} simulation_mode_t;

/**
 * GUI themes for different lighting conditions
 */
typedef enum {
    THEME_DAY = 0,
    THEME_NIGHT,
    THEME_AUTOMOTIVE,
    THEME_HIGH_CONTRAST,
    THEME_COUNT
} gui_theme_t;

/**
 * Warning levels for system alerts
 */
typedef enum {
    WARNING_NONE = 0,
    WARNING_LOW,
    WARNING_MEDIUM,
    WARNING_HIGH,
    WARNING_CRITICAL
} warning_level_t;

/**
 * Vehicle warning types
 */
typedef enum {
    WARNING_ENGINE_TEMP = 0,
    WARNING_OIL_PRESSURE,
    WARNING_BATTERY,
    WARNING_ABS,
    WARNING_AIRBAG,
    WARNING_TURN_LEFT,
    WARNING_TURN_RIGHT,
    WARNING_HIGH_BEAM,
    WARNING_TYPE_COUNT
} warning_type_t;

/* ===============================================================================
 * Core Data Structures
 * =============================================================================== */

/**
 * Vehicle data structure containing all sensor readings
 */
typedef struct {
    /* Motion and Engine */
    float speed_kmh;           /* Vehicle speed in km/h */
    float rpm;                 /* Engine RPM */
    float acceleration;        /* m/s² */
    
    /* Fluids and Temperature */
    float fuel_level;          /* Fuel level percentage (0-100) */
    float engine_temp;         /* Engine temperature in Celsius */
    float oil_pressure;        /* Oil pressure in PSI */
    
    /* Electrical */
    float battery_voltage;     /* Battery voltage */
    float alternator_current;  /* Alternator current */
    
    /* Transmission */
    int gear_position;         /* Current gear (-1=R, 0=N, 1-6=Drive) */
    
    /* Status Flags */
    bool ignition_on;
    bool engine_running;
    bool parking_brake;
    bool seatbelt_fastened;
    
    /* Timestamps */
    struct timespec timestamp;
    uint32_t data_age_ms;      /* Age of data in milliseconds */
    bool data_valid;           /* Data validity flag */
} vehicle_data_t;

/**
 * Gauge widget configuration and state
 */
typedef struct {
    /* Widget Information */
    PtWidget_t *widget;        /* Photon widget pointer */
    char label[32];            /* Gauge label */
    
    /* Value Range */
    float min_value;
    float max_value;
    float current_value;
    float target_value;        /* For smooth animations */
    
    /* Visual Properties */
    PhPoint_t position;        /* Widget position */
    PhDim_t size;             /* Widget size */
    PgColor_t color_normal;    /* Normal color */
    PgColor_t color_warning;   /* Warning color */
    PgColor_t color_critical;  /* Critical color */
    
    /* Behavior */
    float warning_threshold;
    float critical_threshold;
    float animation_speed;     /* Animation interpolation speed */
    bool visible;
    bool enabled;
} gauge_widget_t;

/**
 * Warning light configuration and state
 */
typedef struct {
    /* Widget Information */
    PtWidget_t *widget;
    warning_type_t type;
    char label[32];
    
    /* State */
    bool active;
    bool blinking;
    warning_level_t level;
    
    /* Visual Properties */
    PhPoint_t position;
    PhDim_t size;
    PgColor_t color_off;
    PgColor_t color_on;
    
    /* Animation */
    uint32_t blink_interval_ms;
    uint32_t last_blink_time;
    bool blink_state;
} warning_light_t;

/**
 * GUI manager state and configuration
 */
typedef struct {
    /* Main Window */
    PtWidget_t *main_window;
    PtWidget_t *canvas;
    PhDim_t window_size;
    
    /* Widgets */
    gauge_widget_t gauges[GAUGE_COUNT];
    warning_light_t warnings[WARNING_LIGHT_COUNT];
    
    /* Theme and Display */
    gui_theme_t current_theme;
    bool fullscreen;
    float brightness;
    
    /* Animation and Timing */
    struct timespec last_update;
    uint32_t frame_count;
    float fps;
    
    /* State */
    bool initialized;
    bool needs_redraw;
} gui_manager_t;

/**
 * Data simulator configuration and state
 */
typedef struct {
    /* Configuration */
    simulation_mode_t mode;
    float update_rate_hz;
    bool realistic_physics;
    
    /* Physics Model Parameters */
    float vehicle_mass;        /* kg */
    float drag_coefficient;
    float rolling_resistance;
    float engine_power;        /* kW */
    
    /* Current State */
    vehicle_data_t current_data;
    vehicle_data_t previous_data;
    
    /* Scenario Control */
    float scenario_time;       /* Current scenario time */
    float scenario_speed;      /* Playback speed multiplier */
    bool scenario_loop;        /* Loop scenario when complete */
    
    /* Statistics */
    uint64_t update_count;
    float average_update_time_ms;
    uint32_t error_count;
} data_simulator_t;

/**
 * Performance monitoring structure
 */
typedef struct {
    /* Timing Metrics */
    float gui_update_time_ms;
    float data_update_time_ms;
    float total_frame_time_ms;
    
    /* Resource Usage */
    uint32_t memory_usage_kb;
    float cpu_usage_percent;
    
    /* Performance Counters */
    uint64_t frames_rendered;
    uint64_t data_updates;
    uint32_t missed_deadlines;
    
    /* Quality Metrics */
    float average_fps;
    float frame_time_variance;
    bool performance_warning;
} performance_metrics_t;

/**
 * Main application context
 */
typedef struct {
    /* Application State */
    dashboard_state_t state;
    bool running;
    bool shutdown_requested;
    
    /* Components */
    gui_manager_t gui;
    data_simulator_t simulator;
    performance_metrics_t performance;
    
    /* Threading */
    pthread_t gui_thread;
    pthread_t data_thread;
    pthread_t monitor_thread;
    
    pthread_mutex_t data_mutex;
    pthread_cond_t data_condition;
    
    /* Configuration */
    char config_file[256];
    bool debug_mode;
    bool automotive_mode;
    
    /* Error Handling */
    int last_error;
    char error_message[256];
} dashboard_context_t;

/* ===============================================================================
 * Global Variables (External Declarations)
 * =============================================================================== */
extern dashboard_context_t g_dashboard_context;
extern volatile sig_atomic_t g_shutdown_requested;

/* ===============================================================================
 * Core Function Prototypes
 * =============================================================================== */

/* Application Lifecycle */
int dashboard_initialize(int argc, char *argv[]);
int dashboard_run(void);
void dashboard_shutdown(void);
void dashboard_cleanup(void);

/* State Management */
void dashboard_set_state(dashboard_state_t new_state);
dashboard_state_t dashboard_get_state(void);
const char *dashboard_state_to_string(dashboard_state_t state);

/* Signal Handling */
void signal_handler(int signal);
int setup_signal_handlers(void);

/* Error Handling */
void dashboard_set_error(int error_code, const char *message);
int dashboard_get_last_error(void);
const char *dashboard_get_error_message(void);

/* Threading Support */
int create_application_threads(void);
void *gui_thread_main(void *arg);
void *data_thread_main(void *arg);
void *monitor_thread_main(void *arg);
void join_application_threads(void);

/* Utility Functions */
uint64_t get_time_microseconds(void);
float time_diff_ms(struct timespec *start, struct timespec *end);
void sleep_ms(uint32_t milliseconds);
float clamp_float(float value, float min, float max);

/* ===============================================================================
 * Inline Utility Functions
 * =============================================================================== */

/**
 * Get current time in microseconds since epoch
 */
static inline uint64_t get_timestamp_us(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)ts.tv_sec * 1000000ULL + (uint64_t)ts.tv_nsec / 1000ULL;
}

/**
 * Calculate time difference in milliseconds
 */
static inline float timespec_diff_ms(struct timespec *start, struct timespec *end) {
    return ((float)(end->tv_sec - start->tv_sec) * 1000.0f) + 
           ((float)(end->tv_nsec - start->tv_nsec) / 1000000.0f);
}

/**
 * Clamp value within specified range
 */
static inline float clamp(float value, float min_val, float max_val) {
    if (value < min_val) return min_val;
    if (value > max_val) return max_val;
    return value;
}

/**
 * Linear interpolation between two values
 */
static inline float lerp(float a, float b, float t) {
    return a + t * (b - a);
}

/**
 * Check if value is within valid range
 */
static inline bool is_value_valid(float value, float min_val, float max_val) {
    return (value >= min_val && value <= max_val && !isnan(value) && !isinf(value));
}

/* ===============================================================================
 * Conditional Compilation Support
 * =============================================================================== */

#ifdef AUTOMOTIVE_BUILD
    /* Automotive-specific definitions */
    #define ENABLE_SAFETY_MONITORING 1
    #define ENABLE_DIAGNOSTIC_LOGGING 1
    #define STRICT_TIMING_VALIDATION 1
#endif

#ifdef PERFORMANCE_BUILD
    /* Performance optimization definitions */
    #define ENABLE_FAST_MATH 1
    #define DISABLE_DEBUG_CHECKS 1
    #define OPTIMIZE_MEMORY_USAGE 1
#endif

#ifdef HIGH_PERFORMANCE
    /* High-performance specific settings */
    #define GUI_UPDATE_RATE_HZ    120    /* Higher refresh rate */
    #define DATA_UPDATE_RATE_HZ   200    /* Higher data rate */
#endif

#endif /* DASHBOARD_H */ 