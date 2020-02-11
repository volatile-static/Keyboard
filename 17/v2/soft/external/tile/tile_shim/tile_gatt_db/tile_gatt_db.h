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
 * @file tile_service.h
 * @brief Set up the Tile service
 */

#ifndef TILE_GATT_DB_H_
#define TILE_GATT_DB_H_

#include <stdint.h>

/**
  * Attribute ID's associated with each Tile attribute.
  */
enum TILE_CHARACTERISTICS
{
  TILE_ID_CHAR,       // this is here for backward compatibility and should be removed
  TILE_TOA_CMD_CHAR,
  TILE_TOA_RSP_CHAR,
  TILE_TOA_RSP_CCCD,
  TILE_NUM_ATTRS
};

typedef struct
{
  uint16_t service_handle;
  uint16_t characteristic_handles[TILE_NUM_ATTRS];
} tile_gatt_db_t;

void tile_gatt_db_init(tile_gatt_db_t *p_service);

#endif
