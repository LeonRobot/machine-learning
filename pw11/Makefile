
##################################################################
# START_IP_HEADER						 #
# 								 #
# Written by Francois Fleuret					 #
# Contact <francois.fleuret@idiap.ch> for comments & bug reports #
# 								 #
# END_IP_HEADER							 #
##################################################################

CXXFLAGS = -ggdb3 -Wall
# LDFLAGS=  -lm -lrt -ljpeg  # not for osx
LDFLAGS=  -lm -ljpeg

ifeq ($(DEBUG),yes)
  CXXFLAGS += -DDEBUG -fno-omit-frame-pointer
endif

all: main Makefile.depend

Makefile.depend: *.h *.cc Makefile
	$(CC) $(CXXFLAGS) -M *.cc > Makefile.depend

main:	misc.o \
	rgb_image.o jpeg_misc.o \
	data_set.o \
	transfer_function.o loss_function.o \
	mlp_layer.o mlp.o \
	main.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f *.o main Makefile.depend

-include Makefile.depend
