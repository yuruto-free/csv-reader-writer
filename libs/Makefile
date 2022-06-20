COMPILER = gcc
ARCHIVER = ar
CFLAGS   = -O2 -MMD -MP -Wall -Wextra
LDFLAGS  = -lm
INCLUDE  = -I../include
TARGET   = libcsvapi.a
SRCDIR   = .
OBJDIR   = ./obj
SOURCES  = $(wildcard $(SRCDIR)/*.c)
OBJECTS  = $(addprefix $(OBJDIR)/, $(notdir $(SOURCES:.c=.o)))
DEPENDS  = $(OBJECTS:.o=.d)

.PHONY: all clean

all: $(TARGET)

-include $(DEPENDS)

$(TARGET): $(OBJECTS)
	$(ARCHIVER) -rcs $@ $^

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@if [ ! -e $(OBJDIR) ]; then mkdir -p $(OBJDIR); fi
	$(COMPILER) -o $@ $(CFLAGS) $(INCLUDE) -c $< $(LDFLAGS)

clean:
	rm -rf $(OBJECTS) $(DEPENDS) $(TARGET) $(OBJDIR)
