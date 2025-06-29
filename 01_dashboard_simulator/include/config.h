/**
 * ===============================================================================
 * QNX Automotive Dashboard Simulator - Configuration Header
 * ===============================================================================
 * File: config.h
 * Description: Configuration management system for dashboard simulator
 * Version: 1.0.0
 * Copyright: 2025 QNX Automotive Projects Portfolio
 * ===============================================================================
 */

#ifndef CONFIG_H
#define CONFIG_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

/* ===============================================================================
 * Configuration Constants
 * =============================================================================== */

/* Configuration File Settings */
#define DEFAULT_CONFIG_FILE       "resources/config/dashboard.conf"
#define MAX_CONFIG_LINE_LENGTH    512
#define MAX_CONFIG_KEY_LENGTH     64
#define MAX_CONFIG_VALUE_LENGTH   256
#define MAX_CONFIG_SECTION_LENGTH 64

/* Configuration Sections */
#define CONFIG_SECTION_DISPLAY    "display"
#define CONFIG_SECTION_PERFORMANCE "performance"
#define CONFIG_SECTION_SIMULATION "simulation"
#define CONFIG_SECTION_SAFETY     "safety"
#define CONFIG_SECTION_THEME      "theme"
#define CONFIG_SECTION_AUDIO      "audio"

/* Default Configuration Values */
#define DEFAULT_WINDOW_WIDTH      1024
#define DEFAULT_WINDOW_HEIGHT     768
#define DEFAULT_FULLSCREEN        false
#define DEFAULT_GUI_FPS           60
#define DEFAULT_DATA_RATE_HZ      100
#define DEFAULT_THEME             "automotive"
#define DEFAULT_SIM_MODE          "realistic"

/* ===============================================================================
 * Configuration Enumerations
 * =============================================================================== */

/**
 * Configuration value types
 */
typedef enum {
    CONFIG_TYPE_STRING = 0,
    CONFIG_TYPE_INTEGER,
    CONFIG_TYPE_FLOAT,
    CONFIG_TYPE_BOOLEAN,
    CONFIG_TYPE_COUNT
} config_value_type_t;

/**
 * Configuration loading result
 */
typedef enum {
    CONFIG_SUCCESS = 0,
    CONFIG_ERROR_FILE_NOT_FOUND,
    CONFIG_ERROR_PARSE_ERROR,
    CONFIG_ERROR_INVALID_VALUE,
    CONFIG_ERROR_MEMORY_ALLOCATION,
    CONFIG_ERROR_COUNT
} config_result_t;

/* ===============================================================================
 * Configuration Data Structures
 * =============================================================================== */

/**
 * Configuration key-value pair
 */
typedef struct {
    char key[MAX_CONFIG_KEY_LENGTH];
    char value[MAX_CONFIG_VALUE_LENGTH];
    config_value_type_t type;
    bool required;
    bool loaded;
} config_item_t;

/**
 * Display configuration settings
 */
typedef struct {
    int window_width;
    int window_height;
    bool fullscreen;
    bool vsync_enabled;
    int refresh_rate;
    float brightness;
    char theme_name[MAX_CONFIG_VALUE_LENGTH];
    bool hardware_acceleration;
} display_config_t;

/**
 * Performance configuration settings
 */
typedef struct {
    int gui_update_rate_hz;
    int data_update_rate_hz;
    int monitor_update_rate_hz;
    bool enable_profiling;
    bool enable_statistics;
    int max_memory_usage_mb;
    float max_cpu_usage_percent;
} performance_config_t;

/**
 * Simulation configuration settings
 */
typedef struct {
    char simulation_mode[32];
    bool realistic_physics;
    float physics_timestep;
    bool enable_scenarios;
    char scenario_file[MAX_CONFIG_VALUE_LENGTH];
    bool loop_scenarios;
    float simulation_speed;
} simulation_config_t;

/**
 * Safety configuration settings
 */
typedef struct {
    bool enable_safety_monitoring;
    float max_speed_kmh;
    float max_rpm;
    float overheat_threshold;
    float low_fuel_threshold;
    bool enable_warnings;
    bool strict_validation;
    int watchdog_timeout_ms;
} safety_config_t;

/**
 * Theme configuration settings
 */
typedef struct {
    char current_theme[32];
    bool auto_theme_switching;
    int day_theme_start_hour;
    int night_theme_start_hour;
    float gauge_scale_factor;
    bool enable_animations;
    int animation_duration_ms;
} theme_config_t;

/**
 * Audio configuration settings
 */
typedef struct {
    bool enable_audio;
    float master_volume;
    bool enable_warning_sounds;
    bool enable_turn_signals;
    char audio_device[MAX_CONFIG_VALUE_LENGTH];
    int sample_rate;
} audio_config_t;

/**
 * Main configuration structure
 */
typedef struct {
    char config_file_path[MAX_CONFIG_VALUE_LENGTH];
    bool loaded;
    bool modified;
    
    /* Configuration sections */
    display_config_t display;
    performance_config_t performance;
    simulation_config_t simulation;
    safety_config_t safety;
    theme_config_t theme;
    audio_config_t audio;
    
    /* Metadata */
    char version[32];
    char last_modified[64];
    int config_format_version;
} dashboard_config_t;

/* ===============================================================================
 * Configuration Function Prototypes
 * =============================================================================== */

/* Configuration Management */
config_result_t config_initialize(dashboard_config_t *config);
config_result_t config_load(dashboard_config_t *config, const char *filename);
config_result_t config_save(const dashboard_config_t *config, const char *filename);
config_result_t config_reset_to_defaults(dashboard_config_t *config);
void config_cleanup(dashboard_config_t *config);

/* Configuration Parsing */
config_result_t config_parse_file(dashboard_config_t *config, FILE *file);
config_result_t config_parse_line(dashboard_config_t *config, const char *line, 
                                  const char *current_section);
config_result_t config_set_value(dashboard_config_t *config, const char *section,
                                 const char *key, const char *value);

/* Configuration Validation */
bool config_validate(const dashboard_config_t *config);
bool config_validate_display(const display_config_t *display);
bool config_validate_performance(const performance_config_t *performance);
bool config_validate_simulation(const simulation_config_t *simulation);
bool config_validate_safety(const safety_config_t *safety);

/* Configuration Getters */
const char *config_get_string(const dashboard_config_t *config, const char *section,
                             const char *key, const char *default_value);
int config_get_integer(const dashboard_config_t *config, const char *section,
                      const char *key, int default_value);
float config_get_float(const dashboard_config_t *config, const char *section,
                      const char *key, float default_value);
bool config_get_boolean(const dashboard_config_t *config, const char *section,
                       const char *key, bool default_value);

/* Configuration Setters */
config_result_t config_set_string(dashboard_config_t *config, const char *section,
                                  const char *key, const char *value);
config_result_t config_set_integer(dashboard_config_t *config, const char *section,
                                   const char *key, int value);
config_result_t config_set_float(dashboard_config_t *config, const char *section,
                                 const char *key, float value);
config_result_t config_set_boolean(dashboard_config_t *config, const char *section,
                                   const char *key, bool value);

/* Utility Functions */
const char *config_result_to_string(config_result_t result);
void config_print_summary(const dashboard_config_t *config);
bool config_file_exists(const char *filename);
config_result_t config_create_default_file(const char *filename);

/* Configuration Hot-Reloading */
bool config_file_changed(const dashboard_config_t *config);
config_result_t config_reload_if_changed(dashboard_config_t *config);
void config_watch_for_changes(dashboard_config_t *config);

/* ===============================================================================
 * Configuration Macros
 * =============================================================================== */

/* Safe string copying for configuration values */
#define CONFIG_SAFE_STRCPY(dest, src, max_len) \
    do { \
        strncpy(dest, src, max_len - 1); \
        dest[max_len - 1] = '\0'; \
    } while(0)

/* Configuration validation macro */
#define CONFIG_VALIDATE_RANGE(value, min, max, name) \
    do { \
        if (value < min || value > max) { \
            printf("Configuration error: %s value %f out of range [%f, %f]\n", \
                   name, (float)value, (float)min, (float)max); \
            return false; \
        } \
    } while(0)

/* Configuration logging macro */
#ifdef DEBUG
    #define CONFIG_DEBUG(fmt, ...) \
        printf("[CONFIG] " fmt "\n", ##__VA_ARGS__)
#else
    #define CONFIG_DEBUG(fmt, ...) do {} while(0)
#endif

/* ===============================================================================
 * Default Configuration Template
 * =============================================================================== */

/* This macro defines the default configuration file content */
#define DEFAULT_CONFIG_TEMPLATE \
"# ===============================================================================\n" \
"# QNX Automotive Dashboard Simulator - Configuration File\n" \
"# ===============================================================================\n" \
"# Version: 1.0.0\n" \
"# Auto-generated configuration file\n" \
"# Modify values as needed for your system\n" \
"# ===============================================================================\n" \
"\n" \
"[display]\n" \
"window_width = 1024\n" \
"window_height = 768\n" \
"fullscreen = false\n" \
"vsync_enabled = true\n" \
"refresh_rate = 60\n" \
"brightness = 1.0\n" \
"theme_name = automotive\n" \
"hardware_acceleration = true\n" \
"\n" \
"[performance]\n" \
"gui_update_rate_hz = 60\n" \
"data_update_rate_hz = 100\n" \
"monitor_update_rate_hz = 10\n" \
"enable_profiling = false\n" \
"enable_statistics = true\n" \
"max_memory_usage_mb = 64\n" \
"max_cpu_usage_percent = 80.0\n" \
"\n" \
"[simulation]\n" \
"simulation_mode = realistic\n" \
"realistic_physics = true\n" \
"physics_timestep = 0.016\n" \
"enable_scenarios = false\n" \
"scenario_file = scenarios/default.json\n" \
"loop_scenarios = true\n" \
"simulation_speed = 1.0\n" \
"\n" \
"[safety]\n" \
"enable_safety_monitoring = true\n" \
"max_speed_kmh = 250.0\n" \
"max_rpm = 7500.0\n" \
"overheat_threshold = 95.0\n" \
"low_fuel_threshold = 15.0\n" \
"enable_warnings = true\n" \
"strict_validation = true\n" \
"watchdog_timeout_ms = 1000\n" \
"\n" \
"[theme]\n" \
"current_theme = automotive\n" \
"auto_theme_switching = false\n" \
"day_theme_start_hour = 6\n" \
"night_theme_start_hour = 18\n" \
"gauge_scale_factor = 1.0\n" \
"enable_animations = true\n" \
"animation_duration_ms = 250\n" \
"\n" \
"[audio]\n" \
"enable_audio = false\n" \
"master_volume = 0.7\n" \
"enable_warning_sounds = true\n" \
"enable_turn_signals = true\n" \
"audio_device = default\n" \
"sample_rate = 44100\n"

/* ===============================================================================
 * External Configuration Instance
 * =============================================================================== */
extern dashboard_config_t g_dashboard_config;

#endif /* CONFIG_H */ 