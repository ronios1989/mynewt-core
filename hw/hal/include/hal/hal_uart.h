/**
 * Copyright (c) 2015 Runtime Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef H_HAL_UART_H_
#define H_HAL_UART_H_

#include <inttypes.h>

/*
 * Function prototype for UART driver to ask for more data to send.
 * Returns -1 if no more data is available for TX.
 * Driver must call this with interrupts disabled.
 */
typedef int (*uart_tx_char)(void *arg);

/*
 * Function prototype for UART driver to report that transmission is
 * complete. This should be called when transmission of last byte is
 * finished.
 * Driver must call this with interrupts disabled.
 */
typedef void (*uart_tx_done)(void *arg);

/*
 * Function prototype for UART driver to report incoming byte of data.
 * Returns -1 if data was dropped.
 * Driver must call this with interrupts disabled.
 */
typedef int (*uart_rx_char)(void *arg, uint8_t byte);

/**
 * uart init
 *
 * Initializes given uart. Mapping of logical UART number to physical
 * UART/GPIO pins is in TBD.
 */
int uart_init_cbs(int uart, uart_tx_char tx_func, uart_tx_done tx_done,
  uart_rx_char rx_func, void *arg);

enum uart_parity {
    UART_PARITY_NONE = 0,	/* no parity */
    UART_PARITY_ODD = 1,	/* odd parity bit */
    UART_PARITY_EVEN = 2	/* even parity bit */
};

enum uart_flow_ctl {
    UART_FLOW_CTL_NONE = 0,	/* no flow control */
    UART_FLOW_CTL_RTS_CTS = 1	/* RTS/CTS */
};

/**
 * uart config
 *
 * Applies given configuration to UART.
 */
int uart_config(int uart, int32_t speed, uint8_t databits, uint8_t stopbits,
  enum uart_parity parity, enum uart_flow_ctl flow_ctl);

/**
 * uart start tx
 *
 * More data queued for transmission. UART driver will start asking for that
 * data.
 */
void uart_start_tx(int uart);

/**
 * uart start rx
 *
 * Upper layers have consumed some data, and are now ready to receive more.
 * This is meaningful after uart_rx_char callback has returned -1 telling
 * that no more data can be accepted.
 */
void uart_start_rx(int uart);

#endif /* H_HAL_UART_H_ */
