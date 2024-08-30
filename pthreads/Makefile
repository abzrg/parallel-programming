CC = clang
CFLAGS = -Wall -Wextra -std=c11 -pedantic -pthread -g

# (Reentrancy Flags):
# These flags define macros that ensure reentrant versions of standard library functions are used.
# This is sometimes necessary on older systems or specific platforms to ensure thread safety.
CFLAGS += -D_REENTRANT
CFLAGS += -D_THREAD_SAFE

# (Thread Sanitizer):
# Enables ThreadSanitizer, a tool that detects data races, deadlocks, and other threading-related
# errors. This is highly useful for debugging and ensuring thread safety in multithreaded programs.
# This flag also requires linking with the runtime library (-ltsan), which is usually handled
# automatically by the compiler.
CFLAGS += -fsanitize=thread
# Use `MallocNanoZone=0 /path/to/exe` to avoid the following error when sanitizers are enabled
#   roll_dice(47618,0x1fb508c00) malloc: nano zone abandoned due to inability to reserve vm space.

# (Stack Protection):
# Adds stack protection to help detect stack overflows, which can be especially useful in
# multi-threaded programs where each thread has its own stack. The -strong variant provides enhanced
# protection over -all without significant performance overhead.
CFLAGS += -fstack-protector-strong

# (Keep Frame Pointers):
# Keeps the frame pointer in the call stack, which is useful for debugging and profiling tools like
# gdb or perf. This is typically a good flag for development builds.
CFLAGS += -fno-omit-frame-pointer

# # (Static Analysis): ONLY GCC
# # Enables GCC's static analysis tool to catch potential bugs related to threading, like data races or
# # potential deadlocks. Note that this may increase compilation time.
# CFLAGS += -fanalyzer

BUILDDIR=build
SRCS = $(wildcard *.c)
TARGETS = $(addprefix $(BUILDDIR)/,$(patsubst %.c,%,$(SRCS)))

all: $(TARGETS)

$(BUILDDIR)/%: %.c
	$(CC) $(CFLAGS) $< $(LDLIBS) -o $@

clean:
	@$(RM) -fv $(TARGETS)

