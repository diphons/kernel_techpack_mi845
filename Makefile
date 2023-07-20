ifneq ($(filter y, $(CONFIG_MACH_XIAOMI_E10) $(CONFIG_BOARD_E10)),)
obj-$(CONFIG_NEW_LEDS) += leds/
obj-$(CONFIG_INPUT) += input/
obj-$(CONFIG_SND_SOC) += amplifier/
obj-y += display/
obj-y += gps/
obj-y += mfd/
obj-$(CONFIG_REGULATOR) += regulator/
else
ccflags-y := -Wno-unused-function
obj-y := stub.o
endif
