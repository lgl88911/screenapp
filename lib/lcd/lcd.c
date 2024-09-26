/*
 * Copyright (c) 2024, Frank
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <app/lib/lcd.h>

#include <zephyr/drivers/spi.h>
#include <zephyr/kernel.h>

#define LCD_SPI_DEV DT_ALIAS(lcd_spi)

static const struct device *spi_dev;
static struct spi_config spi_cfg;

// ... existing code ...

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

    spi_write(spi_dev, &spi_cfg, &tx_bufs);
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

    spi_write(spi_dev, &spi_cfg, &tx_bufs);
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
	/*
    res_set();
    delay(1);
    res_clear();
    delay(10);
    res_set();
    delay(120);
*/
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
    spi_dev = DEVICE_DT_GET(LCD_SPI_DEV);
    if (!device_is_ready(spi_dev)) {
        printk("SPI device not ready\n");
        return;
    }

    spi_cfg.operation = SPI_OP_MODE_MASTER | SPI_WORD_SET(8) | SPI_TRANSFER_MSB;
    spi_cfg.frequency = 10000000;  // 10 MHz, adjust as needed

    // Reset LCD
    // Implement GPIO control for reset pin here

    // Initialize LCD
    initi();
}

// ... rest of the LCD functions ...