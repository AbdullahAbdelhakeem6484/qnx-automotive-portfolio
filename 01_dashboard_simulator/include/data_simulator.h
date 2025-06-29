/**
 * QNX Dashboard Simulator - Data Simulator Header
 */

#ifndef DATA_SIMULATOR_H
#define DATA_SIMULATOR_H

#include "dashboard.h"

/* Data Simulator Function Prototypes */
int data_simulator_initialize(data_simulator_t *sim);
void data_simulator_cleanup(data_simulator_t *sim);
void data_simulator_update(data_simulator_t *sim, float delta_time);

/* Simulation Mode Functions */
void data_simulator_set_mode(data_simulator_t *sim, simulation_mode_t mode);
simulation_mode_t data_simulator_get_mode(const data_simulator_t *sim);

/* Data Generation Functions */
void data_generate_realistic(data_simulator_t *sim, float delta_time);
void data_generate_demo(data_simulator_t *sim, float delta_time);
void data_generate_manual(data_simulator_t *sim, const vehicle_data_t *input);
void data_generate_replay(data_simulator_t *sim, float delta_time);

/* Physics Simulation */
float physics_calculate_speed(float current_speed, float acceleration, float delta_time);
float physics_calculate_rpm(float speed, int gear, float engine_load);
float physics_calculate_fuel_consumption(float speed, float rpm, float delta_time);
float physics_calculate_engine_temp(float current_temp, float rpm, float ambient_temp, float delta_time);

/* Data Validation */
bool data_validate_vehicle_data(const vehicle_data_t *data);
void data_clamp_values(vehicle_data_t *data);
void data_set_timestamp(vehicle_data_t *data);

/* Scenario Management */
int data_load_scenario(data_simulator_t *sim, const char *filename);
void data_reset_scenario(data_simulator_t *sim);
bool data_scenario_complete(const data_simulator_t *sim);

/* Statistics */
void data_update_statistics(data_simulator_t *sim, float update_time_ms);
void data_print_statistics(const data_simulator_t *sim);
void data_reset_statistics(data_simulator_t *sim);

/* Utility Functions */
float data_smooth_transition(float current, float target, float rate, float delta_time);
float data_add_noise(float value, float noise_level);
bool data_should_trigger_warning(const vehicle_data_t *data);

/* Default Values */
#define DEFAULT_VEHICLE_MASS     1500.0f  /* kg */
#define DEFAULT_DRAG_COEFF       0.35f
#define DEFAULT_ROLLING_RESIST   0.015f
#define DEFAULT_ENGINE_POWER     150.0f   /* kW */
#define DEFAULT_AMBIENT_TEMP     25.0f    /* Celsius */

#define REALISTIC_ACCELERATION   2.5f     /* m/s² */
#define REALISTIC_DECELERATION   -4.0f    /* m/s² */
#define REALISTIC_MAX_SPEED      200.0f   /* km/h for simulation */

#endif /* DATA_SIMULATOR_H */ 