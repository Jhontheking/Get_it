# Ajusta estq variable a tu ruta de Aleph-w
ALEPH = /home/jesus/Aleph-w

# Ajusta esta variable a la ruta de clang++
CLANGPATH = /usr/bin

CXX = $(CLANGPATH)/clang++-3.6

WARN = -Wall -Wextra -Wcast-align -Wno-sign-compare -Wno-write-strings \
       -Wno-parentheses -Wno-invalid-source-encoding

FLAGS = -g -O0 -std=c++14 $(WARN)

INCLUDE = -I. -I $(ALEPH) 

LIBS = -L $(ALEPH) \
       -lAleph -lm -lgsl -lgslcblas -lgmp -lmpfr -lasprintf -lpthread -lc

all: test

test: test.C file_address_tree.H 
	$(CXX) $(FLAGS) $(INCLUDE) $@.C -o $@ $(LIBS)

clean:
	$(RM) test *~