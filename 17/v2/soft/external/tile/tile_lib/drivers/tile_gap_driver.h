/**
 * NOTICE
 * 
 * Copyright 2017 Tile Inc.  All Rights Reserved.
 * All code or other information included in the accompanying files ("Tile Source Material")
 * is PROPRIETARY information of Tile Inc. ("Tile") and access and use of the Tile Source Material
 * is subject to these terms. The Tile Source Material may only be used for demonstration purposes,
 * and may not be otherwise distributed or made available to others, including for commercial purposes.
 * Without limiting the foregoing , you understand and agree that no production use
 * of the Tile Source Material is allowed without a Tile ID properly obtained under a separate
 * agreement with Tile.
 * You also understand and agree that Tile may terminate the limited rights granted under these terms
 * at any time in its discretion.
 * All Tile Source Material is provided AS-IS without warranty of any kind.
 * Tile does not warrant that the Tile Source Material will be error-free or fit for your purposes.
 * Tile will not be liable for any damages resulting from your use of or inability to use
 * the Tile Source Material.
 *
 * Support: firmware_support@tile.com
 *
 */

/** @file tile_gap_driver.h
 ** @brief Tile GAP Driver interface. Provides TileLib Control over GAP functions, like connection, 
 **        disconnection and connection parameters.
 */

#ifndef TILE_GAP_DRIVER_H_
#define TILE_GAP_DRIVER_H_

#include <stdint.h>


/**
 * Connection parameters.
 */
struct tile_conn_params
{
  uint16_t conn_interval;
  uint16_t slave_latency;
  uint16_t conn_sup_timeout;
};


struct tile_gap_driver
{
  /**
   * Time in 10ms increments before Tile disconnects if no client has
   * authenticated. A value of 0 indicates that this feature is disabled.
   * The value may be updated at any time, but will not clear a timer which
   * is already running. The value is used after a connection is established.
   */
  uint16_t authentication_timer_delay;

  /**
   * Memory space for current connection parameters.
   */
  struct tile_conn_params conn_params;
  
  /**
  * Diagnostic info: counts the number of disconnections triggered by Auth Timer.
   */
  uint16_t* auth_disconnect_count;

  /**
   * Disconnect from the currently connected device.
   *
   * @return See @ref TILE_ERROR_CODES.
   */
  int (*gap_disconnect)(void);
};


/**
 * Register the GAP driver with Tile Library.
 */
int tile_gap_register(struct tile_gap_driver *driver);


/**
 * Call when a connection has been established.
 */
int tile_gap_connected(struct tile_conn_params *conn_params);

/**
 * Call when a connection has been terminated.
 */
int tile_gap_disconnected(void);

/**
 * Call when the connection parameters have been updated. This function will
 * update the values contained in the driver structure.
 */
int tile_gap_params_updated(struct tile_conn_params *conn_params);


#endif
