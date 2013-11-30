CC = gcc
CFLAGS = 
CFLAGS_PRIV = -Wall -g3 -pedantic -std=c99 -I${INC} -D_XOPEN_SOURCE=700 -D__PROFILING__=1 -DDEBUG=1 $(CFLAGS) -I$(MEMORY_MANAGEMENT_LIB)/include
SHAREDFLAGS=
SHAREDFLAGS_PRIV=-fPIC -shared $(SHAREDFLAGS)
LDFLAGS = 
LDFLAGS_PRIV = -L$(MEMORY_MANAGEMENT_LIB)/lib -lmemorymanagement -Llib -l${COBJ} -lpthread $(LDFLAGS)

WLFLAGS=-Wl,-rpath,lib$(COBJ).so.$(COBJMAJORVERSION)
BIN = bin
INC = include
OBJ = obj
SRC = src
MKDIR = mkdir

DOC = doc
LIB = lib
TEST = test
MKDIR = mkdir

MEMORY_MANAGEMENT_LIB=../memorymanagement

COBJ = cobj

COMMUNICATION = communication

LD_LIBRARY_PATH := $(shell echo $$LD_LIBRARY_PATH):$(LIB)

UNAME := $(shell uname)
ifeq ($(UNAME), Linux)
STRIP = strip --strip-unneeded
#LD_LIBRARY_PATH := $(LD_LIBRARY_PATH):$(OPENSSL)/lib
endif

ifeq ($(UNAME), Darwin)
STRIP = strip -X -x -S
endif


.PHONY: all directories compileall runall clean cleanall


#.SUFFIXES:            # Delete the default suffixes
#.SUFFIXES: .c .o .h           # Delete the default suffixes

# +------------+
# | Target all |
# +------------+

all: directories compileall

# +------------+
# | Target lib |
# +------------+

COBJSTATIC = ${LIB}/lib${COBJ}.a
libcobj : directories libmemorymanagement $(COBJSTATIC)

COBJMAJORVERSION=0
COBJMINORVERSION=1
COBJRELEASENUMBER=1
COBJSONAME = ${LIB}/lib${COBJ}.so.$(COBJMAJORVERSION)
COBJREALNAME = ${LIB}/lib${COBJ}.so.$(COBJMAJORVERSION).$(COBJMINORVERSION).$(COBJRELEASENUMBER)
COBJSHARED = ${LIB}/lib${COBJ}.so
FIRSTLINK = $(patsubst $(LIB)/%,%,$(COBJREALNAME))
SECONDLINK = $(patsubst $(LIB)/%,%,$(COBJSONAME))
libcobjshared : directories $(COBJSHARED)
	@ln -s $(FIRSTLINK) $(COBJSONAME) 
	@ln -s $(SECONDLINK) $(COBJSHARED) 

TESTS = $(patsubst $(TEST)/%.c,$(BIN)/%,$(wildcard $(TEST)/*.c))
tests : directories libcobj $(TESTS)
	@for test in ${TESTS}; do \
		echo "**** Testing $$test"; \
	echo "---- end of ${TESTSTRING}"; \
	done
	@#	./"$$test" \

test% : $(BIN)/test%
	@echo "**** Testing $@";
	@$<
	@echo "end of $@";


valgrind% : $(BIN)/test%
	@valgrind  --track-origins=yes --leak-check=full --show-reachable=yes $<

doc : $(DOC)/html/index.html

$(DOC)/html/index.html : 
	@cd $(DOC); /usr/bin/env doxygen


# +--------------------+
# | Target directories |
# +--------------------+

directories: ${OBJ} ${BIN} ${LIB} 

${OBJ}:
	${MKDIR} -p $@
${BIN}:
	${MKDIR} -p $@
${LIB}:
	${MKDIR} -p $@

# +----------------------------+
# | Target compilation with -c |
# +----------------------------+

${OBJ}/%.o : ${SRC}/%.c
	$(CC) -c -o $@ $< ${CFLAGS_PRIV} $(SHAREDFLAGS_PRIV)

${OBJ}/%.o : ${TEST}/%.c
	$(CC) -c -o $@ $< ${CFLAGS_PRIV}

${BIN}/% : ${OBJ}/%.o
	${CC} -o $@ $< ${LDFLAGS_PRIV}


${LIB}/lib${COBJ}.a : $(patsubst ${SRC}/%.c,${OBJ}/%.o,$(wildcard ${SRC}/*.c))
	${AR} r ${LIB}/lib${COBJ}.a $?

${LIB}/lib${COBJ}.so : $(COBJSONAME)

${LIB}/lib${COBJ}.so.$(COBJMAJORVERSION) : $(COBJREALNAME)

${LIB}/lib${COBJ}.so.$(COBJMAJORVERSION).$(COBJMINORVERSION).$(COBJRELEASENUMBER) : $(patsubst ${SRC}/%.c,${OBJ}/%.o,$(wildcard ${SRC}/*.c))
	$(CC) $(CFLAGS_PRIV) $(SHAREDFLAGS_PRIV) $(WLFLAGS) -o $@  $? -lc -lpthread
	$(STRIP) $@


#${BIN}/${TESTSTRING} : libcobj $(OBJ)/${TESTSTRING}.o 
#	${CC} -o $@ $< ${LDFLAGS_PRIV}
#

libmemorymanagement : $(MEMORY_MANAGEMENT_LIB)/lib/libmemorymanagement

$(MEMORY_MANAGEMENT_LIB)/lib/libmemorymanagement : $(MEMORY_MANAGEMENT_LIB)/lib/libmemorymanagement.a
	$(MAKE) -C $(MEMORY_MANAGEMENT_LIB) 

# +-------------------+
# | Target compileall |
# +-------------------+

compileall: libcobj

# +---------------+
# | Target runall |
# +---------------+

runall: compileall
	$(MAKE) tests

# +--------------+
# | Target clean |
# +--------------+

clean:
	-rm -f ${OBJ}/* ${BIN}/* $(LIB)/*

# +-----------------+
# | Target cleanall |
# +-----------------+

cleanall: clean
	-rmdir ${OBJ} ${BIN} $(LIB) 2>/dev/null || exit 0
	-rm -f $(DOC)/html/search/*
	-rmdir $(DOC)/html/search
	-rm -f $(DOC)/{html,latex}/*
	-rmdir $(DOC)/{html,latex}
	-rm -f ${INC}/*~ ${SRC}/*~ *~ ${TEST}/*~

