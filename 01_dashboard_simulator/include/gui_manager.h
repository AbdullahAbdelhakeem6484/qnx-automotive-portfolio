/**
 * QNX Dashboard Simulator - GUI Manager Header
 */

#ifndef GUI_MANAGER_H
#define GUI_MANAGER_H

#include "dashboard.h"

/* GUI Function Prototypes */
int gui_manager_initialize(gui_manager_t *gui);
void gui_manager_cleanup(gui_manager_t *gui);
int gui_manager_create_window(gui_manager_t *gui);
int gui_manager_create_widgets(gui_manager_t *gui);

void gui_manager_update(gui_manager_t *gui, const vehicle_data_t *data);
void gui_manager_render(gui_manager_t *gui);
void gui_manager_handle_events(gui_manager_t *gui);

int gui_manager_set_theme(gui_manager_t *gui, gui_theme_t theme);
void gui_manager_toggle_fullscreen(gui_manager_t *gui);

/* Gauge Functions */
int gui_create_gauge_widget(gauge_widget_t *gauge, const char *label, 
                           float min_val, float max_val, 
                           PhPoint_t pos, PhDim_t size);
void gui_update_gauge_value(gauge_widget_t *gauge, float value);
void gui_animate_gauge(gauge_widget_t *gauge, float delta_time);

/* Warning Light Functions */
int gui_create_warning_light(warning_light_t *warning, const char *label,
                            PhPoint_t pos, PhDim_t size);
void gui_set_warning_state(warning_light_t *warning, bool active, bool blinking);
void gui_update_warning_blink(warning_light_t *warning, uint32_t current_time);

/* Event Handlers */
int gui_window_close_callback(PtWidget_t *widget, ApInfo_t *apinfo, 
                             PtCallbackInfo_t *cbinfo);
int gui_window_resize_callback(PtWidget_t *widget, ApInfo_t *apinfo,
                              PtCallbackInfo_t *cbinfo);
int gui_key_press_callback(PtWidget_t *widget, ApInfo_t *apinfo,
                          PtCallbackInfo_t *cbinfo);

/* Drawing Functions */
void gui_draw_gauge_background(PtWidget_t *widget, PhTile_t *damage);
void gui_draw_gauge_needle(PtWidget_t *widget, float value, float min_val, float max_val);
void gui_draw_digital_display(PtWidget_t *widget, const char *text, PhPoint_t pos);

/* Theme Management */
PgColor_t gui_get_theme_color(gui_theme_t theme, const char *element);
void gui_apply_theme_colors(gui_manager_t *gui);

/* Utility Functions */
PhPoint_t gui_calculate_needle_position(float value, float min_val, float max_val, 
                                       PhPoint_t center, int radius);
float gui_smooth_value_transition(float current, float target, float speed, float delta_time);

#endif /* GUI_MANAGER_H */ 