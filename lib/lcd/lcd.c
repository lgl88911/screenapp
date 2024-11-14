/*
 * Copyright (c) 2024, Frank
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <app/lib/lcd.h>

#include <zephyr/drivers/spi.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/display.h>
#include <zephyr/drivers/gpio.h>

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(lcd, LOG_LEVEL_INF);

#define LCD_SPI_DEV DT_ALIAS(lcd_spi)
#define ZEPHYR_USER_NODE DT_PATH(zephyr_user)

static const struct device *spi_dev;
static struct spi_config spi_cfg;


static const struct gpio_dt_spec lcd_cs = GPIO_DT_SPEC_GET(ZEPHYR_USER_NODE, lcd_cs_gpios);
static const struct gpio_dt_spec lcd_rst = GPIO_DT_SPEC_GET(ZEPHYR_USER_NODE, lcd_rst_gpios);


static void write_command(unsigned char cmd)
{
    uint8_t tx_data[2] = {0x00, cmd};  // Command mode (0x00) + command byte
    struct spi_buf tx_buf = {
        .buf = tx_data,
        .len = sizeof(tx_data)
    };
    struct spi_buf_set tx_bufs = {
        .buffers = &tx_buf,
        .count = 1
    };

	gpio_pin_set_dt(&lcd_cs, 0);

    spi_write(spi_dev, &spi_cfg, &tx_bufs);

	gpio_pin_set_dt(&lcd_cs, 1);
}

static void write_data(unsigned char data)
{
    uint8_t tx_data[2] = {0x40, data};  // Data mode (0x40) + data byte
    struct spi_buf tx_buf = {
        .buf = tx_data,
        .len = sizeof(tx_data)
    };
    struct spi_buf_set tx_bufs = {
        .buffers = &tx_buf,
        .count = 1
    };

	gpio_pin_set_dt(&lcd_cs, 0);
    spi_write(spi_dev, &spi_cfg, &tx_bufs);
	gpio_pin_set_dt(&lcd_cs, 1);
}

// ... 之前的代码 ...

static void delay(uint32_t ms)
{
    k_msleep(ms);
}

void initi(void)
{
    // 复位 LCD
    // 注意：这里假设您已经在其他地方定义了 res 引脚的控制函数
	
    gpio_pin_set_dt(&lcd_rst, 1);
    delay(1);
    gpio_pin_set_dt(&lcd_rst, 0);
    delay(10);
    gpio_pin_set_dt(&lcd_rst, 1);
    delay(120);

	write_command(0xFF);
	write_data(0x77);
	write_data(0x01);
	write_data(0x00);
	write_data(0x00);
	write_data(0x13);

	write_command(0xEF);
	write_data(0x08);

	write_command(0xFF);
	write_data(0x77);
	write_data(0x01);
	write_data(0x00);
	write_data(0x00);
	write_data(0x10);

	write_command(0xC0);
	write_data(0x3B);
	write_data(0x00);

	write_command(0xC1);
	write_data(0x10);
	write_data(0x0C);

	write_command(0xC2);////Inversion selection
	write_data(0x31); //31-2dot ,37-Column 
	write_data(0x0A);

	write_command(0xC3); //SET RGB MODE
	write_data(0x02); //02-DE MODE ,82-HV MODE	 PCLK N
	write_data(0x00);
	write_data(0x00);

	write_command(0xCC);
	write_data(0x10);

	write_command(0xCD);
	write_data(0x08);
	//**********GAMMA SET***************//
	write_command(0xB0);
	write_data(0x40);
	write_data(0x0E);
	write_data(0x58);
	write_data(0x0E);
	write_data(0x12);
	write_data(0x08);
	write_data(0x0C);
	write_data(0x09);
	write_data(0x09);
	write_data(0x27);
	write_data(0x07);
	write_data(0x18);
	write_data(0x15);
	write_data(0x78);
	write_data(0x26);
	write_data(0xC7);

	write_command(0xB1);
	write_data(0x40);
	write_data(0x13);
	write_data(0x5B);
	write_data(0x0D);
	write_data(0x11);
	write_data(0x06);
	write_data(0x0A);
	write_data(0x08);
	write_data(0x08);
	write_data(0x26);
	write_data(0x03);
	write_data(0x13);
	write_data(0x12);
	write_data(0x79);
	write_data(0x28);
	write_data(0xC9);
	/*-----------------------------End Gamma Setting------------------------------*/
	/*------------------------End Display Control setting-------------------------*/
	/*-----------------------------Bank0 Setting  End-----------------------------*/
	/*-------------------------------Bank1 Setting--------------------------------*/
	/*--------------------- Power Control Registers Initial ----------------------*/
	write_command(0xFF);
	write_data(0x77);
	write_data(0x01);
	write_data(0x00);
	write_data(0x00);
	write_data(0x11);

	write_command(0xB0);
	write_data(0x6D);//30
	/*--------------------------------Vcom Setting--------------------------------*/
	write_command(0xB1);
	write_data(0x38);//VOCM
	/*------------------------------End Vcom Setting------------------------------*/
	write_command(0xB2);
	write_data(0x81);//84

	write_command(0xB3);
	write_data(0x80);

	write_command(0xB5);
	write_data(0x4E);//4E

	write_command(0xB7);
	write_data(0x85);

	write_command(0xB8);
	write_data(0x20);

	write_command(0xC1);
	write_data(0x78);

	write_command(0xC2);
	write_data(0x78);

	write_command(0xD0);
	write_data(0x88);
	/*--------------------End Power Control Registers Initial --------------------*/
	//********GIP SET********************///
	write_command(0xE0);
	write_data(0x00);
	write_data(0x00);
	write_data(0x02);

	write_command(0xE1);
	write_data(0x06);
	write_data(0x30);
	write_data(0x08);
	write_data(0x30);
	write_data(0x05);
	write_data(0x30);
	write_data(0x07);
	write_data(0x30);
	write_data(0x00);
	write_data(0x33);
	write_data(0x33);

	write_command(0xE2);
	write_data(0x11);
	write_data(0x11);
	write_data(0x33);
	write_data(0x33);
	write_data(0xF4);
	write_data(0x00);
	write_data(0x00);
	write_data(0x00);
	write_data(0xF4);
	write_data(0x00);
	write_data(0x00);
	write_data(0x00);

	write_command(0xE3);
	write_data(0x00);
	write_data(0x00);
	write_data(0x11);
	write_data(0x11);

	write_command(0xE4);
	write_data(0x44);
	write_data(0x44);

	write_command(0xE5);
	write_data(0x0D);
	write_data(0xF5);
	write_data(0x30);
	write_data(0xF0);
	write_data(0x0F);
	write_data(0xF7);
	write_data(0x30);
	write_data(0xF0);
	write_data(0x09);
	write_data(0xF1);
	write_data(0x30);
	write_data(0xF0);
	write_data(0x0B);
	write_data(0xF3);
	write_data(0x30);
	write_data(0xF0);

	write_command(0xE6);
	write_data(0x00);
	write_data(0x00);
	write_data(0x11);
	write_data(0x11);

	write_command(0xE7);
	write_data(0x44);
	write_data(0x44);

	write_command(0xE8);
	write_data(0x0C);
	write_data(0xF4);
	write_data(0x30);
	write_data(0xF0);
	write_data(0x0E);
	write_data(0xF6);
	write_data(0x30);
	write_data(0xF0);
	write_data(0x08);
	write_data(0xF0);
	write_data(0x30);
	write_data(0xF0);
	write_data(0x0A);
	write_data(0xF2);
	write_data(0x30);
	write_data(0xF0);

	write_command(0xE9);
	write_data(0x36);
	write_data(0x01);

	write_command(0xEB);
	write_data(0x00);
	write_data(0x01);
	write_data(0xE4);
	write_data(0xE4);
	write_data(0x44);
	write_data(0x88);
	write_data(0x40);

	write_command(0xED);
	write_data(0xFF);
	write_data(0x45);
	write_data(0x67);
	write_data(0xFA);
	write_data(0x01);
	write_data(0x2B);
	write_data(0xCF);
	write_data(0xFF);
	write_data(0xFF);
	write_data(0xFC);
	write_data(0xB2);
	write_data(0x10);
	write_data(0xAF);
	write_data(0x76);
	write_data(0x54);
	write_data(0xFF);

	write_command(0xEF);
	write_data(0x10);
	write_data(0x0D);
	write_data(0x04);
	write_data(0x08);
	write_data(0x3F);
	write_data(0x1F);

	write_command(0xFF);
	write_data(0x77);
	write_data(0x01);
	write_data(0x00);
	write_data(0x00);
	write_data(0x00);

	write_command(0x3A);
	write_data(0x70);

	write_command(0x11);
	delay(120);

	write_command(0x29);
	delay(25);
	write_command(0x35);
	write_data(0x00);
}

void EnterSleep(void)
{
    write_command(0x28);
    delay(10);
    write_command(0x10);
}

void ExitSleep(void)
{
    write_command(0x11);
    delay(120);
    write_command(0x29);
}

// ... 其他函数 ...

void lcd_init(void)
{
	int ret = 0;

    spi_dev = DEVICE_DT_GET(LCD_SPI_DEV);
    if (!device_is_ready(spi_dev)) {
        printk("SPI device not ready\n");
        return;
    }

    spi_cfg.operation = SPI_OP_MODE_MASTER | SPI_WORD_SET(8) | SPI_TRANSFER_MSB;
    spi_cfg.frequency = 10000000;  // 10 MHz, adjust as needed

	if (!gpio_is_ready_dt(&lcd_cs)) {
		printk("Error: lcd_cs device %s is not ready\n",
		       lcd_cs.port->name);
		return;
	}

	if (!gpio_is_ready_dt(&lcd_rst)) {
		printk("Error: lcd_cs device %s is not ready\n",
		       lcd_rst.port->name);
		return;
	}

    ret = gpio_pin_configure_dt(&lcd_cs, GPIO_OUTPUT_ACTIVE);
    if (ret < 0) {
        LOG_ERR("Error %d: failed to configure lcd_cs pin %d", ret, lcd_cs.pin);
        return;
    }

    ret = gpio_pin_configure_dt(&lcd_rst, GPIO_OUTPUT_ACTIVE);
    if (ret < 0) {
        LOG_ERR("Error %d: failed to configure lcd_rst pin %d", ret, lcd_rst.pin);
        return;
    }

    // Initialize LCD
    initi();
}

// ... rest of the LCD functions ...

enum corner {
	TOP_LEFT,
	TOP_RIGHT,
	BOTTOM_RIGHT,
	BOTTOM_LEFT
};

typedef void (*fill_buffer)(enum corner corner, uint8_t grey, uint8_t *buf,
			    size_t buf_size);


#ifdef CONFIG_ARCH_POSIX
static void posix_exit_main(int exit_code)
{
#if CONFIG_TEST
	if (exit_code == 0) {
		LOG_INF("PROJECT EXECUTION SUCCESSFUL");
	} else {
		LOG_INF("PROJECT EXECUTION FAILED");
	}
#endif
	posix_exit(exit_code);
}
#endif

static void fill_buffer_argb8888(enum corner corner, uint8_t grey, uint8_t *buf,
				 size_t buf_size)
{
	uint32_t color = 0;

	switch (corner) {
	case TOP_LEFT:
		color = 0x00FF0000u;
		break;
	case TOP_RIGHT:
		color = 0x0000FF00u;
		break;
	case BOTTOM_RIGHT:
		color = 0x000000FFu;
		break;
	case BOTTOM_LEFT:
		color = grey << 16 | grey << 8 | grey;
		break;
	}

	for (size_t idx = 0; idx < buf_size; idx += 4) {
		*((uint32_t *)(buf + idx)) = color;
	}
}

static void fill_buffer_rgb888(enum corner corner, uint8_t grey, uint8_t *buf,
			       size_t buf_size)
{
	uint32_t color = 0;

	switch (corner) {
	case TOP_LEFT:
		color = 0x00FF0000u;
		break;
	case TOP_RIGHT:
		color = 0x0000FF00u;
		break;
	case BOTTOM_RIGHT:
		color = 0x000000FFu;
		break;
	case BOTTOM_LEFT:
		color = grey << 16 | grey << 8 | grey;
		break;
	}

	for (size_t idx = 0; idx < buf_size; idx += 3) {
		*(buf + idx + 0) = color >> 16;
		*(buf + idx + 1) = color >> 8;
		*(buf + idx + 2) = color >> 0;
	}
}

static uint16_t get_rgb565_color(enum corner corner, uint8_t grey)
{
	uint16_t color = 0;
	uint16_t grey_5bit;

	switch (corner) {
	case TOP_LEFT:
		color = 0xF800u;
		break;
	case TOP_RIGHT:
		color = 0x07E0u;
		break;
	case BOTTOM_RIGHT:
		color = 0x001Fu;
		break;
	case BOTTOM_LEFT:
		grey_5bit = grey & 0x1Fu;
		/* shift the green an extra bit, it has 6 bits */
		color = grey_5bit << 11 | grey_5bit << (5 + 1) | grey_5bit;
		break;
	}
	return color;
}

static void fill_buffer_rgb565(enum corner corner, uint8_t grey, uint8_t *buf,
			       size_t buf_size)
{
	uint16_t color = get_rgb565_color(corner, grey);

	for (size_t idx = 0; idx < buf_size; idx += 2) {
		*(buf + idx + 0) = (color >> 8) & 0xFFu;
		*(buf + idx + 1) = (color >> 0) & 0xFFu;
	}
}

static void fill_buffer_bgr565(enum corner corner, uint8_t grey, uint8_t *buf,
			       size_t buf_size)
{
	uint16_t color = get_rgb565_color(corner, grey);

	for (size_t idx = 0; idx < buf_size; idx += 2) {
		*(uint16_t *)(buf + idx) = color;
	}
}

static void fill_buffer_mono(enum corner corner, uint8_t grey,
			     uint8_t black, uint8_t white,
			     uint8_t *buf, size_t buf_size)
{
	uint16_t color;

	switch (corner) {
	case BOTTOM_LEFT:
		color = (grey & 0x01u) ? white : black;
		break;
	default:
		color = black;
		break;
	}

	memset(buf, color, buf_size);
}

static inline void fill_buffer_mono01(enum corner corner, uint8_t grey,
				      uint8_t *buf, size_t buf_size)
{
	fill_buffer_mono(corner, grey, 0x00u, 0xFFu, buf, buf_size);
}

static inline void fill_buffer_mono10(enum corner corner, uint8_t grey,
				      uint8_t *buf, size_t buf_size)
{
	fill_buffer_mono(corner, grey, 0xFFu, 0x00u, buf, buf_size);
}

int lcd_test(void)
{
	size_t x;
	size_t y;
	size_t rect_w;
	size_t rect_h;
	size_t h_step;
	size_t scale;
	size_t grey_count;
	uint8_t bg_color;
	uint8_t *buf;
	int32_t grey_scale_sleep;
	const struct device *display_dev;
	struct display_capabilities capabilities;
	struct display_buffer_descriptor buf_desc;
	size_t buf_size = 0;
	fill_buffer fill_buffer_fnc = NULL;

	display_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));
	if (!device_is_ready(display_dev)) {
		LOG_ERR("Device %s not found. Aborting sample.",
			display_dev->name);
#ifdef CONFIG_ARCH_POSIX
		posix_exit_main(1);
#else
		return 0;
#endif
	}

	LOG_INF("Display sample for %s", display_dev->name);
	display_get_capabilities(display_dev, &capabilities);

	if (capabilities.screen_info & SCREEN_INFO_MONO_VTILED) {
		rect_w = 16;
		rect_h = 8;
	} else {
		rect_w = 2;
		rect_h = 1;
	}

	if ((capabilities.x_resolution < 3 * rect_w) ||
	    (capabilities.y_resolution < 3 * rect_h) ||
	    (capabilities.x_resolution < 8 * rect_h)) {
		rect_w = capabilities.x_resolution * 40 / 100;
		rect_h = capabilities.y_resolution * 40 / 100;
		h_step = capabilities.y_resolution * 20 / 100;
		scale = 1;
	} else {
		h_step = rect_h;
		scale = (capabilities.x_resolution / 8) / rect_h;
	}

	rect_w *= scale;
	rect_h *= scale;

	if (capabilities.screen_info & SCREEN_INFO_EPD) {
		grey_scale_sleep = 10000;
	} else {
		grey_scale_sleep = 100;
	}

	buf_size = rect_w * rect_h;

	if (buf_size < (capabilities.x_resolution * h_step)) {
		buf_size = capabilities.x_resolution * h_step;
	}

	switch (capabilities.current_pixel_format) {
	case PIXEL_FORMAT_ARGB_8888:
		bg_color = 0xFFu;
		fill_buffer_fnc = fill_buffer_argb8888;
		buf_size *= 4;
		break;
	case PIXEL_FORMAT_RGB_888:
		bg_color = 0xFFu;
		fill_buffer_fnc = fill_buffer_rgb888;
		buf_size *= 3;
		break;
	case PIXEL_FORMAT_RGB_565:
		bg_color = 0xFFu;
		fill_buffer_fnc = fill_buffer_rgb565;
		buf_size *= 2;
		break;
	case PIXEL_FORMAT_BGR_565:
		bg_color = 0xFFu;
		fill_buffer_fnc = fill_buffer_bgr565;
		buf_size *= 2;
		break;
	case PIXEL_FORMAT_MONO01:
		bg_color = 0xFFu;
		fill_buffer_fnc = fill_buffer_mono01;
		buf_size = DIV_ROUND_UP(DIV_ROUND_UP(
			buf_size, NUM_BITS(uint8_t)), sizeof(uint8_t));
		break;
	case PIXEL_FORMAT_MONO10:
		bg_color = 0x00u;
		fill_buffer_fnc = fill_buffer_mono10;
		buf_size = DIV_ROUND_UP(DIV_ROUND_UP(
			buf_size, NUM_BITS(uint8_t)), sizeof(uint8_t));
		break;
	default:
		LOG_ERR("Unsupported pixel format. Aborting sample.");
#ifdef CONFIG_ARCH_POSIX
		posix_exit_main(1);
#else
		return 0;
#endif
	}

	buf = k_malloc(buf_size);

	if (buf == NULL) {
		LOG_ERR("Could not allocate memory. Aborting sample.");
#ifdef CONFIG_ARCH_POSIX
		posix_exit_main(1);
#else
		return 0;
#endif
	}

	(void)memset(buf, bg_color, buf_size);

	buf_desc.buf_size = buf_size;
	buf_desc.pitch = capabilities.x_resolution;
	buf_desc.width = capabilities.x_resolution;
	buf_desc.height = h_step;

	for (int idx = 0; idx < capabilities.y_resolution; idx += h_step) {
		/*
		 * Tweaking the height value not to draw outside of the display.
		 * It is required when using a monochrome display whose vertical
		 * resolution can not be divided by 8.
		 */
		if ((capabilities.y_resolution - idx) < h_step) {
			buf_desc.height = (capabilities.y_resolution - idx);
		}
		display_write(display_dev, 0, idx, &buf_desc, buf);
	}

	buf_desc.pitch = rect_w;
	buf_desc.width = rect_w;
	buf_desc.height = rect_h;

	fill_buffer_fnc(TOP_LEFT, 0, buf, buf_size);
	x = 0;
	y = 0;
	display_write(display_dev, x, y, &buf_desc, buf);

	fill_buffer_fnc(TOP_RIGHT, 0, buf, buf_size);
	x = capabilities.x_resolution - rect_w;
	y = 0;
	display_write(display_dev, x, y, &buf_desc, buf);

	fill_buffer_fnc(BOTTOM_RIGHT, 0, buf, buf_size);
	x = capabilities.x_resolution - rect_w;
	y = capabilities.y_resolution - rect_h;
	display_write(display_dev, x, y, &buf_desc, buf);

	display_blanking_off(display_dev);

	grey_count = 0;
	x = 0;
	y = capabilities.y_resolution - rect_h;

	while (1) {
		fill_buffer_fnc(BOTTOM_LEFT, grey_count, buf, buf_size);
		display_write(display_dev, x, y, &buf_desc, buf);
		++grey_count;
		k_msleep(grey_scale_sleep);
#if CONFIG_TEST
		if (grey_count >= 1024) {
			break;
		}
#endif
	}

#ifdef CONFIG_ARCH_POSIX
	posix_exit_main(0);
#endif
	return 0;
}
