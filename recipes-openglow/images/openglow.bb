DESCRIPTION = "OpenGlow Platform"

LICENSE = "GPLv3"

inherit core-image

IMAGE_INSTALL = " \
	packagegroup-base \
	packagegroup-core-boot \
	${CORE_IMAGE_EXTRA_INSTALL} \
	i2c-tools \
	u-boot-fw-utils \
	nano \
	python \
	python3 \
	python3-spidev \
	wpa-supplicant \
"

IMAGE_FEATURES += " ssh-server-dropbear "
