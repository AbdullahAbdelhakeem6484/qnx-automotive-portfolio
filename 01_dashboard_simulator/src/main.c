/**
 * ===============================================================================
 * QNX Automotive Dashboard Simulator - Main Application
 * ===============================================================================
 */

#include <signal.h>
#include <sys/sched.h>
#include "../include/dashboard.h"
#include "../include/gui_manager.h"
#include "../include/data_simulator.h"
#include "../include/config.h"

/* Global Variables */
dashboard_context_t g_dashboard_context = {0};
volatile sig_atomic_t g_shutdown_requested = 0;

/* Forward Declarations */
static void print_startup_banner(void);
static int process_command_line_args(int argc, char *argv[]);
static void setup_thread_priorities(void);

/**
 * Main application entry point
 */
int main(int argc, char *argv[]) {
    printf("===============================================================================\n");
    printf("  QNX Automotive Dashboard Simulator v%s\n", DASHBOARD_VERSION);
    printf("  Professional Automotive HMI Development Platform\n");
    printf("===============================================================================\n\n");
    
    /* Initialize signal handlers */
    if (setup_signal_handlers() != 0) {
        fprintf(stderr, "Error: Failed to setup signal handlers\n");
        return 1;
    }
    
    /* Initialize the dashboard system */
    int result = dashboard_initialize(argc, argv);
    if (result != 0) {
        fprintf(stderr, "Error: Dashboard initialization failed (%d)\n", result);
        return result;
    }
    
    /* Run the main application loop */
    result = dashboard_run();
    
    /* Cleanup and shutdown */
    dashboard_cleanup();
    
    printf("Dashboard simulator shutdown complete.\n");
    return result;
}

/**
 * Initialize the dashboard system
 */
int dashboard_initialize(int argc, char *argv[]) {
    dashboard_context_t *ctx = &g_dashboard_context;
    
    printf("Initializing QNX Dashboard Simulator...\n");
    
    /* Set initial state */
    dashboard_set_state(STATE_INITIALIZING);
    
    /* Initialize threading primitives */
    if (pthread_mutex_init(&ctx->data_mutex, NULL) != 0) {
        fprintf(stderr, "Error: Failed to initialize data mutex\n");
        return -1;
    }
    
    /* Initialize GUI manager */
    if (gui_manager_initialize(&ctx->gui) != 0) {
        fprintf(stderr, "Error: Failed to initialize GUI manager\n");
        return -1;
    }
    
    /* Initialize data simulator */
    if (data_simulator_initialize(&ctx->simulator) != 0) {
        fprintf(stderr, "Error: Failed to initialize data simulator\n");
        return -1;
    }
    
    /* Create application threads */
    if (create_application_threads() != 0) {
        fprintf(stderr, "Error: Failed to create application threads\n");
        return -1;
    }
    
    dashboard_set_state(STATE_RUNNING);
    printf("Dashboard initialization complete.\n");
    
    return 0;
}

/**
 * Main application run loop
 */
int dashboard_run(void) {
    dashboard_context_t *ctx = &g_dashboard_context;
    
    ctx->running = true;
    printf("Starting dashboard main loop...\n");
    
    /* Main event loop */
    while (ctx->running && !g_shutdown_requested) {
        gui_manager_handle_events(&ctx->gui);
        
        if (dashboard_get_state() == STATE_SHUTTING_DOWN) {
            break;
        }
        
        usleep(1000); /* 1ms */
    }
    
    printf("Main loop terminated. Initiating shutdown...\n");
    
    /* Signal threads to stop */
    ctx->running = false;
    
    /* Wait for threads to complete */
    join_application_threads();
    
    return 0;
}

/**
 * GUI Thread Main Function
 */
void *gui_thread_main(void *arg) {
    dashboard_context_t *ctx = (dashboard_context_t *)arg;
    struct timespec sleep_time = {0, GUI_UPDATE_INTERVAL_MS * 1000000};
    
    printf("GUI thread started\n");
    
    while (ctx->running && !g_shutdown_requested) {
        /* Lock data for reading */
        pthread_mutex_lock(&ctx->data_mutex);
        vehicle_data_t current_data = ctx->simulator.current_data;
        pthread_mutex_unlock(&ctx->data_mutex);
        
        /* Update GUI */
        gui_manager_update(&ctx->gui, &current_data);
        gui_manager_render(&ctx->gui);
        
        ctx->performance.frames_rendered++;
        
        nanosleep(&sleep_time, NULL);
    }
    
    printf("GUI thread terminated\n");
    return NULL;
}

/**
 * Data Thread Main Function
 */
void *data_thread_main(void *arg) {
    dashboard_context_t *ctx = (dashboard_context_t *)arg;
    struct timespec sleep_time = {0, DATA_UPDATE_INTERVAL_MS * 1000000};
    
    printf("Data thread started\n");
    
    while (ctx->running && !g_shutdown_requested) {
        float delta_time = DATA_UPDATE_INTERVAL_MS / 1000.0f;
        
        pthread_mutex_lock(&ctx->data_mutex);
        data_simulator_update(&ctx->simulator, delta_time);
        pthread_mutex_unlock(&ctx->data_mutex);
        
        ctx->performance.data_updates++;
        
        nanosleep(&sleep_time, NULL);
    }
    
    printf("Data thread terminated\n");
    return NULL;
}

/**
 * Monitor Thread Main Function
 */
void *monitor_thread_main(void *arg) {
    dashboard_context_t *ctx = (dashboard_context_t *)arg;
    struct timespec sleep_time = {0, MONITOR_UPDATE_INTERVAL_MS * 1000000};
    
    printf("Monitor thread started\n");
    
    while (ctx->running && !g_shutdown_requested) {
        /* Basic performance monitoring */
        static uint64_t last_frames = 0;
        static time_t last_time = 0;
        
        time_t current_time = time(NULL);
        if (current_time > last_time) {
            ctx->performance.average_fps = (float)(ctx->performance.frames_rendered - last_frames);
            last_frames = ctx->performance.frames_rendered;
            last_time = current_time;
        }
        
        nanosleep(&sleep_time, NULL);
    }
    
    printf("Monitor thread terminated\n");
    return NULL;
}

/**
 * Create application threads
 */
int create_application_threads(void) {
    dashboard_context_t *ctx = &g_dashboard_context;
    
    if (pthread_create(&ctx->gui_thread, NULL, gui_thread_main, ctx) != 0) {
        perror("Failed to create GUI thread");
        return -1;
    }
    
    if (pthread_create(&ctx->data_thread, NULL, data_thread_main, ctx) != 0) {
        perror("Failed to create data thread");
        return -1;
    }
    
    if (pthread_create(&ctx->monitor_thread, NULL, monitor_thread_main, ctx) != 0) {
        perror("Failed to create monitor thread");
        return -1;
    }
    
    return 0;
}

/**
 * Wait for all threads to complete
 */
void join_application_threads(void) {
    dashboard_context_t *ctx = &g_dashboard_context;
    
    printf("Waiting for threads to terminate...\n");
    
    pthread_join(ctx->gui_thread, NULL);
    pthread_join(ctx->data_thread, NULL);
    pthread_join(ctx->monitor_thread, NULL);
    
    printf("All threads terminated successfully\n");
}

/**
 * Signal handler for graceful shutdown
 */
void signal_handler(int signal) {
    printf("\nReceived signal %d. Initiating graceful shutdown...\n", signal);
    dashboard_shutdown();
}

/**
 * Setup signal handlers
 */
int setup_signal_handlers(void) {
    struct sigaction sa;
    
    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    
    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("sigaction SIGINT");
        return -1;
    }
    
    if (sigaction(SIGTERM, &sa, NULL) == -1) {
        perror("sigaction SIGTERM");
        return -1;
    }
    
    return 0;
}

/**
 * Shutdown the dashboard system
 */
void dashboard_shutdown(void) {
    printf("Shutting down dashboard system...\n");
    dashboard_set_state(STATE_SHUTTING_DOWN);
    g_shutdown_requested = 1;
    g_dashboard_context.running = false;
}

/**
 * Cleanup resources
 */
void dashboard_cleanup(void) {
    dashboard_context_t *ctx = &g_dashboard_context;
    
    printf("Starting cleanup...\n");
    
    gui_manager_cleanup(&ctx->gui);
    data_simulator_cleanup(&ctx->simulator);
    pthread_mutex_destroy(&ctx->data_mutex);
    
    printf("Cleanup complete.\n");
}

/**
 * State management functions
 */
void dashboard_set_state(dashboard_state_t new_state) {
    g_dashboard_context.state = new_state;
}

dashboard_state_t dashboard_get_state(void) {
    return g_dashboard_context.state;
}

const char *dashboard_state_to_string(dashboard_state_t state) {
    switch (state) {
        case STATE_INITIALIZING: return "INITIALIZING";
        case STATE_RUNNING: return "RUNNING";
        case STATE_PAUSED: return "PAUSED";
        case STATE_ERROR: return "ERROR";
        case STATE_SHUTTING_DOWN: return "SHUTTING_DOWN";
        default: return "UNKNOWN";
    }
}

/**
 * Print startup banner
 */
static void print_startup_banner(void) {
    printf("===============================================================================\n");
    printf("  QNX Automotive Dashboard Simulator v%s\n", DASHBOARD_VERSION);
    printf("  Professional Automotive HMI Development Platform\n");
    printf("===============================================================================\n\n");
}

/**
 * Process command line arguments
 */
static int process_command_line_args(int argc, char *argv[]) {
    /* Simple argument processing */
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--debug") == 0) {
            g_dashboard_context.debug_mode = true;
            printf("Debug mode enabled\n");
        } else if (strcmp(argv[i], "--config") == 0 && i + 1 < argc) {
            strncpy(g_dashboard_context.config_file, argv[++i], sizeof(g_dashboard_context.config_file) - 1);
            printf("Using config file: %s\n", g_dashboard_context.config_file);
        } else if (strcmp(argv[i], "--help") == 0) {
            printf("Usage: %s [options]\n", argv[0]);
            printf("Options:\n");
            printf("  --debug           Enable debug mode\n");
            printf("  --config <file>   Specify configuration file\n");
            printf("  --help            Show this help message\n");
            return 1;
        }
    }
    return 0;
}

/**
 * Setup thread priorities for real-time performance
 */
static void setup_thread_priorities(void) {
    /* Note: In a real QNX system, you would need appropriate privileges
     * to set real-time thread priorities */
    printf("Setting up real-time thread priorities...\n");
    printf("  GUI Thread Priority: %d\n", GUI_THREAD_PRIORITY);
    printf("  Data Thread Priority: %d\n", DATA_THREAD_PRIORITY);
    printf("  Monitor Thread Priority: %d\n", MONITOR_THREAD_PRIORITY);
} 