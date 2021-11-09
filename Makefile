###############################################################################
######################### Windows make starts here ############################
###############################################################################

ifeq ($(OS),Windows_NT)

# Name of the executable file which is created
EXEC = rpiserialterminal.exe


# Find all the C and C++ files we want to compile
# Note the single quotes around the * expressions. Make will incorrectly expand these otherwise.

CC = gcc
CCFLAGS = -Wall -Werror -Wextra -pedantic -std=c99


# The final build step.
$(EXEC): rpiserialterminal.o
	$(CC) $(CCFLAGS) rpiserialterminal.o -o $(EXEC)

rpiserialterminal.o: rpiserialterminal.c
	$(CC) $(CFLAGS) -c rpiserialterminal.c

rpiserial.o: src\rpiserial.c
	$(CC) $(CCFLAGS) -c src\rpiserial.c

clean:
	del *.o $(EXEC)

###############################################################################
########################## Unix make starts here ##############################
###############################################################################

else

# Name of the executable file which is created
EXEC = rpiserialterminal
SRC_DIR = ./src
BUILD_DIR = ./build
#[ -d $(BUILD_DIR) ] || mkdir -p $(BUILD_DIR) # Creates build directory if not exists

# checking operating system
#os:
#	echo $(OS)


# Find all the C and C++ files we want to compile
# Note the single quotes around the * expressions. Make will incorrectly expand these otherwise.
# SRCS = $(shell find $(SRC_DIR) -name '*.c')


CC = gcc
CCFLAGS = -Wall -Werror -Wextra -pedantic -std=c99 -pthread -I/src

# IDIR = ./include
# IDIR_FLAGS = $(addprefix -I,$(IDIR))


# The final build step.
#$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
#    $(CC) $(OBJS) -o $@ $(LDFLAGS)

# Build step for C source
#$(BUILD_DIR)/%.o: %.c %.h
#    mkdir -p $(dir $@)
#    $(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# The final build step.
$(EXEC): rpiserialterminal.o rpiserial.o
	$(CC) $(CCFLAGS) rpiserialterminal.o rpiserial.o -o $(EXEC)

rpiserialterminal.o: rpiserialterminal.c rpiserial.o
	$(CC) $(CFLAGS) -c rpiserialterminal.c

rpiserial.o: src/rpiserial.c src/rpiserial.h
	$(CC) $(CCFLAGS) -c src/rpiserial.c

clean:
	rm -rf *.o $(EXEC)

endif