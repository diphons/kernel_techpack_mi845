# auto-detect subdirs
ifeq ($(CONFIG_ARCH_SDM845), y)
include $(srctree)/techpack/mi845/audio/config/sdm845auto.conf

# Use USERINCLUDE when you must reference the UAPI directories only.
USERINCLUDE     += \
                -I$(srctree)/techpack/mi845/audio/include/uapi \
                -I$(srctree)/techpack/mi845/audio/include

# Use LINUXINCLUDE when you must reference the include/ directory.
# Needed to be compatible with the O= option
LINUXINCLUDE    += \
                -I$(srctree)/techpack/mi845/audio/include/uapi \
                -I$(srctree)/techpack/mi845/audio/include

LINUXINCLUDE    += \
                -include $(srctree)/techpack/mi845/audio/config/sdm845autoconf.h
obj-y += soc/
obj-y += dsp/
obj-y += ipc/
obj-y += asoc/
endif
