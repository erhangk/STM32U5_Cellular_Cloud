/*
 * Copyright (c) 2017-2019, Arm Limited. All rights reserved.
 * Portions Copyright (c) STMicroelectronics, all rights reserved
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
#include <stdint.h>
#include "stm32u5xx_hal.h"
#include "tfm_api.h"
#include "tfm_ns_interface.h"
#include "tx_api.h"

static uint8_t users = 0;
static TX_MUTEX tfm_ns_mutex;

#define COMPILER_BARRIER() __ASM volatile("" : : : "memory")

static uint8_t core_util_atomic_incr_u8(volatile uint8_t *valuePtr, uint8_t delta)
{
  COMPILER_BARRIER();
  uint8_t newValue;
  do
  {
    newValue = __LDREXB(valuePtr) + delta;
  } while (__STREXB(newValue, valuePtr));
  COMPILER_BARRIER();
  return newValue;
}

/**
  * \brief NS world, NS lock based dispatcher
  *        In Azure Iot application there are some TFM calls before 
  *        ThreadX is started.
  *        So it is needed to have different exclusion mechanisms
  *        depending if ThreadX is started or not.
  */
int32_t tfm_ns_interface_dispatch(veneer_fn fn,
                                  uint32_t arg0, uint32_t arg1,
                                  uint32_t arg2, uint32_t arg3)
{
  uint32_t ret;
  
  if (tx_thread_identify() != NULL)
  {
    /* we are in a ThreadX thread, we can use mutex */
    UINT mutex_ret = TX_MUTEX_ERROR;

    mutex_ret = tx_mutex_get(&tfm_ns_mutex, TX_WAIT_FOREVER);
  
    if (mutex_ret == TX_SUCCESS)
    {
      ret = (uint32_t)fn(arg0, arg1, arg2, arg3);
      tx_mutex_put(&tfm_ns_mutex);
    }
    else
    {
      ret = TFM_SECURE_LOCK_FAILED;
    }
  }
  else
  {
    /*  ThreadX is not started, use another locking mechanism */
    if (core_util_atomic_incr_u8(&users, 1) > 1)
    {
      while (1);
    }
    ret = (uint32_t)fn(arg0, arg1, arg2, arg3);
    users = 0;
  }
  return ret;
}

enum tfm_status_e tfm_ns_interface_init(void)
{
  uint32_t status = TX_MUTEX_ERROR;
  status =  tx_mutex_create(&tfm_ns_mutex, "tfm_ns_lock", TX_INHERIT);

  if (status != TX_SUCCESS)
  {
    return TFM_ERROR_NOT_INITIALIZED;
  }
  return TFM_SUCCESS;
}

