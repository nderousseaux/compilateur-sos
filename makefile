CC			:= cc
TARGET		:= SoS

#Directories
SRCDIR		:= src
INCDIR		:= $(SRCDIR)/includes
OBJDIR		:= out
TARGETDIR	:= .

#Flags
TEMP_FLAGS  := -Wno-unneeded-internal-declaration -Wno-sign-compare -Wno-unused-function -Wno-type-limits#FIXME: supprimer ces flags
CFLAGS		:= -Wall -Wextra -Wall -Werror $(TEMP_FLAGS) -I$(INCDIR)

#Files
SYNTAX		:= syntaxe
GRAMMAR     := grammaire
SRCS		:= $(wildcard $(SRCDIR)/*.c) $(wildcard $(SRCDIR)/**/*.c)
OBJS		:= $(patsubst %.c, $(OBJDIR)/%.o, $(notdir $(SRCS))) $(OBJDIR)/$(GRAMMAR).tab.o $(OBJDIR)/$(SYNTAX).yy.c
INCLUDES	:= $(wildcard $(INCDIR)/*.h)


#Rules
all: copyright $(TARGETDIR)/$(TARGET) 

debug: CFLAGS += -g
debug: $(TARGETDIR)/$(TARGET)

$(TARGETDIR)/$(TARGET): $(OBJS) #On construit l'executable
	$(CC) $(CFLAGS) -o $@ $^
	rm -f $(OBJDIR)/*.c $(OBJDIR)/*.h

$(OBJDIR)/$(GRAMMAR).tab.c: $(SRCDIR)/$(GRAMMAR).yacc #On construit le .c de la grammaire
	bison -d -o $@ $<

$(OBJDIR)/$(SYNTAX).yy.c: $(SRCDIR)/$(SYNTAX).lex $(OBJDIR)/$(GRAMMAR).tab.h #On construit le .c de la syntaxe
	flex -o $@ $<

$(OBJDIR)/%.o: $(SRCDIR)/%.c #On construit le reste des .c
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJDIR)/%.o: $(SRCDIR)/**/%.c #On construit le reste (dans les sous-dossiers)
	$(CC) $(CFLAGS) -c -o $@ $<

copyright: .git logo.ans
	sh gen-copyright.sh

clean:
	rm -f $(OBJS)