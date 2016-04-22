/*
 * SPISettings.h
 *
 *  Created on: Apr 20, 2016
 *      Author: harry
 */
/** @defgroup base_spi SPI support classes
 *  @brief    Provides SPI support
 */

#ifndef SMINGCORE_SPISETTINGS_H_
#define SMINGCORE_SPISETTINGS_H_

#include "../include/sming_global.h"
#include "Digital.h"

// Mode			Clock Polarity (CPOL)	Clock Phase (CPHA)
//	SPI_MODE0		0					0
//	SPI_MODE1		0					1
//	SPI_MODE2		1					0
//	SPI_MODE3		1					1

#define	SPI_MODE0		0x00
#define	SPI_MODE1		0x0F
#define	SPI_MODE2		0xF0
#define	SPI_MODE3		0xFF

class SPISettings {
public:
	/** @brief SPISettings()
	 * default Constructor
     *  @addtogroup base_spi
     *  @{
	 */
	SPISettings();
	virtual ~SPISettings();

	/** @brief constructor for SPISettings
	 *
	 * Settings are applied to SPI::beginTransaction(SPISettings) and are valid until
	 * next beginTransaction()
	 *
	 * @param 	speed: The maximum speed of communication. For a SPI chip rated up to sys clock speed.
	 * For 20 MHz, use 20000000.
	 * @param	byteOrder: MSBFIRST or LSBFIRST
	 * @param	dataMode : SPI_MODE0, SPI_MODE1, SPI_MODE2, or SPI_MODE3
	 *
	 * byteOrder's are:
	 *
	 * 		MSBFIRST 	Data is sent out starting with Bit31 and down to Bit0
	 * 		LSBFIRST 	Data is sent out starting with the lowest BYTE, from MSB to LSB.
	 *						0xABCDEFGH would be sent as 0xGHEFCDAB
	 *
	 * Data modes are:
	 *
	 *  		Mode		Clock Polarity (CPOL)	Clock Phase (CPHA)
	 * 		SPI_MODE0		0					0
	 * 		SPI_MODE1		0					1
	 * 		SPI_MODE2		1					0
	 * 		SPI_MODE3		1					1
	 */
	SPISettings(int speed, uint8 byteOrder, uint8 dataMode);

	friend class SPIClass;

private:
	int 	_speed;
	uint8	_byteOrder;
	uint8	_dataMode;
	uint32_t _user_regvalue;

	// overload operator to check wheter the settings are equal
	bool operator==(const SPISettings &other) const;

	void print(const char *s);

};

#endif /* SMINGCORE_SPISETTINGS_H_ */
