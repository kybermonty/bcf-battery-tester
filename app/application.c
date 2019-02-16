#include <application.h>

float voltage;
int percentage;

void application_init(void)
{
    voltage = 0;
    percentage = 0;

    // Battery Module
    bc_module_battery_init();
    bc_module_battery_set_event_handler(battery_module_event_handler, NULL);
    bc_module_battery_set_update_interval(2000);

    // LCD Module
    bc_module_lcd_init();
}

void application_task(void)
{
    if (!bc_module_lcd_is_ready())
    {
        bc_scheduler_plan_current_relative(500);
        return;
    }

    char str[32];

    bc_system_pll_enable();

    bc_module_lcd_clear();

    bc_module_lcd_set_font(&bc_font_ubuntu_28);
    snprintf(str, sizeof(str), "%.1f V", voltage);
    bc_module_lcd_draw_string(25, 10, str, true);

    bc_module_lcd_set_font(&bc_font_ubuntu_28);
    snprintf(str, sizeof(str), "%d %%", percentage);
    bc_module_lcd_draw_string(25, 50, str, true);

    bc_system_pll_disable();

    bc_module_lcd_update();

    bc_scheduler_plan_current_relative(1000);
}

void battery_module_event_handler(bc_module_battery_event_t event, void *event_param)
{
    (void) event_param;

    if (event == BC_MODULE_BATTERY_EVENT_UPDATE)
    {
        float value1;
        int value2;
        if (bc_module_battery_get_voltage(&value1))
        {
            voltage = value1;
        }
        if (bc_module_battery_get_charge_level(&value2)) {
            percentage = value2;
        }
    }
}
