/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id$
 *
 * Copyright (C) 2007 Dave Chapman
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ****************************************************************************/
#include "config.h"
#include "ata_idle_notify.h"
#include "system.h"
#include <string.h>
#include "thread.h"
#include "led.h"
#include "disk.h"
#include "storage.h"
#include "panic.h"
#include "usb.h"
#include "ftl-target.h"

/* for compatibility */
long last_disk_activity = -1;

/** static, private data **/ 
static bool initialized = false;

static long next_yield = 0;
#define MIN_YIELD_PERIOD 2000

/* API Functions */

void nand_led(bool onoff)
{
    led(onoff);
}

int nand_read_sectors(IF_MD2(int drive,) unsigned long start, int incount,
                     void* inbuf)
{
    return ftl_read(start, incount, inbuf);
}

int nand_write_sectors(IF_MD2(int drive,) unsigned long start, int count,
                      const void* outbuf)
{
    return ftl_write(start, count, outbuf);
}

void nand_spindown(int seconds)
{
    (void)seconds;
}

void nand_sleep(void)
{
}

void nand_spin(void)
{
}

void nand_enable(bool on)
{
    (void)on;
}

void nand_get_info(IF_MD2(int drive,) struct storage_info *info)
{
    (void)info;
}

long nand_last_disk_activity(void)
{
    return 0;
}

int nand_init(void)
{
    if (ftl_init()) return 1;

    initialized = true;
    return 0;
}

#ifdef CONFIG_STORAGE_MULTI
int nand_num_drives(int first_drive)
{
    /* We don't care which logical drive number(s) we have been assigned */
    (void)first_drive;
    
    return 1;
}
#endif
