/*
 * Copyright (C) 2010 - 2018 Novatek, Inc.
 * Copyright (C) 2019 XiaoMi, Inc.
 *
 * $Revision: 47247 $
 * $Date: 2019-07-10 10:41:36 +0800 (Wed, 10 Jul 2019) $
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 */
#ifndef 	_LINUX_NVT_TOUCH_H
#define		_LINUX_NVT_TOUCH_H

#include <linux/delay.h>
#include <linux/i2c.h>
#include <linux/input.h>
#include <linux/spi/spi-geni-qcom.h>
#include <linux/uaccess.h>

#ifdef CONFIG_HAS_EARLYSUSPEND
#include <linux/earlysuspend.h>
#endif

#include "nt36xxx_mem_map.h"

#define PINCTRL_STATE_ACTIVE		"pmx_ts_active"
#define PINCTRL_STATE_SUSPEND		"pmx_ts_suspend"
#define PINCTRL_STATE_RELEASE		"pmx_ts_release"

#define NVT_DEBUG 0

//---GPIO number---
#define NVTTOUCH_RST_PIN 980
#define NVTTOUCH_INT_PIN 943


//---INT trigger mode---
//#define IRQ_TYPE_EDGE_RISING 1
//#define IRQ_TYPE_EDGE_FALLING 2
#define INT_TRIGGER_TYPE IRQF_TRIGGER_RISING


//---I2C driver info.---
#define NVT_I2C_NAME "NVT-ts"
#define I2C_BLDR_Address 0x01
#define I2C_FW_Address 0x01
#define I2C_HW_Address 0x62

#if NVT_DEBUG
#define NVT_LOG(fmt, args...)    pr_err("[%s] %s %d: " fmt, NVT_I2C_NAME, __func__, __LINE__, ##args)
#else
#define NVT_LOG(fmt, args...)    pr_info("[%s] %s %d: " fmt, NVT_I2C_NAME, __func__, __LINE__, ##args)
#endif
#define NVT_ERR(fmt, args...)    pr_err("[%s] %s %d: " fmt, NVT_I2C_NAME, __func__, __LINE__, ##args)

//---Input device info.---
#define NVT_TS_NAME "NVTCapacitiveTouchScreen"


//---Touch info.---
#define TOUCH_DEFAULT_MAX_WIDTH 1080
#define TOUCH_DEFAULT_MAX_HEIGHT 2246
#define TOUCH_MAX_FINGER_NUM 10
#define TOUCH_KEY_NUM 0
#if TOUCH_KEY_NUM > 0
extern const uint16_t touch_key_array[TOUCH_KEY_NUM];
#endif
#define TOUCH_FORCE_NUM 1000

/* Enable only when module have tp reset pin and connected to host */
#define NVT_TOUCH_SUPPORT_HW_RST 0

//---Customerized func.---
#define NVT_TOUCH_PROC 1
#define NVT_TOUCH_EXT_PROC 1

#ifdef CONFIG_TOUCHSCREEN_NT36XXX_MPCTRL
#define NVT_TOUCH_MP 1
#else
#define NVT_TOUCH_MP 0
#endif

#define MT_PROTOCOL_B 1
#define WAKEUP_GESTURE 1
#if WAKEUP_GESTURE
extern const uint16_t gesture_key_array[];
#endif
#define BOOT_UPDATE_FIRMWARE 0
#define BOOT_UPDATE_FIRMWARE_NAME "novatek_nt36672_e10.fw"

#define NVT_LOCKDOWN_SIZE	8

#define NVT_TOUCH_COUNT_DUMP
#ifdef NVT_TOUCH_COUNT_DUMP
#define TOUCH_COUNT_FILE_MAXSIZE 50
#endif

struct nvt_config_info {
	u8 tp_vendor;
	u8 tp_color;
	u8 tp_hw_version;
	const char *nvt_cfg_name;
	const char *nvt_cfg_name_n;
	const char *nvt_cfg_set;
	const char *nvt_limit_name;
#ifdef NVT_TOUCH_COUNT_DUMP
	const char *clicknum_file_name;
#endif
};

struct nvt_ts_data {
	struct i2c_client *client;
	struct input_dev *input_dev;
	struct work_struct nvt_work;
	struct delayed_work nvt_fwu_work;
	struct regulator *vddio_reg;
	struct regulator *lab_reg;
	struct regulator *ibb_reg;
	struct nvt_config_info *config_array;
	struct attribute_group attrs;
	struct proc_dir_entry *tp_selftest_proc;
	struct dentry *debugfs;
	struct pinctrl *ts_pinctrl;
	struct pinctrl_state *pinctrl_state_active;
	struct pinctrl_state *pinctrl_state_suspend;
	const char *vddio_reg_name;
	const char *lab_reg_name;
	const char *ibb_reg_name;
	const char *fw_name;

	int result_type;
	u8 lockdown_info[NVT_LOCKDOWN_SIZE];
	uint16_t addr;
	int8_t phys[32];
	const struct i2c_device_id *id;
#if defined(CONFIG_DRM_PANEL)
	struct notifier_block drm_notif;
#elif defined(CONFIG_FB)
	struct notifier_block fb_notif;
#elif defined(CONFIG_HAS_EARLYSUSPEND)
	struct early_suspend early_suspend;
#endif
	uint8_t fw_ver;
	uint8_t x_num;
	uint8_t y_num;
	uint16_t abs_x_max;
	uint16_t abs_y_max;
	uint8_t max_touch_num;
	uint8_t max_button_num;
	uint32_t int_trigger_type;
	int32_t irq_gpio;
	uint32_t irq_flags;
	int32_t reset_gpio;
	uint32_t reset_flags;
	int32_t reset_tddi;
	struct mutex lock;
	struct mutex mdata_lock;
	const struct nvt_ts_mem_map *mmap;
	uint8_t carrier_system;
	uint16_t nvt_pid;
	uint8_t xbuf[1025];
	struct mutex xbuf_lock;
	bool irq_enabled;

	int gesture_enabled;
	int glove_enabled;
	int charger_enabled;
	int stylus_enabled;
	int cover_enabled;
	int grip_enabled;
	int dbclick_count;
	size_t config_array_size;
	int current_index;
	bool dev_pm_suspend;
	struct work_struct suspend_work;
	struct work_struct resume_work;
	struct workqueue_struct *event_wq;
	struct completion dev_pm_suspend_completion;
	struct proc_dir_entry *input_proc;
#ifdef NVT_TOUCH_COUNT_DUMP
	struct class *nvt_tp_class;
	struct device *nvt_touch_dev;
	bool dump_click_count;
	char *current_clicknum_file;
#endif
	bool palm_sensor_changed;
	bool palm_sensor_switch;
	bool gamemode_enabled;
	struct mutex reg_lock;
};

#if WAKEUP_GESTURE
struct mi_mode_switch {
	struct nvt_ts_data *nvt_data;
	unsigned char mode;
	struct work_struct switch_mode_work;
};
#endif

#if NVT_TOUCH_PROC
struct nvt_flash_data{
	rwlock_t lock;
	struct i2c_client *client;
};
#endif

typedef enum {
	RESET_STATE_INIT = 0xA0,// IC reset
	RESET_STATE_REK,	// ReK baseline
	RESET_STATE_REK_FINISH,	// baseline is ready
	RESET_STATE_NORMAL_RUN,	// normal run
	RESET_STATE_MAX  = 0xAF
} RST_COMPLETE_STATE;

typedef enum {
    EVENT_MAP_HOST_CMD                      = 0x50,
    EVENT_MAP_HANDSHAKING_or_SUB_CMD_BYTE   = 0x51,
    EVENT_MAP_RESET_COMPLETE                = 0x60,
    EVENT_MAP_FWINFO                        = 0x78,
    EVENT_MAP_PROJECTID                     = 0x9A,
} I2C_EVENT_MAP;

//---extern structures---
extern struct nvt_ts_data *ts;

//---extern functions---
extern void nvt_bootloader_reset(void);
extern void nvt_sw_reset_idle(void);
extern int32_t nvt_check_fw_reset_state(RST_COMPLETE_STATE check_reset_state);
extern int32_t nvt_get_fw_info(void);
extern int32_t nvt_clear_fw_status(void);
extern int32_t nvt_check_fw_status(void);
int32_t nvt_set_pocket_palm_switch(uint8_t pocket_palm_switch);
extern int32_t nvt_set_page(uint16_t i2c_addr, uint32_t addr);
extern void nvt_stop_crc_reboot(void);
extern int32_t nvt_get_lockdown_info(char *lockdata);

/*******************************************************
Description:
	Novatek touchscreen i2c read function.
return:
	Executive outcomes. 2---succeed. -5---I/O error
*******************************************************/
static inline int32_t CTP_I2C_READ(struct i2c_client *client, uint16_t address, uint8_t *buf, uint16_t len)
{
	struct i2c_msg msgs[2];
	int32_t ret = -1;
	int32_t retries = 0;

	mutex_lock(&ts->xbuf_lock);

	msgs[0].flags = !I2C_M_RD;
	msgs[0].addr  = address;
	msgs[0].len   = 1;
	msgs[0].buf   = &buf[0];

	msgs[1].flags = I2C_M_RD;
	msgs[1].addr  = address;
	msgs[1].len   = len - 1;
	msgs[1].buf   = ts->xbuf;

	while (retries < 5) {
		ret = i2c_transfer(client->adapter, msgs, 2);
		if (ret == 2)	break;
		retries++;
		msleep(20);
		NVT_ERR("error, retry=%d\n", retries);
	}

	if (unlikely(retries == 5)) {
		NVT_ERR("error, ret=%d\n", ret);
		ret = -EIO;
	}

	memcpy(buf + 1, ts->xbuf, len - 1);

	mutex_unlock(&ts->xbuf_lock);

	return ret;
}

/*******************************************************
Description:
	Novatek touchscreen i2c write function.
return:
	Executive outcomes. 1---succeed. -5---I/O error
*******************************************************/
static inline int32_t CTP_I2C_WRITE(struct i2c_client *client, uint16_t address, uint8_t *buf, uint16_t len)
{
	struct i2c_msg msg;
	int32_t ret = -1;
	int32_t retries = 0;

	mutex_lock(&ts->xbuf_lock);

	msg.flags = !I2C_M_RD;
	msg.addr  = address;
	msg.len   = len;
	memcpy(ts->xbuf, buf, len);
	msg.buf   = ts->xbuf;

	while (retries < 5) {
		ret = i2c_transfer(client->adapter, &msg, 1);
		if (ret == 1)	break;
		retries++;
		msleep(20);
		NVT_ERR("error, retry=%d\n", retries);
	}

	if (unlikely(retries == 5)) {
		NVT_ERR("error, ret=%d\n", ret);
		ret = -EIO;
	}

	mutex_unlock(&ts->xbuf_lock);

	return ret;
}

#endif /* _LINUX_NVT_TOUCH_H */
