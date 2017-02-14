APP_NAME = smartpa

##########compile############
PREFIX =arm-none-linux-gnueabi-
#PREFIX =
CC = $(PREFIX)gcc
CPP = $(PREFIX)g++
LD = $(PREFIX)ld
AR = $(PREFIX)ar

##########parameters#########
ifeq ($(PREFIX),)
$(warnning "--------lative--------")
CFLAGS= -I/usr/local/include 
LIBS =  -L/usr/local/lib
else
$(warnning "--------cross--------")
LIBS = -lts
endif

CPPFLAGS=
CFLAGS += -g
LIBS += -lminigui_ths -lpng -lm -ldl -lz -lpthread -lmgeff -lfreetype -lstdc++

##########target#############		
OBJS=ButtonEx.o button_ex.o register_res.o font.o sqlite3.o irdb_manager.o ir_code_send.o \
		smartpa.o desktop.o taskbar.o common.o screen_lock.o \
		volume_setting.o power_setting.o language_setting.o brightness_setting.o \
		setting.o dock_bar.o dev_type.o dev_brand.o match_code.o \
		tv.o tvbox.o iptv.o dvd.o fan.o air_condition.o air_cleaner.o water_heater.o projector.o
		
all:$(OBJS)
	$(CC) -o $(APP_NAME) $(OBJS) $(CFLAGS) $(LIBS)
clean:
	find . -name "*.o"  | xargs rm -f
	rm -rf $(APP_NAME)


##########rules##############
%.o : %.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@

	
