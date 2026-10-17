
//DEFINES HERE

#define MOTOR_PIN PIN_TACT_MOTOR

//END

#ifndef HAPTIC_RATE
	#define HAPTIC_RATE 50
#endif

#ifndef HAPTIC_PIN_INVERTED
	#define HAPTIC_PIN_INVERTED false
#endif

#ifndef MOTOR_PIN
	#define MOTOR_PIN -1
#endif

#ifndef TIMEOUT
	#define TIMEOUT 10000UL
#endif

#ifndef DISCOVERY_RATE
	#define DISCOVERY_RATE 5000UL
#endif

//UNCOMMENT FOR DEBUG LOGGING
#define HAPTIC_DEBUG