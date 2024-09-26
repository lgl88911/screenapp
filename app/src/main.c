/*
 * Copyright (c) 2024 Frank Li
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <app/lib/lcd.h>

#include <app_version.h>

LOG_MODULE_REGISTER(main, CONFIG_APP_LOG_LEVEL);



int main(void)
{
	printk("Startup\r\n");
	lcd_init();
	while (1) {
		k_sleep(K_MSEC(100));
	}

	return 0;
}

