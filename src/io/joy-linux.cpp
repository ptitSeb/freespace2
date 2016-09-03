/*
 * Native Linux joystick for FreeSpace 2.
 *
 * Author: Karl Robillard <wickedsmoke@users.sf.net>
 */


#include "pstypes.h"
#include "joy.h"
#include "fix.h"
#include "key.h"
#include "timer.h"
#include "osregistry.h"
#include "controlsconfig.h"
#include "joy_ff.h"
#include "osapi.h"
#include <linux/joystick.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>


#define DEFAULT_JOYSTICK    0


int Joy_sensitivity = 9;
int Dead_zone_size  = 10;   // percentage of range that is dead zone
Joy_info joystick;


static int _joyfd = -1;
static int _joy_axis_value[JOY_NUM_AXES];


typedef struct joy_button_info {
	int     state;              // 1 when the button is down.
	int     down_count;
	uint    down_time;          // Accumulated milliseconds down.
    uint    last_down_check;    // Timestamp in milliseconds of last check.
	uint    down_post;          // Time of last check or down transition.
} joy_button_info;

static joy_button_info _joy_buttons[JOY_TOTAL_BUTTONS];


#define VALID_BTN(b)    (_joyfd > -1) && (b >= 0) && (b < JOY_TOTAL_BUTTONS)


// --------------------------------------------------------------
//  joy_down()
//
//  Return the state of button number 'btn'
//
//  input:      btn =>      button number to check
//
//  returns:    0   =>      not pressed
//              1   =>      pressed
//
int joy_down(int btn)
{
    if( VALID_BTN(btn) )
    {
        return _joy_buttons[btn].state;
    }
    return 0;
}


// --------------------------------------------------------------
//  joy_down_count()
//
//  Return the number of times the joystick button has gone down since
//  joy_down_count() was last called
//
//  input:      btn         =>      button number to check
//              reset_count =>      (default 1): if true reset down_count
//
//  returns:    number of times button 'btn' has gone down since last call
//
int joy_down_count(int btn, int reset_count)
{
    if( VALID_BTN(btn) )
    {
        int tmp = _joy_buttons[btn].down_count;
        if( reset_count )
            _joy_buttons[btn].down_count = 0;
        return tmp;
    }
    return 0;
}


// --------------------------------------------------------------
//  joy_down_time()
//
//  Return a number between 0 and 1.  This number represents the percentage
//  time that the joystick button has been down since it was last checked
//
//  input:      btn =>      button number to check
//
//  returns:    value between 0 and 1
//
float joy_down_time(int btn)
{
    if( VALID_BTN(btn) )
    {
        float rval;
        uint  now;
        joy_button_info* bi;

        now = timer_get_milliseconds();

        bi = &_joy_buttons[btn];

        if( bi->down_post )
        {
            bi->down_time += now - bi->down_post;
            bi->down_post = now;
        }

        if (now > bi->last_down_check)
            rval = i2fl(bi->down_time) / (now - bi->last_down_check);
        else
            rval = 0.0f;

        bi->down_time = 0;
        bi->last_down_check = now;

        if (rval < 0)
            rval = 0.0f;
        if (rval > 1)
            rval = 1.0f;

        return rval;
    }
    return 0.0f;
}


// --------------------------------------------------------------
//  joy_flush()
//
// Clear the state of the joystick.
//
void joy_flush()
{
    if (_joyfd > -1)
    {
        int i;
        joy_button_info *bi;
        uint now = timer_get_milliseconds();

        for ( i = 0; i < JOY_TOTAL_BUTTONS; i++)
        {
            bi = &_joy_buttons[i];
            bi->state       = 0;
            bi->down_count  = 0;
            bi->down_time   = 0;
            bi->down_post   = 0;
            bi->last_down_check = now;
        }
    }
}


int joy_get_unscaled_reading(int raw, int axn)
{
	int rng;

	// Make sure it's calibrated properly.
	if (joystick.axis_center[axn] - joystick.axis_min[axn] < 5)
		return 0;

	if (joystick.axis_max[axn] - joystick.axis_center[axn] < 5)
		return 0;

	rng = joystick.axis_max[axn] - joystick.axis_min[axn];
	raw -= joystick.axis_min[axn];  // adjust for linear range starting at 0
	
	// cap at limits
	if (raw < 0)
		raw = 0;
	if (raw > rng)
		raw = rng;

	return (int) ((unsigned int) raw * (unsigned int) F1_0 / (unsigned int) rng);  // convert to 0 - F1_0 range.
}


// --------------------------------------------------------------
//	joy_get_scaled_reading()
//
//	input:  raw =>  the raw value for an axis position
//          axn =>  axis number, numbered starting at 0
//
// return:  joy_get_scaled_reading will return a value that represents
//          the joystick pos from -1 to +1 for the specified axis number
//          'axn', and the raw value 'raw'
//
int joy_get_scaled_reading(int raw, int axn)
{
	int x, d, dead_zone, rng;
	float percent, sensitivity_percent, non_sensitivity_percent;

	// Make sure it's calibrated properly.
	if (joystick.axis_center[axn] - joystick.axis_min[axn] < 5)
		return 0;

	if (joystick.axis_max[axn] - joystick.axis_center[axn] < 5)
		return 0;

	raw -= joystick.axis_center[axn];

	dead_zone = (joystick.axis_max[axn] - joystick.axis_min[axn]) * Dead_zone_size / 100;

	if (raw < -dead_zone) {
		rng = joystick.axis_center[axn] - joystick.axis_min[axn] - dead_zone;
		d = -raw - dead_zone;

	} else if (raw > dead_zone) {
		rng = joystick.axis_max[axn] - joystick.axis_center[axn] - dead_zone;
		d = raw - dead_zone;

	} else
		return 0;

	if (d > rng)
		d = rng;

	Assert(Joy_sensitivity >= 0 && Joy_sensitivity <= 9);

	// compute percentages as a range between 0 and 1
	sensitivity_percent = (float) Joy_sensitivity / 9.0f;
	non_sensitivity_percent = (float) (9 - Joy_sensitivity) / 9.0f;

	// find percent of max axis is at
	percent = (float) d / (float) rng;

	// work sensitivity on axis value
	percent = (percent * sensitivity_percent + percent * percent * percent * percent * percent * non_sensitivity_percent);

	x = (int) ((float) F1_0 * percent);

	//nprintf(("AI", "d=%6i, sens=%3i, percent=%6.3f, val=%6i, ratio=%6.3f\n", d, Joy_sensitivity, percent, (raw<0) ? -x : x, (float) d/x));

	if (raw < 0)
		return -x;

	return x;
}


// --------------------------------------------------------------
//  joy_get_pos()
//
//  input:  x   =>   OUTPUT PARAMETER: x-axis position of stick (-1 to 1)
//          y   =>   OUTPUT PARAMETER: y-axis position of stick (-1 to 1)
//          z   =>   OUTPUT PARAMETER: z-axis (throttle) position of stick (-1 to 1)
//          r   =>   OUTPUT PARAMETER: rudder position of stick (-1 to 1)
//
//  return:   success    => 1
//            failure    => 0
//
int joy_get_pos(int *x, int *y, int *z, int *rx)
{
	if (x) *x = 0;
	if (y) *y = 0;
	if (z) *z = 0;
	if (rx) *rx = 0;

	if (_joyfd == -1)
        return 0;

	//	joy_get_scaled_reading will return a value represents the joystick pos from -1 to +1
	if (x && joystick.axis_valid[0])
		*x = joy_get_scaled_reading(_joy_axis_value[0], 0);
	if (y && joystick.axis_valid[1]) 
		*y = joy_get_scaled_reading(_joy_axis_value[1], 1);
	if (z && joystick.axis_valid[2])
		*z = joy_get_unscaled_reading(_joy_axis_value[2], 2);
	if (rx && joystick.axis_valid[3])
		*rx = joy_get_scaled_reading(_joy_axis_value[3], 3);

	return 1;
}


/*
   Convert Linux axis value to FreeSpace button.
*/
static void convertAxisToHat( int value, int btnA, int btnB )
{
    if( value < -20000 )
    {
        _joy_buttons[ btnA ].state = 1;
        _joy_buttons[ btnB ].state = 0;
    }
    else if( value > 20000 )
    {
        _joy_buttons[ btnA ].state = 0;
        _joy_buttons[ btnB ].state = 1;
    }
    else
    {
        _joy_buttons[ btnA ].state = 0;
        _joy_buttons[ btnB ].state = 0;
    }
}


void joy_read()
{
    struct js_event je;

    if( _joyfd == -1 )
        return;

    while( read(_joyfd, &je, sizeof(struct js_event)) > 0 )
    {
        if( je.type & JS_EVENT_BUTTON )
        {
            // je.value will be 0 or 1.

            if( je.number < JOY_NUM_AXES )
            {
                joy_button_info* bi = &_joy_buttons[ je.number ];
                uint now = timer_get_milliseconds();

                bi->state = je.value;

                if (bi->state)
                {
                    // Went from up to down.
                    bi->down_count++;
                    bi->down_post = now;
                }
                else
                {
                    // Went from down to up.
                    if( bi->down_post )
                    {
                        bi->down_time += now - bi->down_post;
                        bi->down_post = 0;
                    }
                }
            }
        }
        else if( je.type & JS_EVENT_AXIS )
        {
            // je.value will be -32767 to +32767.

            if( je.number < JOY_NUM_AXES )
            {
                _joy_axis_value[ je.number ] = je.value + 32768;

                // Joystick hat is typically on axis 4/5
                if( je.number == 4 )
                {
                    convertAxisToHat( je.value, JOY_HATLEFT, JOY_HATRIGHT );
                }
                else if( je.number == 5 )
                {
                    convertAxisToHat( je.value, JOY_HATFORWARD, JOY_HATBACK );
                }
            }
        }
    }

    if( errno != EAGAIN )
        perror( "joy_read" );
}

#ifdef PANDORA
static char _joy_device[] = "/dev/input/js0";
#else
static char _joy_device[] = "/dev/js0";
#endif

static bool openJoyDevice( int n )
{
    _joy_device[7] = '0' + n;
    _joyfd = open( _joy_device, O_RDONLY | O_NONBLOCK );
    return (_joyfd > -1) ? true : false;
}


// --------------------------------------------------------------
//  joy_init()
//
// Initialize the joystick system.  This is called once at game startup.
//
int joy_init()
{
	int i;
    char joyname[80];
    int current;
    int buttons, axes;

	if (_joyfd > -1)
		return 0;

	current = os_config_read_uint (NULL, "CurrentJoystick", DEFAULT_JOYSTICK);

    if( ! openJoyDevice( current ) )
    {
        if( current != DEFAULT_JOYSTICK )
            mprintf(("Unable to open joystick %s\n", _joy_device ));

        for( i = 0; i < 4; ++i )
        {
            if( i == current )
                continue;
            if( openJoyDevice( i ) )
            {
                current = i;
                break;
            }
        }

        if( _joyfd < 0 )
        {
            mprintf(("No joysticks found\n"));
            return 0;
        }
    }
    
    ioctl( _joyfd, JSIOCGAXES, &axes );
    ioctl( _joyfd, JSIOCGBUTTONS, &buttons );
    ioctl( _joyfd, JSIOCGNAME(80), &joyname );

    nprintf (("JOYSTICK", "Joystick #%d: %s\n", current, joyname));

    for (i=0; i < JOY_NUM_AXES; i++)
    {
        joystick.axis_valid[i] = (i < axes) ? 1 : 0;
        _joy_axis_value[i] = 32768;   // Center
    }

	joy_flush();

	// Fake a calibration
    for (i=0; i<4; i++) {
        joystick.axis_center[i] = 32768;
        joystick.axis_min[i] = 0;
        joystick.axis_max[i] = 65536;
    }


    // Map throttle by default if joystick has at least 3 axes.
    if( axes > 2 )
    {
        extern int Axis_map_to_defaults[5];
        Axis_map_to_defaults[3] = JOY_Z_AXIS;
    }

	return 1;
}


// --------------------------------------------------------------
//  joy_close()
//
// Close the joystick system.  Should be called at game exit.
//
void joy_close()
{
    if( _joyfd > -1 )
    {
        close( _joyfd );
        _joyfd = -1;
    }
}


void joy_set_cen()
{
	joystick_read_raw_axis( 2, joystick.axis_center );
}

int joystick_read_raw_axis(int num_axes, int *axis)
{
	int i;
	
	if (_joyfd == -1)
		return 0;
	
	for (i = 0; i < num_axes; i++)
        axis[i] = _joy_axis_value[i];
	
	return 1;
}

void joy_ff_adjust_handling(int speed)
{
//	STUB_FUNCTION;
}

void joy_ff_afterburn_off()
{
//	STUB_FUNCTION;
}

void joy_ff_afterburn_on()
{
//	STUB_FUNCTION;
}

void joy_ff_deathroll()
{
//	STUB_FUNCTION;
}

void joy_ff_docked()
{
//	STUB_FUNCTION;
}

void joy_ff_explode()
{
//	STUB_FUNCTION;
}

void joy_ff_fly_by(int mag)
{
//	STUB_FUNCTION;
}

void joy_ff_mission_init(vector v)
{
//	STUB_FUNCTION;
}

void joy_ff_play_dir_effect(float x, float y)
{
//	STUB_FUNCTION;
}

void joy_ff_play_primary_shoot(int gain)
{
//	STUB_FUNCTION;
}

void joy_ff_play_reload_effect()
{
//	STUB_FUNCTION;
}

void joy_ff_play_secondary_shoot(int gain)
{
//	STUB_FUNCTION;
}

void joy_ff_play_vector_effect(vector *v, float scaler)
{
//	STUB_FUNCTION;
}

void joy_ff_stop_effects()
{
	joy_ff_afterburn_off();
}


//EOF
