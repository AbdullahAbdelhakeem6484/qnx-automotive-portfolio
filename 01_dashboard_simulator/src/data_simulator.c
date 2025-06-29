/**
 * QNX Dashboard Simulator - Data Simulator Implementation
 */

#include "../include/data_simulator.h"

/* Global simulation state */
static float g_simulation_time = 0.0f;
static float g_acceleration = 0.0f;
static float g_fuel_consumption_rate = 0.05f; /* L/100km base rate */

/**
 * Initialize data simulator
 */
int data_simulator_initialize(data_simulator_t *sim) {
    if (!sim) return -1;
    
    printf("Initializing data simulator...\n");
    
    memset(sim, 0, sizeof(data_simulator_t));
    
    /* Set default configuration */
    sim->mode = SIM_MODE_REALISTIC;
    sim->update_rate_hz = DATA_UPDATE_RATE_HZ;
    sim->realistic_physics = true;
    
    /* Initialize vehicle data with safe defaults */
    vehicle_data_t *data = &sim->current_data;
    data->speed_kmh = 0.0f;
    data->rpm = 800.0f; /* Idle RPM */
    data->fuel_level = 75.0f;
    data->engine_temp = 25.0f; /* Ambient temperature */
    data->oil_pressure = 45.0f;
    data->battery_voltage = 12.6f;
    data->gear_position = 0; /* Neutral */
    data->ignition_on = true;
    data->engine_running = true;
    data->parking_brake = false;
    data->seatbelt_fastened = true;
    data->data_valid = true;
    
    /* Set timestamp */
    clock_gettime(CLOCK_MONOTONIC, &data->timestamp);
    
    /* Copy to previous data */
    sim->previous_data = sim->current_data;
    
    /* Reset statistics */
    sim->update_count = 0;
    sim->error_count = 0;
    
    printf("Data simulator initialized successfully\n");
    return 0;
}

/**
 * Update data simulator
 */
void data_simulator_update(data_simulator_t *sim, float delta_time) {
    if (!sim) return;
    
    /* Update simulation time */
    g_simulation_time += delta_time;
    
    /* Store previous data */
    sim->previous_data = sim->current_data;
    
    /* Generate new data based on mode */
    switch (sim->mode) {
        case SIM_MODE_REALISTIC:
            data_generate_realistic(sim, delta_time);
            break;
        case SIM_MODE_DEMO:
            data_generate_demo(sim, delta_time);
            break;
        case SIM_MODE_MANUAL:
            /* Manual mode - data set externally */
            break;
        case SIM_MODE_REPLAY:
            data_generate_replay(sim, delta_time);
            break;
        default:
            data_generate_realistic(sim, delta_time);
            break;
    }
    
    /* Validate and clamp data */
    data_clamp_values(&sim->current_data);
    data_set_timestamp(&sim->current_data);
    
    /* Update statistics */
    sim->update_count++;
}

/**
 * Generate realistic vehicle data
 */
void data_generate_realistic(data_simulator_t *sim, float delta_time) {
    vehicle_data_t *data = &sim->current_data;
    
    /* Simple driving pattern simulation */
    static float target_speed = 0.0f;
    static float speed_change_timer = 0.0f;
    
    /* Change target speed periodically */
    speed_change_timer += delta_time;
    if (speed_change_timer > 5.0f) { /* Change every 5 seconds */
        target_speed = (float)(rand() % 120 + 20); /* 20-140 km/h */
        speed_change_timer = 0.0f;
    }
    
    /* Smooth acceleration/deceleration */
    float speed_diff = target_speed - data->speed_kmh;
    if (fabs(speed_diff) > 1.0f) {
        float max_accel = speed_diff > 0 ? REALISTIC_ACCELERATION : REALISTIC_DECELERATION;
        g_acceleration = clamp(speed_diff * 0.5f, REALISTIC_DECELERATION, REALISTIC_ACCELERATION);
    } else {
        g_acceleration = 0.0f;
    }
    
    /* Update speed */
    data->speed_kmh = physics_calculate_speed(data->speed_kmh, g_acceleration, delta_time);
    data->speed_kmh = clamp(data->speed_kmh, 0.0f, REALISTIC_MAX_SPEED);
    
    /* Calculate RPM based on speed and gear */
    data->gear_position = (int)(data->speed_kmh / 25.0f) + 1; /* Simple gear calculation */
    data->gear_position = clamp(data->gear_position, 1, 6);
    
    data->rpm = physics_calculate_rpm(data->speed_kmh, data->gear_position, 1.0f);
    
    /* Update fuel consumption */
    float fuel_consumed = physics_calculate_fuel_consumption(data->speed_kmh, data->rpm, delta_time);
    data->fuel_level -= fuel_consumed;
    data->fuel_level = clamp(data->fuel_level, 0.0f, 100.0f);
    
    /* Update engine temperature */
    float ambient_temp = DEFAULT_AMBIENT_TEMP;
    data->engine_temp = physics_calculate_engine_temp(data->engine_temp, data->rpm, 
                                                     ambient_temp, delta_time);
    
    /* Update oil pressure (simplified) */
    data->oil_pressure = 20.0f + (data->rpm / 8000.0f) * 60.0f; /* 20-80 PSI */
    
    /* Update battery voltage (simplified) */
    data->battery_voltage = 12.0f + (data->engine_running ? 2.4f : 0.6f);
    if (data->engine_temp > 100.0f) {
        data->battery_voltage -= 0.2f; /* Voltage drop when overheating */
    }
    
    /* Add some noise for realism */
    data->speed_kmh = data_add_noise(data->speed_kmh, 0.5f);
    data->rpm = data_add_noise(data->rpm, 50.0f);
    data->engine_temp = data_add_noise(data->engine_temp, 1.0f);
    data->oil_pressure = data_add_noise(data->oil_pressure, 2.0f);
}

/**
 * Generate demo data sequence
 */
void data_generate_demo(data_simulator_t *sim, float delta_time) {
    vehicle_data_t *data = &sim->current_data;
    
    /* Predefined demo sequence */
    float demo_cycle = fmod(g_simulation_time, 30.0f); /* 30 second cycle */
    
    if (demo_cycle < 10.0f) {
        /* Acceleration phase */
        float t = demo_cycle / 10.0f;
        data->speed_kmh = t * 100.0f; /* 0-100 km/h */
        data->rpm = 800.0f + t * 5000.0f; /* 800-5800 RPM */
    } else if (demo_cycle < 20.0f) {
        /* Cruise phase */
        data->speed_kmh = 100.0f;
        data->rpm = 3000.0f;
    } else {
        /* Deceleration phase */
        float t = (30.0f - demo_cycle) / 10.0f;
        data->speed_kmh = t * 100.0f;
        data->rpm = 800.0f + t * 2200.0f;
    }
    
    /* Update other parameters */
    data->fuel_level = 50.0f + 25.0f * sin(g_simulation_time * 0.1f);
    data->engine_temp = 70.0f + 20.0f * sin(g_simulation_time * 0.05f);
    data->oil_pressure = 40.0f + 20.0f * (data->rpm / 8000.0f);
    data->battery_voltage = 12.6f + 1.4f * sin(g_simulation_time * 0.2f);
    
    data->gear_position = (int)(data->speed_kmh / 20.0f) + 1;
    data->gear_position = clamp(data->gear_position, 1, 6);
}

/**
 * Generate replay data (placeholder)
 */
void data_generate_replay(data_simulator_t *sim, float delta_time) {
    /* For now, just use realistic mode */
    data_generate_realistic(sim, delta_time);
}

/**
 * Physics calculation functions
 */
float physics_calculate_speed(float current_speed, float acceleration, float delta_time) {
    float new_speed = current_speed + (acceleration * delta_time * 3.6f); /* m/s² to km/h */
    return clamp(new_speed, 0.0f, MAX_SPEED);
}

float physics_calculate_rpm(float speed, int gear, float engine_load) {
    if (gear <= 0) return 800.0f; /* Idle */
    
    /* Simple gear ratio calculation */
    float gear_ratios[] = {0, 3.5f, 2.1f, 1.4f, 1.0f, 0.8f, 0.65f};
    float final_drive = 3.9f;
    float wheel_circumference = 2.0f; /* meters */
    
    float gear_ratio = gear_ratios[clamp(gear, 1, 6)];
    float rpm = (speed / 3.6f) * 60.0f / wheel_circumference * final_drive * gear_ratio;
    
    return clamp(rpm, 800.0f, MAX_RPM);
}

float physics_calculate_fuel_consumption(float speed, float rpm, float delta_time) {
    /* Simple fuel consumption model */
    float base_consumption = g_fuel_consumption_rate;
    float speed_factor = 1.0f + (speed / 100.0f) * 0.5f; /* Higher speed = more fuel */
    float rpm_factor = 1.0f + (rpm / 8000.0f) * 0.3f; /* Higher RPM = more fuel */
    
    float consumption_per_hour = base_consumption * speed_factor * rpm_factor;
    return (consumption_per_hour * delta_time / 3600.0f) * 0.1f; /* Convert to percentage */
}

float physics_calculate_engine_temp(float current_temp, float rpm, float ambient_temp, float delta_time) {
    float target_temp = ambient_temp + 50.0f + (rpm / 8000.0f) * 40.0f; /* 75-115°C range */
    float temp_change_rate = 10.0f; /* °C per second */
    
    float temp_diff = target_temp - current_temp;
    float temp_change = temp_diff * temp_change_rate * delta_time;
    
    return current_temp + temp_change;
}

/**
 * Data validation and utility functions
 */
bool data_validate_vehicle_data(const vehicle_data_t *data) {
    if (!data) return false;
    
    /* Check for reasonable ranges */
    if (data->speed_kmh < 0.0f || data->speed_kmh > MAX_SPEED) return false;
    if (data->rpm < 0.0f || data->rpm > MAX_RPM) return false;
    if (data->fuel_level < 0.0f || data->fuel_level > 100.0f) return false;
    if (data->engine_temp < -40.0f || data->engine_temp > 150.0f) return false;
    
    return true;
}

void data_clamp_values(vehicle_data_t *data) {
    if (!data) return;
    
    data->speed_kmh = clamp(data->speed_kmh, 0.0f, MAX_SPEED);
    data->rpm = clamp(data->rpm, 0.0f, MAX_RPM);
    data->fuel_level = clamp(data->fuel_level, 0.0f, MAX_FUEL_LEVEL);
    data->engine_temp = clamp(data->engine_temp, -40.0f, MAX_TEMP);
    data->oil_pressure = clamp(data->oil_pressure, 0.0f, 100.0f);
    data->battery_voltage = clamp(data->battery_voltage, 8.0f, 16.0f);
    data->gear_position = clamp(data->gear_position, -1, 6);
}

void data_set_timestamp(vehicle_data_t *data) {
    if (!data) return;
    
    clock_gettime(CLOCK_MONOTONIC, &data->timestamp);
    data->data_age_ms = 0; /* Fresh data */
    data->data_valid = true;
}

/**
 * Utility functions
 */
float data_smooth_transition(float current, float target, float rate, float delta_time) {
    float diff = target - current;
    float max_change = rate * delta_time;
    
    if (fabs(diff) <= max_change) {
        return target;
    } else {
        return current + (diff > 0 ? max_change : -max_change);
    }
}

float data_add_noise(float value, float noise_level) {
    float noise = ((float)rand() / RAND_MAX - 0.5f) * 2.0f * noise_level;
    return value + noise;
}

bool data_should_trigger_warning(const vehicle_data_t *data) {
    if (!data) return false;
    
    /* Check warning conditions */
    if (data->engine_temp > OVERHEAT_WARNING) return true;
    if (data->fuel_level < LOW_FUEL_WARNING) return true;
    if (data->oil_pressure < 15.0f) return true;
    if (data->battery_voltage < 11.0f) return true;
    
    return false;
}

/**
 * Simulation mode functions
 */
void data_simulator_set_mode(data_simulator_t *sim, simulation_mode_t mode) {
    if (!sim) return;
    
    sim->mode = mode;
    printf("Data simulator mode changed to: %d\n", mode);
}

simulation_mode_t data_simulator_get_mode(const data_simulator_t *sim) {
    return sim ? sim->mode : SIM_MODE_REALISTIC;
}

/**
 * Statistics functions
 */
void data_update_statistics(data_simulator_t *sim, float update_time_ms) {
    if (!sim) return;
    
    /* Update running average */
    float alpha = 0.1f; /* Smoothing factor */
    sim->average_update_time_ms = (1.0f - alpha) * sim->average_update_time_ms + 
                                  alpha * update_time_ms;
}

void data_print_statistics(const data_simulator_t *sim) {
    if (!sim) return;
    
    printf("=== Data Simulator Statistics ===\n");
    printf("Mode: %d\n", sim->mode);
    printf("Update Count: %llu\n", (unsigned long long)sim->update_count);
    printf("Error Count: %u\n", sim->error_count);
    printf("Average Update Time: %.2f ms\n", sim->average_update_time_ms);
    printf("Current Speed: %.1f km/h\n", sim->current_data.speed_kmh);
    printf("Current RPM: %.0f\n", sim->current_data.rpm);
    printf("Fuel Level: %.1f%%\n", sim->current_data.fuel_level);
    printf("Engine Temp: %.1f°C\n", sim->current_data.engine_temp);
}

/**
 * Cleanup data simulator
 */
void data_simulator_cleanup(data_simulator_t *sim) {
    if (!sim) return;
    
    printf("Cleaning up data simulator...\n");
    
    /* Print final statistics */
    data_print_statistics(sim);
    
    /* Reset state */
    memset(sim, 0, sizeof(data_simulator_t));
    
    printf("Data simulator cleanup complete\n");
} 