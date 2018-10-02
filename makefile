SUFFIXES 	+= .d
NODEPS 		:= clean tags svn

# Variables Entries

NAME 	= Raytracer
TARGET  = .

COMPILER 	:= g++
BASEFLAGS 	:= -std=c++11 -msse4.1 -c -MMD -MP -Wall -Wfatal-errors -O3
EXEOPTIONS  := 
#EXEOPTIONS  := -Wl,--subsystem,windows

PROGRAMS = exe fastexe

SOURCEDIR 	:= source
OBJDIR 		:= obj
LIBDIR 		:= lib

SRCDIRS 	:= $(shell ls $(SOURCEDIR)/)
INCLUDES 	:= -I./include -I./depend
LIBS 		:= -lmingw32 -L$(LIBDIR) -lSDL2main -lSDL2  -mwindows  -Wl,--no-undefined -lm -ldinput8 -ldxguid -ldxerr8 -luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lshell32 -lversion -luuid -static-libgcc

SOURCES := $(shell find $(SOURCEDIR)/ -name '*.cpp')
OBJDUMP := $(patsubst $(SOURCEDIR)/%.cpp,%.o,$(SOURCES))

EXE_ARG := -DEXPORT_AS_EXE -D__WIN32__

OBJECTS := $(addprefix $(OBJDIR)/exe/, $(OBJDUMP))
RECIPES = $(patsubst %,$(OBJDIR)/%/%.o, $(PROGRAMS))

define mkdir_subobjs
	@echo $(OBJDIR)/$(1)
	mkdir -p $(OBJDIR)/$(1)
	for dir in $(SRCDIRS); \
	do \
		echo $(OBJDIR)/$(1)/$$dir; \
		mkdir -p $(OBJDIR)/$(1)/$$dir; \
	done
endef
define mkdir_objs
	@echo $(OBJDIR)
	mkdir -p $(OBJDIR)
endef

exe: FILE_ARG 	= $(EXE_ARG)
exe: CPPFLAGS 	= $(BASEFLAGS)
exe: SUBDIR		= exe
exe: directories $(OBJECTS)
	$(COMPILER) $(OBJECTS) $(LIBS) -o $(TARGET)/$(NAME).exe $(EXEOPTIONS)
	$(TARGET)/$(NAME).exe
	
$(RECIPES): $(SOURCEDIR)/%.cpp
	$(COMPILER) $(CPPFLAGS) $(FILE_ARG) $(INCLUDES) $< -o $@
	
directories:
	@$(call mkdir_objs)
	@$(call mkdir_subobjs,$(SUBDIR))
	
-include $(patsubst %.o,%.d,$($OBJECTS))