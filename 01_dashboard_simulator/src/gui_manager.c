/**
 * QNX Dashboard Simulator - GUI Manager Implementation
 */

#include "../include/gui_manager.h"

/* Color definitions */
static const PgColor_t COLORS[] = {
    0x000000, /* Black */
    0xFFFFFF, /* White */
    0x0080FF, /* Blue */
    0xFF4000, /* Red */
    0x00C000, /* Green */
    0xFFFF00, /* Yellow */
    0xFF0000, /* Critical Red */
    0x808080  /* Gray */
};

/**
 * Initialize GUI manager
 */
int gui_manager_initialize(gui_manager_t *gui) {
    if (!gui) return -1;
    
    printf("Initializing GUI manager...\n");
    
    memset(gui, 0, sizeof(gui_manager_t));
    
    /* Initialize Photon */
    if (PtInit(NULL) == -1) {
        fprintf(stderr, "Error: Failed to initialize Photon\n");
        return -1;
    }
    
    gui->current_theme = THEME_AUTOMOTIVE;
    gui->fullscreen = false;
    gui->needs_redraw = true;
    
    if (gui_manager_create_window(gui) != 0) {
        fprintf(stderr, "Error: Failed to create main window\n");
        return -1;
    }
    
    if (gui_manager_create_widgets(gui) != 0) {
        fprintf(stderr, "Error: Failed to create widgets\n");
        return -1;
    }
    
    gui->initialized = true;
    printf("GUI manager initialized successfully\n");
    
    return 0;
}

/**
 * Create main window
 */
int gui_manager_create_window(gui_manager_t *gui) {
    PtArg_t args[5];
    int n = 0;
    
    PtSetArg(&args[n++], Pt_ARG_WINDOW_TITLE, WINDOW_TITLE, 0);
    PtSetArg(&args[n++], Pt_ARG_DIM, &(PhDim_t){WINDOW_WIDTH, WINDOW_HEIGHT}, 0);
    
    gui->main_window = PtCreateWidget(PtWindow, Pt_NO_PARENT, n, args);
    if (!gui->main_window) {
        fprintf(stderr, "Error: Failed to create main window\n");
        return -1;
    }
    
    /* Create canvas */
    n = 0;
    PhDim_t canvas_size = {WINDOW_WIDTH, WINDOW_HEIGHT};
    PtSetArg(&args[n++], Pt_ARG_DIM, &canvas_size, 0);
    
    gui->canvas = PtCreateWidget(PtRaw, gui->main_window, n, args);
    if (!gui->canvas) {
        fprintf(stderr, "Error: Failed to create canvas\n");
        return -1;
    }
    
    PtAddCallback(gui->main_window, Pt_CB_WINDOW_OPENING, 
                  gui_window_close_callback, gui);
    
    PtRealizeWidget(gui->main_window);
    
    return 0;
}

/**
 * Create dashboard widgets
 */
int gui_manager_create_widgets(gui_manager_t *gui) {
    printf("Creating dashboard widgets...\n");
    
    /* Initialize gauges */
    gui_create_gauge_widget(&gui->gauges[0], "SPEED", 0.0f, MAX_SPEED,
                           (PhPoint_t){100, 100}, (PhDim_t){200, 200});
    
    gui_create_gauge_widget(&gui->gauges[1], "RPM", 0.0f, MAX_RPM,
                           (PhPoint_t){350, 100}, (PhDim_t){200, 200});
    
    gui_create_gauge_widget(&gui->gauges[2], "FUEL", 0.0f, MAX_FUEL_LEVEL,
                           (PhPoint_t){100, 350}, (PhDim_t){150, 150});
    
    gui_create_gauge_widget(&gui->gauges[3], "TEMP", 0.0f, MAX_TEMP,
                           (PhPoint_t){300, 350}, (PhDim_t){150, 150});
    
    /* Initialize warning lights */
    const char *labels[] = {"ENGINE", "OIL", "BATTERY", "ABS"};
    for (int i = 0; i < 4; i++) {
        PhPoint_t pos = {600 + (i % 2) * 100, 100 + (i / 2) * 60};
        gui_create_warning_light(&gui->warnings[i], labels[i],
                                pos, (PhDim_t){80, 40});
    }
    
    printf("Dashboard widgets created successfully\n");
    return 0;
}

/**
 * Create gauge widget
 */
int gui_create_gauge_widget(gauge_widget_t *gauge, const char *label,
                           float min_val, float max_val,
                           PhPoint_t pos, PhDim_t size) {
    if (!gauge || !label) return -1;
    
    strncpy(gauge->label, label, sizeof(gauge->label) - 1);
    gauge->min_value = min_val;
    gauge->max_value = max_val;
    gauge->current_value = 0.0f;
    gauge->target_value = 0.0f;
    gauge->position = pos;
    gauge->size = size;
    gauge->visible = true;
    gauge->enabled = true;
    
    return 0;
}

/**
 * Create warning light widget
 */
int gui_create_warning_light(warning_light_t *warning, const char *label,
                            PhPoint_t pos, PhDim_t size) {
    if (!warning || !label) return -1;
    
    strncpy(warning->label, label, sizeof(warning->label) - 1);
    warning->active = false;
    warning->blinking = false;
    warning->position = pos;
    warning->size = size;
    
    return 0;
}

/**
 * Update GUI with vehicle data
 */
void gui_manager_update(gui_manager_t *gui, const vehicle_data_t *data) {
    if (!gui || !data || !gui->initialized) return;
    
    gui_update_gauge_value(&gui->gauges[0], data->speed_kmh);
    gui_update_gauge_value(&gui->gauges[1], data->rpm);
    gui_update_gauge_value(&gui->gauges[2], data->fuel_level);
    gui_update_gauge_value(&gui->gauges[3], data->engine_temp);
    
    /* Update warnings */
    gui_set_warning_state(&gui->warnings[0], data->engine_temp > 95.0f, false);
    gui_set_warning_state(&gui->warnings[1], data->oil_pressure < 10.0f, false);
    gui_set_warning_state(&gui->warnings[2], data->battery_voltage < 11.5f, false);
    
    /* Animate gauges */
    float delta_time = 1.0f / 60.0f;
    for (int i = 0; i < GAUGE_COUNT; i++) {
        gui_animate_gauge(&gui->gauges[i], delta_time);
    }
    
    gui->needs_redraw = true;
}

/**
 * Update gauge value
 */
void gui_update_gauge_value(gauge_widget_t *gauge, float value) {
    if (!gauge) return;
    gauge->target_value = clamp(value, gauge->min_value, gauge->max_value);
}

/**
 * Animate gauge
 */
void gui_animate_gauge(gauge_widget_t *gauge, float delta_time) {
    if (!gauge) return;
    
    float diff = gauge->target_value - gauge->current_value;
    if (fabs(diff) > 0.1f) {
        gauge->current_value += diff * 5.0f * delta_time;
    } else {
        gauge->current_value = gauge->target_value;
    }
}

/**
 * Set warning state
 */
void gui_set_warning_state(warning_light_t *warning, bool active, bool blinking) {
    if (!warning) return;
    warning->active = active;
    warning->blinking = blinking;
}

/**
 * Render GUI
 */
void gui_manager_render(gui_manager_t *gui) {
    if (!gui || !gui->initialized || !gui->needs_redraw) return;
    
    /* Simple console output for now - in real implementation would draw to Photon */
    static int render_count = 0;
    if (render_count++ % 60 == 0) { /* Print every second at 60 FPS */
        printf("GUI Render - Speed: %.1f, RPM: %.1f, Fuel: %.1f%%, Temp: %.1f°C\n",
               gui->gauges[0].current_value,
               gui->gauges[1].current_value,
               gui->gauges[2].current_value,
               gui->gauges[3].current_value);
    }
    
    gui->needs_redraw = false;
    gui->frame_count++;
}

/**
 * Handle events
 */
void gui_manager_handle_events(gui_manager_t *gui) {
    if (!gui || !gui->initialized) return;
    
    /* Process Photon events */
    PtProcessEvent();
}

/**
 * Window close callback
 */
int gui_window_close_callback(PtWidget_t *widget, ApInfo_t *apinfo, 
                             PtCallbackInfo_t *cbinfo) {
    printf("Window close requested - shutting down\n");
    dashboard_shutdown();
    return Pt_CONTINUE;
}

/**
 * Cleanup GUI manager
 */
void gui_manager_cleanup(gui_manager_t *gui) {
    if (!gui) return;
    
    printf("Cleaning up GUI manager...\n");
    
    if (gui->main_window) {
        PtDestroyWidget(gui->main_window);
        gui->main_window = NULL;
    }
    
    PtExit();
    gui->initialized = false;
    
    printf("GUI manager cleanup complete\n");
} 