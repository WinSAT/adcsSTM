/*
 * master_send.h
 *
 *      Author: adam
 *
 *      This is to test master send data
 *      The STM32 communicates to an Arudino i2c slave
 */

/* Pins:
 * SCL - PB8
 * SDA - PB9
 */
#ifndef INC_MASTER_SEND_H_
#define INC_MASTER_SEND_H_

#define MASTER_ADDR 0x61 // STM addr is NA
#define SLAVE_ADDR 0x68 // Arduino slave address

void master_send_init(void);
void master_send_msg(void);

#endif /* INC_MASTER_SEND_H_ */
