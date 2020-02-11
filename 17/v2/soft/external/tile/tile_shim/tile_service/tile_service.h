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

/**
 * @file tile_service.h"
 * @brief Core functionality for Tile Lib
 */


#ifndef TILE_SERVICE_H_
#define TILE_SERVICE_H_

#include "ble.h"
#include "tile_lib.h"
#include "ble_advdata.h"
#include "ble_advertising.h"

/**
 * @brief Time before automatic disconnection if not authenticated
 */

/**
 * @brief Initialize Tile service. Store advertising data configuration from the application.
 * @note  This function must be called before @ref ble_advertising_init
 */
void tile_service_init(ble_advdata_t const * const p_advdata,
                       ble_advertising_t   * const p_advertising);

void tile_on_ble_evt(ble_evt_t const * p_evt, void * p_context);

uint16_t tile_get_adv_uuid(void);

#endif // TILE_SERVICE_H_

