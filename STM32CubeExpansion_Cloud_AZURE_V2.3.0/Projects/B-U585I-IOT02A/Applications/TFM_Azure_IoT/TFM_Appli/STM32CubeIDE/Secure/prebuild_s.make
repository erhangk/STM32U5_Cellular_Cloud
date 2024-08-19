
BUILD_DIR?=Release
CPU?=STM32U585xx

phony: all

all:
	@mkdir -p $(BUILD_DIR)
	@bash -c './prebuild_s.sh  $(BUILD_DIR) $(CPU)'	
	
clean:
	@echo Done