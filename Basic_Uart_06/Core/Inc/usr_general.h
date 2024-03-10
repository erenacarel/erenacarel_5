#ifndef __USR_GENERAL_H
#define __USR_GENERAL_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include "usart.h"
#include "gpio.h"

#define _io  static
#define _iov static volatile

#include "usr_system.h"
#include "usr_lib_log.h"
#include "usr_lib_sensor.h"

#endif