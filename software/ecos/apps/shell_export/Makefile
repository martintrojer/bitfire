
BUILD_TAG = echo `date` foo@`hostname`
COMPILER_RELEASE = echo `gcc --version | head -1`
UNAME = `whoami | cut -c 1-2`

CC = gcc-3.3

CC_FLAGS = -g -DSHELL_DEBUG
CC_INC = -I. -I../build/install/include
LD = -L../build/install/lib -Ttarget.ld -nostdlib
TLIB = ../build/install/lib/libtarget.a ../build/install/lib/libextras.a

CC_FLAGS +=	-DBUILD_TAG=\""$(shell $(BUILD_TAG))"\" -DCOMPILER_RELEASE=\""$(shell $(COMPILER_RELEASE))"\" -DBUILD_FLAGS="\"-g"\"


TARGET = shell_export

OBJS = buildtag.o builtin_commands.o init.o shell_err.o shell_thread.o shelltask.o thread_cleanup.o thread_info.o

$(TARGET): $(OBJS) $(TLIB)
	$(CC) -o $(TARGET) $(OBJS) $(LD)

$(OBJS): %.o: %.c
	$(CC) -c $(CC_FLAGS) $(CC_INC) $< -o $@

clean:
	rm -rf $(OBJS) $(TARGET)