/**
 * NOTICE
 * 
 * Copyright 2014 Tile Inc.  All Rights Reserved.
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

/** @file toa.h
 ** @brief Tile Overtheair Api: defines Tile communication protocol over the air
 */

#ifndef TOA_H_
#define TOA_H_

#include <stdint.h>
#include <stdbool.h>

#include "modules/tile_toa_module.h"

#define TILE_SUPPORT_TEST 1

/**
 * @brief TOA Features enabling
 * This is to automatically add TOA Feature Bits when these are supported
 */
#ifdef TILE_SUPPORT_TAC
  #define TAC_VAL 0x80
#else
  #define TAC_VAL 0
#endif
#ifdef TILE_SUPPORT_TEST
  #define TEST_VAL 0x40
#else
  #define TEST_VAL 0
#endif

/** @defgroup TOA Tile Over-the-air API
 *  @{
 *  @ingroup ext_ble_lib
 *  @brief Tile Over-the-air Api: defines Tile communication protocol over the air
 *
 *  @anchor TOA_Service
 *  @details TOA is a transport using 2 GATT characteristics.
 *  - TOA commands are received by the server using GATT Write without Response on Characteristic TOA_CMD.<br>
 *  - TOA responses are transmited using GATT Notify on characteristic TOA_RSP.<br>
 *  - TOA Client sends @ref TOA_CMD and TOA Server sends @ref TOA_RSP.<br>
 *  - TOA is a multiplexer for supporting independent Upper Layer, these can be seen as TOA channels.<br>
 *  - TOA is stateless and does not guarantee a command will provide a response. The Upper Layer may guarantee this.<br>
 *  - TOA supports a notification mechanism, which means an Upper Layer may send a response message when no command was sent.<br>

 * @anchor MEP_TOA @par Multi-End-Point and Legacy TOA
 * There are 2 versions of TOA: Legacy TOA and Multi_End_Point TOA (MEP_TOA).<br>
 * Support for either version is discovered through Characteristic discovery (as these use different BLE characteristics).<br><br>
 * The main difference between the 2 are as follows:
 * - Community Find:
 *   - MEP_TOA Supports a Command to Authenticate over a connectionless channel (for Community Find).
 *   - When MEP_TOA is not supported, Community Find authentication is achieved through Legacy Authentication (over legacy characteristics)
 * - TOA Channel Open:
 *   - MEP_TOA Supports a Command to Open a channel (sent over a connectionless channel).
 *   - When MEP_TOA is not supported, Community Find authentication is achieved through Legacy Authentication (over legacy characteristics)
 *   - Legacy TOA requires Legacy Authentication (over legacy characteristics) prior to opening the TOA Channel.
 *   - On Legacy TOA, configuring TOA_RSP Characterisitc for notifications opens the channel.
 *   - On MEP_TOA, configuring MEP_RSP Characterisitc for notifications is done before sending any TOA Comamnd.
 * - TOA Commands and Format:
 *   - MEP_TOA adds a Channel ID (CID) Byte at the beginning of TOA Commands and Responses.
 *
 * @cond MEP
 *  Here is a description of multi-endpoint TOA:  @ref MEP. <br>
 *  Here are the Legacy Characteristics:  @ref LEGACY_CHARACTERISTICS. <br>
 * @endcond
 *
 * @defgroup TOA_COMPATIBILITY Tile Over-the-air API Compatibility
 * @ingroup TOA TOA Compatibility
 * TOA compatibility Requirements:<br>
 * - TOA Server:
 *   - TOA Server shall ignore (without behavioral impact) any unknown @ref TOA_CMD and respond with error @ref TOA_RSP_ERROR_UNSUPPORTED.
 *   - TOA Server shall ignore (without behavioral impact) any unknown additional parameters after any TOA_CMD payload.
 * - TOA Client:
 *   - TOA Client shall ignore (without behavioral impact) any unknown @ref TOA_RSP.
 *   - TOA Client shall ignore (without behavioral impact) any unknown additional parameters after any TOA_RSP payload.
 *   - There is no mandatory TOA feature and the TOA Client shall support that any TOA feature is not supported (like @ref TMD or @ref TILE_SONG for instance).
 *  @{
 */


/**
 * @brief CID for TOA connectionless channel.
 */
#define TOA_CONNECTIONLESS_CID 0


/**
 * @brief Size of the token included with each connectionless command/response.
 */
#define TOA_TOKEN_LEN 4


/**
 * @brief CID for TOA broadcast messages.
 * Broadcast messages are unidirectional, from Server to Client. Broadcast
 * messages contain a MIC. The parameters for the broadcast session key
 * may be requested by an authenticated Client at any time.
 */
#define TOA_BROADCAST_CID 1

/**
 * @brief Token to use on the Conectionless CID to send broadcast Responses.
 *  When supported, TOA Clients will process any response using this token.
 */
#define TOA_BROADCAST_TOKEN 0xFFFFFFFF


/**
 * @brief Defines related to authentication
 */
#define TILE_AUTH_RAND_A_LEN  14
#define TILE_AUTH_RAND_T_LEN  10
#define TILE_SRES_LEN         4
#define TILE_RAND_A_LEN       14
#define TILE_RAND_T_LEN       13
#define TILE_RAND_T_FIXED_LEN 10
#define TILE_MIC_LEN          4


/**
 * @brief TOA Response Codes<br>
  TOA_RSP Format:
  
  TOA_RSP Code | Payload                                     | MIC
  -------------|---------------------------------------------|--------
  1 Byte       | Up to @ref TOA_MPS Bytes (Max Payload Size) | 4 Bytes

  @ref TOA_MPS is defined by the TOA Server and is provided in the @ref TOA_RSP_READY Response<br>
  TOA Client shall silently ignore an unkown TOA_RSP.<br>
  TOA Client shall silently ignore any extra unknown parameter at the end of a response.<br>
 */
enum TOA_RSP
{
  TOA_RSP_RESERVED      = 0x00, /**< reserved, not used. */
  TOA_RSP_READY         = 0x01,
  /**< This Response is sent when a @ref TOA_CMD_READY command is received. <br>
  Note: For Legacy TOA, this response is sent as soon as notification has been enabled on TOA_RSP characteristic. <br>
  Format:
  
  @ref TOA_RSP_READY Code | @ref TOA_MPS  | Supported Features Byte 0, 1 and 2         | TOA Broadcast Nonce | RFU
  ------------------------|---------------|--------------------------------------------|---------------------|--------------------------------
  1 Byte                  |  1 Byte       | 3 Bytes, check @ref TOA_SUPPORTED_FEATURES | 4 Bytes             | Varies, Reserved for Future Use
  */

  TOA_RSP_TOFU_CTL      = 0x02, /**< TOFU Control Response */
  TOA_RSP_ASSERT        = 0x03, /**< ASSERT Response */
  TOA_RSP_BDADDR        = 0x04, /**< BDADDR Response */
  TOA_RSP_ERROR         = 0x05,
  /**< This Response is sent when something goes wrong. <br>
  see @ref TOA_ERROR_CODES for a description of the possible errors and their parameters <br>
  Format:
  
  @ref TOA_RSP_ERROR Code | @ref TOA_ERROR_CODES  |  Parameters
  ------------------------|-----------------------|------------
  1 Byte                  |  1 Byte               | Varies
  */

  TOA_RSP_TDT           = 0x06, /**< TDT Response */
  TOA_RSP_SONG          = 0x07, /**< SONG Response */
  TOA_RSP_PPM           = 0x08, /**< PPM Response */
  TOA_RSP_ADV_INT       = 0x09, /**< ADV_INT Response */
  TOA_RSP_TKA           = 0x0a, /**< TKA Response */
  TOA_RSP_TAC           = 0x0b, /**< TAC Response */
  TOA_RSP_TDG           = 0x0c, /**< TDG Response */
  TOA_RSP_TMD           = 0x0d, /**< TMD Response */
  TOA_RSP_TCU           = 0x0e, /**< TCU Response */
  TOA_RSP_TIME          = 0x0f, /**< TIME Response */
  TOA_RSP_TEST          = 0x10, /**< TEST Response */
  TOA_RSP_TFC           = 0x11, /**< TFC Response */
  TOA_RSP_OPEN_CHANNEL  = 0x12,
  /**< OPEN_CHANNEL Response.
        This Response is Mandatory over @ref MEP_TOA and forbidden on Legacy TOA
        Used on @ref TOA_CONNECTIONLESS_CID. Format:
		
        @ref TOA_RSP_OPEN_CHANNEL | Allocated CID | RAND_T
        --------------------------|---------------|---------
        1 byte                    | 1 byte        | 13 bytes
        */
  TOA_RSP_CLOSE_CHANNEL = 0x13,
  /**< CLOSE_CHANNEL Response.
        This Response is Mandatory over @ref MEP_TOA and forbidden on Legacy TOA
        Format:
		
        @ref TOA_RSP_CLOSE_CHANNEL | @ref TOA_CHANNEL_CLOSE_REASONS code | Payload
        ---------------------------|-------------------------------------|--------
        1 byte                     | 1 byte                              | Varies
        */
  TOA_RSP_TDI           = 0x14, /**< TDI Response. Used on @ref TOA_CONNECTIONLESS_CID. */
  TOA_RSP_AUTHENTICATE  = 0x15,
  /**< AUTHENTICATE Response.
        This Response is Mandatory over @ref MEP_TOA and forbidden on Legacy TOA
        Used on @ref TOA_CONNECTIONLESS_CID. Format:
		
        @ref TOA_RSP_AUTHENTICATE | RAND_T   | SRES_T
        --------------------------|----------|--------
        1 byte                    | 10 bytes | 4 bytes
		
        To generate SRES_T, follow the following steps:
        1. Pad the end of RAND_A and RAND_T until each is 16 bytes
        2. Run HMAC-SHA256 on RAND_A concatenated with RAND_T and keyed with the auth key
        3. Take bytes 4:7. These bytes are SRES_T.
        */
  TOA_RSP_TMF           = 0x16, /**< TMF Response */
  TOA_RSP_TLIL          = 0x17, /**< TLIL Response */
  TOA_RSP_TEF           = 0x18, /**< TEF Response */
  TOA_RSP_TRM           = 0x19, /**< TRM Response */
  TOA_RSP_TPC           = 0x1A, /**< TPC Response */
  TOA_RSP_ASSOCIATE     = 0x1B,
  /**< ASSOCIATE Response.
        This Response is Mandatory over @ref MEP_TOA and forbidden on Legacy TOA
        Used on @ref TOA_CONNECTIONLESS_CID. 
        Format:
		
        @ref TOA_RSP_ASSOCIATE    | RAND_T   | SRES_T
        --------------------------|----------|--------
        1 byte                    | 10 bytes | 4 bytes
		
        To generate SRES_T, follow the following steps:
        1. Pad the end of RAND_A and RAND_T until each is 16 bytes
        2. Run HMAC-SHA256 on RAND_A concatenated with RAND_T and keyed with the auth key
        3. Take bytes 4:7. These bytes are SRES_T.
        */
  TOA_RSP_AUTHORIZED    = 0x1C,
  /**< AUTHORIZED Response.
        This Response is Mandatory over @ref MEP_TOA and forbidden on Legacy TOA
        Used on @ref TOA_CONNECTIONLESS_CID. 
        It is a broadcast Response so it is sent using the @ref TOA_BROADCAST_TOKEN
        Format:
	
        @ref TOA_RSP_AUTHORIZED   | Authorization Type                | Authorization Timer
        --------------------------|-----------------------------------|---------------------
        1 byte                    | 1 Byte (value 1 for Button Press) | 2 bytes (in seconds)
        */
};

/**
 * @brief TOA Command Codes<br>
  TOA_CMD Format:
  
  TOA_CMD Code | Payload                                     | MIC
  -------------|---------------------------------------------|---------
  1 Byte       | Up to @ref TOA_MPS Bytes (Max Payload Size) | 4 Bytes

  @ref TOA_MPS is provided by the TOA Server in the @ref TOA_RSP_READY response.<br>
  TOA Server shall respond to an unknow @ref TOA_CMD by a @ref TOA_RSP_ERROR with ErrorCode @ref TOA_RSP_ERROR_UNSUPPORTED.<br>
  TOA Server shall silently ignore any extra unknown parameter at the end of a command.
 */
enum TOA_CMD
{
  TOA_CMD_TOFU_CTL      = 0x01, /**< TOFU Control Command */
  TOA_CMD_TOFU_DATA     = 0x02,
  /**< TOFU_DATA Command<br>
  TOFU Data Channel is separated from the TOFU Control Channel for efficiency reasons.
  Note that TOFU Data Can only be sent when TOFU is ready.
  Format:
  
  @ref TOA_CMD_TOFU_DATA Code | TOFU Data
  ----------------------------|----------------------------------
  1 Byte                      | Varies (Up to @ref TOA_MPS Bytes)
  */

  TOA_CMD_BDADDR        = 0x03, /**< BDADDR Command */
  TOA_CMD_TDT           = 0x04, /**< TDT Command */
  TOA_CMD_SONG          = 0x05, /**< SONG Command */
  TOA_CMD_PPM           = 0x06, /**< PPM Command */
  TOA_CMD_ADV_INT       = 0x07, /**< ADV_INT Command */
  TOA_CMD_TKA           = 0x08, /**< TKA Command */
  TOA_CMD_TAC           = 0x09, /**< TAC Command */
  TOA_CMD_TDG           = 0x0a, /**< TDG Command */
  TOA_CMD_TMD           = 0x0b, /**< TMD Command */
  TOA_CMD_TCU           = 0x0c, /**< TCU Command */
  TOA_CMD_TIME          = 0x0d, /**< TIME Command */
  TOA_CMD_TEST          = 0x0e, /**< TEST Command */
  TOA_CMD_TFC           = 0x0f, /**< TFC Command */
  TOA_CMD_OPEN_CHANNEL  = 0x10,
  /**< OPEN_CHANNEL Command. 
        This Command is Mandatory over @ref MEP_TOA and forbidden on Legacy TOA
        Available on @ref TOA_CONNECTIONLESS_CID. Format:
		
        @ref TOA_CMD_OPEN_CHANNEL | RAND_A
        --------------------------|---------
        1 byte                    | 14 bytes
        */
  TOA_CMD_CLOSE_CHANNEL = 0x11,
  /**< CLOSE_CHANNEL Command. 
        This Command is Mandatory over @ref MEP_TOA and forbidden on Legacy TOA
        Format:
		
        @ref TOA_CMD_CLOSE_CHANNEL | @ref TOA_CHANNEL_CLOSE_REASONS code | Payload
        ---------------------------|-------------------------------------|--------
        1 byte                     | 1 byte                              | Varies
        */
  TOA_CMD_READY         = 0x12,
  /**< READY Command. 
        This Command is Mandatory over @ref MEP_TOA and forbidden on Legacy TOA
        Format:
		
        @ref TOA_CMD_READY | @ref TOA_MPS | @ref TOA_CLIENT_SUPPORTED_FEATURES
        -------------------|--------------|-----------------------------------
        1 byte             | 1 byte       | Varies
        */
  TOA_CMD_TDI           = 0x13, 
    /**< TDI Command. 
        This Command is Mandatory over @ref MEP_TOA and forbidden on Legacy TOA
        Available on @ref TOA_CONNECTIONLESS_CID. */
  TOA_CMD_AUTHENTICATE  = 0x14,
  /**< AUTHENTICATE Command. 
        This Command is Mandatory over @ref MEP_TOA and forbidden on Legacy TOA
        Available on @ref TOA_CONNECTIONLESS_CID. Format:
		
        @ref TOA_CMD_AUTHENTICATE | RAND_A
        --------------------------|---------
        1 byte                    | 14 bytes
        */
  TOA_CMD_TMF           = 0x15, /**< TMF Command */
  TOA_CMD_TLIL          = 0x16, /**< TLIL Command */
  TOA_CMD_TEF           = 0x17, /**< TEF Command */
  TOA_CMD_TRM           = 0x18, /**< TRM Command */
  TOA_CMD_TPC           = 0x19, /**< TPC Command */
  TOA_CMD_ASSOCIATE     = 0x1A,
  /**< ASSOCIATE Command. 
        This Command is Mandatory over @ref MEP_TOA and forbidden on Legacy TOA
        Available on @ref TOA_CONNECTIONLESS_CID. 
    Will trigger either a @ref TOA_RSP_ASSOCIATE or a @ref TOA_RSP_ERROR with error code @ref TOA_RSP_ERROR_AUTHORIZATION
    Format:
	
        @ref TOA_CMD_ASSOCIATE    | RAND_A
        --------------------------|---------
        1 byte                    | 14 bytes
        */
};


/**
 * @brief Reason codes for @ref TOA_RSP_CLOSE_CHANNEL and @ref TOA_CMD_CLOSE_CHANNEL.
 */
enum TOA_CHANNEL_CLOSE_REASONS
{
  CLOSE_REASON_NO_ERROR           = 0x00,
  /**< Clean close. No payload. */
  CLOSE_REASON_MIC_FAILURE        = 0x01,
  /**< Could not verify MIC. Payload format:
  
  Nonce value | Expected MIC
  ------------|-------------
  4 bytes     | 4 bytes
  */
  CLOSE_REASON_TKA_MISSING        = 0x02,
  /**< Did not receive TKA ack. No payload. */
  CLOSE_REASON_BROADCAST_MIC_FAIL = 0x03,
  /**< The Client received a broadcast with an invalid MIC. Payload format:
  
  Client NonceB value | Expected MIC
  --------------------|-------------
  4 bytes             | 4 bytes
  */
};


/**
 * @brief TOA SUPPORTED FEATURES BITFIELD
 */
#define TOA_FEATURE_BYTES    3


/**
 @brief TOA SUPPORTED FEATURES

BYTE 0:<br>

Bit  | Feature
-----|------------------------------------------
bit 0| TOFU : Tile Over the air Firmware Upgrade
bit 1| BDADDR : Tile BDADDR Access
bit 2| TDT : Tile Double Tap
bit 3| @ref TILE_SONG : Tile Song Feature
bit 4| @ref PPM : Low Power Clock Accuracy Control
bit 5| @ref ADV_INT : Advertising Interval Control
bit 6| OLD_TKA (obsolete) : Tile Keep Alive compatibility with FW 1.9
bit 7| TAC : Tile Accelerometer Feature (Evaluation/disabled)

BYTE 1:<br>

Bit  | Feature
-----|--------------------------------------------------------------------------
bit 0| Unused, fixed to 0 (may cause unwanted behavior in previous app versions)
bit 1| TKA : Tile Keep Alive Feature
bit 2| TDG : Tile Diagnostic Access
bit 3| TMD : Tile Mode Control
bit 4| TCU : Tile Connection Update Control
bit 5| TIME : Tile Time-keeper
bit 6| TEST : Tile TEST Feature
bit 7| TMA

BYTE 2:<br>

Bit  | Feature
-----|-------------------------------------
bit 0| TFC : Tile Flow Control - DEPRECATED
bit 1| TPS : Tile Programmable Songs
bit 2| TMF : Tile Manufacturing
bit 3| TLIL : Tile Lock-If-Lost
bit 4| TEF : Tile Experimental Framework
bit 5| TRM : Tile RSSI Monitoring
bit 6| TPC : Tile Power Control
bit 7| TSQ : Tile Song Quality
 */
enum TOA_SUPPORTED_FEATURES
{
  /* BYTE 0 */
  TOA_FEATURE_TOFU    = 0,
  TOA_FEATURE_BDADDR  = 1,
  TOA_FEATURE_TDT     = 2,
  TOA_FEATURE_SONG    = 3,
  TOA_FEATURE_PPM     = 4,
  TOA_FEATURE_ADV_INT = 5,
  TOA_FEATURE_OLD_TKA = 6,
  TOA_FEATURE_TAC     = 7,

  /* BYTE 1 */
  TOA_FEATURE_UNUSED  = 8,
  TOA_FEATURE_TKA     = 9,
  TOA_FEATURE_TDG     = 10,
  TOA_FEATURE_TMD     = 11,
  TOA_FEATURE_TCU     = 12,
  TOA_FEATURE_TIME    = 13,
  TOA_FEATURE_TEST    = 14,
  TOA_FEATURE_TMA     = 15,

  /* BYTE 2 */
  TOA_FEATURE_TFC     = 16,
  TOA_FEATURE_TPS     = 17,
  TOA_FEATURE_TMF     = 18,
  TOA_FEATURE_TLIL    = 19,
  TOA_FEATURE_TEF     = 20,
  TOA_FEATURE_TRM     = 21,
  TOA_FEATURE_TPC     = 22,
  TOA_FEATURE_TSQ     = 23,
};



/**
 * @brief TOA CLIENT SUPPORTED FEATURES BITFIELD
 */
#define TOA_CLIENT_FEATURE_BYTES 1

/**
 @brief TOA CLIENT SUPPORTED FEATURES

Bit  | Feature
-----|-----------------------------
bit 0| TDE : Can receive TDE events
bit 1| RFU
bit 2| RFU
bit 3| RFU
bit 4| RFU
bit 5| RFU
bit 6| RFU
bit 7| RFU
*/
enum TOA_CLIENT_SUPPORTED_FEATURES
{
  TOA_CLIENT_FEATURE_TDE  = 0,
};


/** @}*/


/** \defgroup TOA_COMMON Tile Over-the-air Common API
Some TOA Features only use simple and similar Read/Write Operations. <br>
When it is the case, @ref TOA_COMMON Commands and response codes and format are used.
*  @{
*/

/**
 * @brief TOA Common Command Codes
 */
enum TOA_COM_CMD
{
  TOA_COM_CMD_READ_FEATURES = 0x01,
  /**< Common Command Read Features <br>
  This Command SHALL be supported by the TOA Server if @ref TOA_COMMON scheme is suported.
  Format: there is no parameter.
  */
  TOA_COM_CMD_READ_VAL      = 0x02,
  /**< Common Command Read Value
  Format: there is no parameter.
  */
  TOA_COM_CMD_WRITE_VAL     = 0x03,
  /**< Common Command Write Value
  Format:
  @ref TOA_COM_CMD_WRITE_VAL Code | Data to write
  --------------------------------|-----------
  1 Byte                          | Size and format depends on the Feature
  */
};

/**
 * @brief TOA Common Response Codes
 */
enum TOA_COM_RSP
{
  TOA_COM_RSP_READ_FEATURES_OK  = 0x01,
  /**< Common Response Read Features OK
  Format:
  @ref TOA_COM_RSP_READ_FEATURES_OK Code | Supported features
  ---------------------------------------|-------------------
  1 Byte                                 | 1 Byte (bit 0 for @ref TOA_COM_CMD_READ_VAL / bit 1 for @ref TOA_COM_CMD_WRITE_VAL)
  */
  TOA_COM_RSP_READ_VAL_OK       = 0x02,
  /**< Common Response Read Value OK
  Format:
  @ref TOA_COM_RSP_READ_VAL_OK Code | Data Read
  ----------------------------------| ---------
  1 Byte                            | Size and format depends on the Feature
  */
  TOA_COM_RSP_WRITE_VAL_OK      = 0x03,
  /**< Common Response Write Value OK
  Format:
  @ref TOA_COM_RSP_WRITE_VAL_OK Code | Data Written
  -----------------------------------|-------------
  1 Byte                             | Size and format depends on the Feature
  */
  TOA_COM_RSP_ERROR             = 0x20,
  /**< Common error response. Format is according to @ref TOA_FEATURE_ERROR_CODES
  Format:
  @ref TOA_COM_RSP_ERROR | Offending Command | @ref TOA_FEATURE_ERROR_CODES Code | Additional Payload
  -----------------------|-------------------|-----------------------------------|-------------------
  1 Byte                 | 1 Byte            | 1 Byte                            | Up to TOA_MPS - 4 bytes
  */
};

/** @}*/


enum BDADDR_CMD
{
  BDADDR_CMD_READ_FEATURES   = 0x01,
  BDADDR_CMD_READ_VAL        = 0x02,
};

enum BDADDR_RSP
{
  BDADDR_RSP_READ_FEATURES_OK = 0x01,
  BDADDR_RSP_READ_VAL_OK      = 0x02,
};


/** \defgroup PPM Oscillator Accuracy (in PPM)
- PPM is the feature to read and write PPM accuracy
- Errors for PPM are returned as @ref TOA_RSP_ERROR_UNSUPPORTED or
@ref TOA_RSP_ERROR_PARAMETERS errors
There is no enum defined in our code for PPM values. The mapping is as follows:
Value | PPM
----- | --------
0x00  | PPM reporting is not supported
0x06  | Crystal oscillator 500 PPM accuracy.
0x07  | Crystal oscillator 250 PPM accuracy.
0x08  | Crystal oscillator 150 PPM accuracy.
0x09  | Crystal oscillator 100 PPM accuracy.
0x0a  | Crystal oscillator 75 PPM accuracy.
0x0b  | Crystal oscillator 50 PPM accuracy.
0x0c  | Crystal oscillator 30 PPM accuracy.
0x0d  | Crystal oscillator 20 PPM accuracy.
* @{
*/

/**
 * @brief Command codes for PPM
 */
enum PPM_CMD
{
  PPM_CMD_READ_FEATURES    = 0x01,
  /**< PPM Read supported features */
  PPM_CMD_READ_VAL         = 0x02,
  /**< PPM Read current value */
  PPM_CMD_WRITE_VAL        = 0x03,
  /**< PPM Write new value (1 byte) */
};

/**
 * @brief: Response codes from PPM
 */
enum PPM_RSP
{
  PPM_RSP_READ_FEATURES_OK  = 0x01,
  /**< Parameter is a 1 byte a bitfield with
        bit 0 indicating read supported and bit 1 indicating write supported */
  PPM_RSP_READ_VAL_OK       = 0x02,
  /**< Parameter is the current PPM value. It is a 1 byte value */
  PPM_RSP_WRITE_VAL_OK      = 0x03,
  /**< Parameter is the current PPM value. It is a 1 byte value */
};

/** @} */

/** \defgroup ADV_INT Advertising Interval
- ADV_INT is the feature to read and set the advertising interval.
- Legal values for the advertising interval are between 32 and 16384, inclusive.
The real interval time is approximately equal to (adv_interval)*0.625 ms, e.g.
an advertising interval of 3200 is approximately 2 seconds.
- - Errors for ADV_INT are returned as @ref TOA_RSP_ERROR_UNSUPPORTED or
@ref TOA_RSP_ERROR_PARAMETERS errors
* @{
*/

/**
 * @brief Command codes for ADV_INT
 */
enum ADV_INT_CMD
{
  ADV_INT_CMD_READ_FEATURES  = 0x01,
  /**< ADV_INT Read supported features */
  ADV_INT_CMD_READ_VAL       = 0x02,
  /**< ADV_INT Read current value */
  ADV_INT_CMD_WRITE_VAL      = 0x03,
  /**< ADV_INT Write new value (2 bytes, little Endian) */
};

/**
 * @brief: Response codes from ADV_INT
 */
enum ADV_INT_RSP
{
  ADV_INT_RSP_READ_FEATURES_OK  = 0x01,
  /**< Parameter is a 1 byte bitfield with bit 0 indicating
  read is supported and bit 1 indicating write is supported */
  ADV_INT_RSP_READ_VAL_OK       = 0x02,
  /**< Parameter is a 2 byte value of the current advertising interval,
  as specified above */
  ADV_INT_RSP_WRITE_VAL_OK      = 0x03,
  /**< Parameter is the new advertising interval (2 bytes, little Endian) */
};

/** @} */


/**
 * @brief Helper defines for dealing with CIDs
 */
#define TOA_CHANNEL_IDX(cid) ((cid) - TOA_BROADCAST_CID - 1)
#define TOA_CHANNEL_MIN_CID  (TOA_BROADCAST_CID + 1)
#define TOA_CHANNEL_MAX_CID  (TOA_BROADCAST_CID + toa->num_channels)


/**
 * @brief Seconds until a GAP disconnection is issued after all
 *        TOA channels are closed.
 */
#define TOA_DISCONNECT_TIME     5


/**
 * @brief Message packing helpers
 */

/** Extract a particular byte from an integer */
#define BYTE(a, n) (((a) >> ((n)*8)) & 0xFF)

/**
 * Convert an integer into the elements of a byte array, in little-Endian.
 * Example: uint8_t msg[] = {TOA_RSP_X, MKLE32(myUint32Var), MKLE16(myUint16Var)};
 */
#define MKLE16(x) BYTE((uint16_t)(x), 0), BYTE((uint16_t)(x), 1)
#define MKLE32(x) MKLE16((uint32_t)(x)),  MKLE16(((uint32_t)(x)) >> 16)

/**
 * Write an integer into a byte array, in little-Endian.
 */
static inline void WRLE16(uint8_t *a, const uint16_t x)
{
  a[0] = x & 0xFF;
  a[1] = (x >> 8) & 0xFF;
}

static inline void WRLE32(uint8_t *a, const uint32_t x)
{
  a[0] = x & 0xFF;
  a[1] = (x >> 8) & 0xFF;
  a[2] = (x >> 16) & 0xFF;
  a[3] = (x >> 24) & 0xFF;
}

/**
 * @brief Message unpacking helpers
 */

/** Convert little-Endian uint8_t array to integer. */
static inline uint16_t RDLE16(const uint8_t *a)
{
  return a[0]  |
    a[1] << 8;
}

static inline uint32_t RDLE32(const uint8_t *a)
{
  return ((uint32_t)a[0])       |
         ((uint32_t)a[1] << 8)  |
         ((uint32_t)a[2] << 16) |
         ((uint32_t)a[3] << 24);
}

/**
 * @brief TOA channels states bitfield
 */
enum TOA_CHANNEL_STATE_BITFIELD
{
  TOA_CHANNEL_ASSIGNED      = 0x01,
  TOA_CHANNEL_AUTHENTICATED = 0x02,
  TOA_CHANNEL_TKA_ENABLED   = 0x04,
  TOA_CHANNEL_WAIT4ACK      = 0x08,
};


/**
 * The following three macros should be used to allow TOA features to be
 * completely unlinked at link time. This is done by making everything
 * within a TOA feature static, and only exposing those static functions to
 * the outside world through externally visible function pointers. These
 * macros assist in this process, and should be used as follows:
 *   1. All externally visible functions should be declared with
 *        @ref TOA_EXTERN_DECL in the header for the TOA feature.
 *   2. Each function declared with @ref TOA_EXTERN_DECL should be implemented
 *        using @ref TOA_EXTERN_IMPL in the .c file for the feature, e.g.
 *           TOA_EXTERN_IMPL(void, tmd_process_command, (uint8_t *data, uint8_t datalen))
 *           {
 *              // Do stuff here
 *           }
 *   3. Each function declared with @ref TOA_EXTERN_DECL should be linked in
 *        the feature's register function with @ref TOA_EXTERN_LINK.
 */
#define TOA_EXTERN_DECL(ret, fname, args) extern ret (*fname)args           ///< Declare a function visible externally.
#define TOA_EXTERN_IMPL(ret, fname, args) \
  ret (*fname)args = 0; \
  static ret fname ## _linked args                                          ///< Define an externally-visible function implementation.
#define TOA_EXTERN_LINK(fname) fname = fname ## _linked                     ///< Link an externally-visible function.


extern uint8_t sca;
extern uint8_t toaRspNotif;

void toa_set_feature(uint16_t bit);
uint8_t toa_get_feature(uint16_t bit);
void toa_clear_feature(uint16_t bit);
void toa_init(void);
bool is_toa_authenticated(void);
void toa_process_command(const uint8_t* data, uint8_t datalen);
void toa_write_ok(void);
void toa_process_cccd(uint16_t cccd);
void toa_send_attempt(void);
void toa_send_connectionless_response(uint8_t *token, uint8_t response, uint8_t *data, uint8_t datalen);
void toa_send_response(uint8_t cid, uint8_t response, uint8_t *data, uint8_t datalen);
void toa_send_broadcast(uint8_t response, uint8_t *data, uint8_t datalen);
void toa_send_connectionless_response_error(uint8_t *token, uint8_t error, uint8_t command);
void toa_send_response_error(uint8_t cid, uint8_t error, uint8_t command);
void toa_send_response_error_param(uint8_t cid, uint8_t error, uint8_t command, uint8_t param);
void toa_channel_close(uint8_t cid, uint8_t reason, uint8_t *payload, uint8_t payload_length);
void toa_disconnect(void);

/**
 *@}
 **/
 
#endif  // TOA_H_
