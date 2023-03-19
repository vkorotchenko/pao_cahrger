/*
 * config.h
 */

#ifndef CONFIG_H_
#define CONFIG_H_

/*
PINS
*/

#define GREEN_PIN 12
#define ORANGE_PIN 13
#define RED_PIN 14
#define SPI_CS_PIN 9 // CS Pins


/*
 * SERIAL CONFIGURATION
 */
#define SERIAL_SPEED 115200

// CONFIGURATIONS
#define tcc_incoming_can_id 0x1806E5F4
#define tcc_outgoing_can_id 0x18FF50E5
#define tcc_send_interval 1000
#define led_reset_interval 1000


#define FULL_CHARGE_MULTIPLIER 1.046875
#define MID_CHARGE_MULTIPLIER 1.040625

/*
 * HARD CODED PARAMETERS
 */

#define CAN_SPEED			500		//can speed in 1000 kbps
#define MAX_VOLTAGE		3650	//max volatage for the battery pack in 1/10th of a volt
#define NOMINAL_VOLTAGE		3200	//nominal volatage for the battery pack in 1/10th of a volt
#define MIN_VOLTAGE         2500    //min voltage for 
#define TARGER_PERCENTAGE   900     // if we are limiting charging to x percente for battery life protection in thenth of a percent
#define MAX_AMPS			200		//Max amp for the charger in 1/10 of an AMP
#define MAX_CAHRGE_TIME		200		//not used yet TODO


/*
    EEPROM ADDRESSES
*/
#define EEPROM_CAN_SPEED	        2 
#define EEPROM_MAX_VOLTAGE			4
#define EEPROM_MAX_AMPS		    	6
#define EEPROM_MAX_CHARGE_TIME      8

#endif /* CONFIG_H_ */