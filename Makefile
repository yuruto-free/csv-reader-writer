COMPILER = gcc
CFLAGS   = -O2 -MMD -MP -Wall -Wextra
LDFLAGS  = -lm
LIBDIR   = ./libs
LIBS     = $(LIBDIR)/libcsvapi.a
INCLUDE  = -I./include
TARGET   = target
SRCDIR   = ./src
OBJDIR   = ./obj
SOURCES  = $(wildcard $(SRCDIR)/*.c)
OBJECTS  = $(addprefix $(OBJDIR)/, $(notdir $(SOURCES:.c=.o)))
DEPENDS  = $(OBJECTS:.o=.d)

.PHONY: all clean run

all: $(TARGET)

run: $(TARGET)
	./$(TARGET) sample.csv

-include $(DEPENDS)

$(TARGET): $(OBJECTS) $(LIBS)
	$(COMPILER) -o $@ $^ $(LDFLAGS)

$(LIBS):
	make COMPILER="$(COMPILER)" LDFLAGS="$(LDFLAGS)" -C $(LIBDIR)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@if [ ! -e $(OBJDIR) ]; then mkdir -p $(OBJDIR); fi
	$(COMPILER) -o $@ $(CFLAGS) $(INCLUDE) -c $<

clean:
	make clean -C $(LIBDIR)
	rm -rf $(OBJECTS) $(DEPENDS) $(TARGET) $(OBJDIR)
	rm -rf output.csv
	rm -f *~
