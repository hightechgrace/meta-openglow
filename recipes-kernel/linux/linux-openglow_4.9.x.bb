# Adapted from linux-imx.inc, copyright (C) 2013, 2014 O.S. Systems Software LTDA
# Released under the MIT license (see COPYING.MIT for the terms)

require recipes-kernel/linux/linux-imx.inc

SUMMARY = "Linux kernel for OpenGlow boards"

SRC_URI = "git://github.com/boundarydevices/linux-imx6.git;branch=${SRCBRANCH} \
           file://git/arch/arm/boot/dts/openglow_std.dts \
           file://git/arch/arm/mach-imx/epit_api.c \
           file://git/drivers/dma/imx-sdma.c \
           file://git/include/linux/platform_data/dma-imx-sdma.h \
           file://git/include/linux/platform_data/epit-imx.h \
           file://defconfig \
           file://Add-DT-Prescaler-to-PWM-Driver.patch \
           file://Add-EPIT-API.patch \
"

LOCALVERSION = "-1.0.0-ga+openglow"
SRCBRANCH = "boundary-imx_4.9.x_1.0.0_ga"
SRCREV = "${AUTOREV}"
DEPENDS += "lzop-native bc-native"
COMPATIBLE_MACHINE = "openglow_std"
