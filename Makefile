# mCASE main Makefile

files	=	configFile \
			RATtype \
			terminalType \
			userType \
			serviceType \
			movementType \
			BStype \
			operatorType \
			businessType \
			rng \
			mtrand \
			event \
			geometry \
			userTerminal \
			basestation \
			configScenario \
			movement \
			service \
			accessSelection \
			statistics \
			scenario
#				common \
#				rng \
#				userTerminal \
#				movement \
#				baseStation \
#				relayNode \
#				service \

all_objs	=	$(addsuffix .o, $(addprefix src/, $(files)))

library		=	lib/libmulti.a

CURSES		=	@

CPPFLAGS	+=	-I./inc -Wall -g

# Flag for Valgrind
CPPFLAGS	+=  -fno-inline -O0

ifeq "$(CURSES)" "yes"
CPPFLAGS	+=	-DNCURSES
endif

.PHONY	:	bin clean tools

all		:	$(library) bin

$(library)	: 	$(all_objs)
			$(AR) r $@ $?

bin		:
			cd bin && make $(MAKECMDGOALS)
			
tools	:
			cd tools && g++ `pkg-config --cflags --libs plplotd-c++` bsDeployPlot.cc -o bsDeployPlot

clean	: 	
			rm $(targetExe) $(all_objs) $(execPos_objs) $(all_exec_objs)  $(library) -f
			cd bin && make $(MAKECMDGOALS)
			
cltxt	: 	
			rm bin/*.txt -f
			rm results/*.txt results/*.trc -f
			rm results/snapshots/* -rf
			rm results/stats/* -rf
			