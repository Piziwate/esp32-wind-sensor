#include "mgos.h"


//#include "mgos_mqtt.h"
//#include "mgos_rpc.h"
//#include "mgos_vfs.h"

#define HALF_CIRCUMFERENCE 17.27875

static uint16_t	prev_wind_pulses=0;
static uint16_t	wind_pulses=0;


static void int_handle_wind_pulse(int pin, void *arg)
{
	wind_pulses++;
}

static void timer_debug_cb(void *arg)
{
	uint16_t delta = wind_pulses - prev_wind_pulses;
	prev_wind_pulses = wind_pulses;
	
	float wind_speed = (HALF_CIRCUMFERENCE * delta * 360.0) / 1000000.0 ; // km/h
	
	LOG(LL_INFO,("Speed : %.4f [km/h], Pulses /sec : %d (curr=%d, prev=%d)", wind_speed, delta, wind_pulses, prev_wind_pulses));
	// diameter 11mm - circumference = pi * 11 = 34.5575 mm
		
	(void)arg;
}

enum mgos_app_init_result mgos_app_init(void)
{
  //mgos_gpio_setup_output(mgos_sys_config_get_board_led1_pin(), 0);

  LOG(LL_INFO, ("Initializing wind input..."));
  mgos_gpio_set_mode(5, MGOS_GPIO_MODE_INPUT);
  mgos_gpio_set_pull(5, MGOS_GPIO_PULL_UP);
  mgos_gpio_set_int_handler(5, MGOS_GPIO_INT_EDGE_NEG, int_handle_wind_pulse, NULL);
  mgos_gpio_enable_int(5);

  mgos_set_timer(10000 /* ms */, MGOS_TIMER_REPEAT, timer_debug_cb, NULL);

  return MGOS_APP_INIT_SUCCESS;
}
