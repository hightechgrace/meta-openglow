/**
 * pic_api.c
 *
 * PIC userspace API handlers.
 *
 * Copyright (C) 2018 Scott Wiederhold <s.e.wiederhold@gmail.com>
 * Portions Copyright (C) 2015-2018 Glowforge, Inc. <opensource@glowforge.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "pic_private.h"
#include "device_attr.h"
#include "openglow.h"

static ssize_t pic_read_register_ascii(struct device *dev, enum pic_register reg, char *buf)
{
  struct i2c_client *client = to_i2c_client(dev);
  ssize_t value = pic_read_one_register(client, reg);
  if (value >= 0) {
    return scnprintf(buf, PAGE_SIZE, "%hu\n", value);
  } else {
    return value;
  }
}


static ssize_t pic_write_register_ascii(struct device *dev, enum pic_register reg, const char *buf, size_t count)
{
  struct i2c_client *client = to_i2c_client(dev);
  pic_value new_value = 0;
  int ret;
  /* Error if the entire buffer is used; we need a null-terminator */
  if (count >= PAGE_SIZE) { return -E2BIG; }

  if (sscanf(buf, "%hu", &new_value) != 1) {
    return -EINVAL;
  }
  ret = pic_write_one_register(client, reg, new_value);
  return (ret >= 0) ? count : ret;
}

#define DEFINE_READONLY_REG_ATTR(name, reg) \
  static ssize_t name##_show(struct device *dev, struct device_attribute *attr, char *buf) { \
    return pic_read_register_ascii(dev, reg, buf); } \
  static DEVICE_ATTR(name, S_IRUSR, name##_show, NULL)

#define DEFINE_READWRITE_REG_ATTR(name, reg) \
  static ssize_t name##_show(struct device *dev, struct device_attribute *attr, char *buf) { \
    return pic_read_register_ascii(dev, reg, buf); } \
  static ssize_t name##_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count) { \
    return pic_write_register_ascii(dev, reg, buf, count); } \
  static DEVICE_ATTR(name, S_IRUSR|S_IWUSR, name##_show, name##_store)

/** Single register attributes (ASCII decimal values) */
DEFINE_READONLY_REG_ATTR(ATTR_PIC_ID, PIC_REG_ID);
DEFINE_READONLY_REG_ATTR(ATTR_PIC_VERSION, PIC_REG_VERSION);
DEFINE_READONLY_REG_ATTR(ATTR_WATER_TEMP_1, PIC_REG_WATER_THERM1);
DEFINE_READONLY_REG_ATTR(ATTR_WATER_TEMP_2, PIC_REG_WATER_THERM2);
DEFINE_READONLY_REG_ATTR(ATTR_TEC_TEMP, PIC_REG_TEC_THERM);
DEFINE_READONLY_REG_ATTR(ATTR_PWR_TEMP, PIC_REG_PWR_THERM);
DEFINE_READONLY_REG_ATTR(ATTR_LID_IR_1, PIC_REG_LID_IR_DET1);
DEFINE_READONLY_REG_ATTR(ATTR_LID_IR_2, PIC_REG_LID_IR_DET2);
DEFINE_READONLY_REG_ATTR(ATTR_LID_IR_3, PIC_REG_LID_IR_DET3);
DEFINE_READONLY_REG_ATTR(ATTR_LID_IR_4, PIC_REG_LID_IR_DET4);
DEFINE_READONLY_REG_ATTR(ATTR_HV_CURRENT, PIC_REG_HV_ISENSE);
DEFINE_READONLY_REG_ATTR(ATTR_HV_VOLTAGE, PIC_REG_HV_VSENSE);
DEFINE_READWRITE_REG_ATTR(ATTR_X_CURRENT, PIC_REG_STEP_DAC_X);
DEFINE_READWRITE_REG_ATTR(ATTR_Y_CURRENT, PIC_REG_STEP_DAC_Y);
DEFINE_READWRITE_REG_ATTR(ATTR_LID_LED, PIC_REG_LID_LED);
DEFINE_READWRITE_REG_ATTR(ATTR_BUTTON_LED_1, PIC_REG_BUTTON_LED1);
DEFINE_READWRITE_REG_ATTR(ATTR_BUTTON_LED_2, PIC_REG_BUTTON_LED2);
DEFINE_READWRITE_REG_ATTR(ATTR_BUTTON_LED_3, PIC_REG_BUTTON_LED3);
DEFINE_READONLY_REG_ATTR(ATTR_DAC1_ADC, PIC_REG_DAC1_ADC);
DEFINE_READONLY_REG_ATTR(ATTR_DAC2_ADC, PIC_REG_DAC2_ADC);
DEFINE_READONLY_REG_ATTR(ATTR_FVR_ADC, PIC_REG_FVR_ADC);
DEFINE_READONLY_REG_ATTR(ATTR_PIC_TEMP, PIC_REG_TEMP_ADC);


static struct attribute *pic_attrs[] = {
  DEV_ATTR_PTR(ATTR_PIC_ID),
  DEV_ATTR_PTR(ATTR_PIC_VERSION),
  DEV_ATTR_PTR(ATTR_WATER_TEMP_1),
  DEV_ATTR_PTR(ATTR_WATER_TEMP_2),
  DEV_ATTR_PTR(ATTR_TEC_TEMP),
  DEV_ATTR_PTR(ATTR_PWR_TEMP),
  DEV_ATTR_PTR(ATTR_LID_IR_1),
  DEV_ATTR_PTR(ATTR_LID_IR_2),
  DEV_ATTR_PTR(ATTR_LID_IR_3),
  DEV_ATTR_PTR(ATTR_LID_IR_4),
  DEV_ATTR_PTR(ATTR_HV_CURRENT),
  DEV_ATTR_PTR(ATTR_HV_VOLTAGE),
  DEV_ATTR_PTR(ATTR_X_CURRENT),
  DEV_ATTR_PTR(ATTR_Y_CURRENT),
  DEV_ATTR_PTR(ATTR_LID_LED),
  DEV_ATTR_PTR(ATTR_BUTTON_LED_1),
  DEV_ATTR_PTR(ATTR_BUTTON_LED_2),
  DEV_ATTR_PTR(ATTR_BUTTON_LED_3),
  DEV_ATTR_PTR(ATTR_DAC1_ADC),
  DEV_ATTR_PTR(ATTR_DAC2_ADC),
  DEV_ATTR_PTR(ATTR_FVR_ADC),
  DEV_ATTR_PTR(ATTR_PIC_TEMP),
  NULL
};

const struct attribute_group pic_attr_group = {
  .attrs = pic_attrs
};
