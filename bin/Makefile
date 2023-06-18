files		=	mcase

lib		= 	multi


CPPFLAGS 	+=	-I../inc -Wall -g
LDFLAGS		+=	-L../lib

ifeq "$(CURSES)" "yes"
CPPFLAGS	+=	-DNCURSES
endif

all		:	$(files)

$(files)	: % : %.cc
		$(CXX) $(CPPFLAGS) $(LDFLAGS) $< -o $@ -lmulti -lm -lncurses
#		$(CXX) $(CPPFLAGS) $< -o $@ -lm -lncurses
		
$(files)	:	../lib/libmulti.a

clean:
		rm -f $(files)
