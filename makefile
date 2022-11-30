CC			:= gcc
TARGET		:= SoS

#Directories
SRCDIR		:= src
INCDIR		:= $(SRCDIR)/includes
OBJDIR		:= out
TARGETDIR	:= .

#Flags
CFLAGS		:= -Wall -Wextra -Werror -I$(INCDIR)

#Files
SRCS		:= $(wildcard $(SRCDIR)/*.c)
OBJS		:= $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRCS))
INCLUDES	:= $(wildcard $(INCDIR)/*.h)


#Rules
all: copyright $(TARGETDIR)/$(TARGET) 

debug: CFLAGS += -g
debug: $(TARGETDIR)/$(TARGET)

$(TARGETDIR)/$(TARGET): $(OBJS) #On construit l'executable
	$(CC) $(CFLAGS) -o $@ $^

$(OBJDIR)/%.o: $(SRCDIR)/%.c #On construit le reste
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS)

copyright: .git logo.ans
	sh gen-copyright.sh