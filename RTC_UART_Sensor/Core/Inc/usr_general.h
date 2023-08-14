#ifndef __USR_GENERAL_H
#define __USR_GENERAL_H

#include "stdbool.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "main.h"
#include "dma.h"
#include "usart.h"
#include "rtc.h"
#include "gpio.h"

#define _io static
#define _iov static volatile

#include "usr_system.h"
#include "usr_rtc.h"

#endif

