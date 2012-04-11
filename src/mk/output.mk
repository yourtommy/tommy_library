#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# All of the C source files (*.c) are compiled
# separately into an object file (*.o).
# They are built in two modes: debug and release.
# In the debug mode an object file's basename
# ends with "d" while in the release mode
# it doesn't.
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

###########################
# Functions
##########################

# Warn that the varible not defined and 
# a defualt value is used.
# Usage: $(call MakeVar,VarName)
#define WarnVar
#  $(warning Variable $1 is not defined. Using "$($1)".)
#endef

# Error: the varible is not defined. 
# Usage: $(call ErrorVar,VarName)
define ErrorVar
  $(error Variable $1 is not defined!)
endef

define NotifySuccess
	@echo Build "$@" successfully.
endef

# BuildOutput is defined in Variables section
# because it depends on the value of OUTTYPE

define MakeOutDir
	if test ! -d ${OUTDIR}; then mkdir ${OUTDIR}; fi
endef


###############################
# Variables
###############################
##TDDO:find a way to deal with the space inside a path
ifndef OUTNAME
  #OUTNAME?="$(shell pwd)"
  #$(call WarnVar,OUTNAME)
  $(error OUTNAME is not defined!!)
endif
ifndef BASEDIR
  BASEDIR?=../..# the base of include and src
  $(call WarnVar,BASEDIR)
endif
INCDIR=${BASEDIR}/include
LIBDIR=${BASEDIR}/lib
APPDIR=${BASEDIR}/app

ifdef INPUTNAMES
  $(info Depending on ${INPUTNAME}.)
endif
#ifdef INPUTTYPE
#  INPUTTYPE=shared
#  $(call WarnVar,INPUTTYPE)
#endif
#ifneq (INPUTTYPE,static)
#  INPUTSUFFIX=.so
#else
#  INPUTSUFFIX=.a
#endif 
INPUTLIBS=$(foreach name,${INPUTNAMES},-l${name})
release:INPUTLIBS+=$(foreach name,${INPUTNAMES_SUFFIX},-l${name})
debug:INPUTLIBS+=$(foreach name,${INPUTNAMES_SUFFIX},-l${name}d)


ifndef OUTTYPE
  $(error OUTTYPE is not defined!!)
endif

VALIDTYPES=executable shared static
ifeq ($(words $(findstring ${OUTTYPE},${VALIDTYPES})),0)
  $(error OUTTYPE ${OUTTYPE} is not supported!${ISVALID})
endif

ifeq (${OUTTYPE},shared)
  define BuildOutput
	$(MakeOutDir)
	${CC} ${CFLAGS} -shared -o $@ $^
  endef
  OUTSUFFIX:=.so
  OUTDIR:=${LIBDIR}
  OUTNAME:=lib${OUTNAME}
endif
ifeq (${OUTTYPE},executable)
  define BuildOutput
	$(MakeOutDir)
	${CC} ${CFLAGS} -I${INCDIR} -L${LIBDIR} -o $@ $^ ${INPUTLIBS}
  endef
  OUTSUFFIX:=
  OUTDIR:=${APPDIR}
endif 
ifeq (${OUTTYPE},static)
  define BuildOutput
	$(MakeOutDir)
	${AR} ${CFLAGS} $@ $^
  endef
  OUTSUFFIX:=.a
  OUTDIR:=${LIBDIR}
  OUTNAME:=lib${OUTNAME}
endif 


ifndef OUTPUT_D
  OUTPUT_D?=${OUTDIR}/${OUTNAME}${DEUBG_SUFFIX}${OUTSUFFIX}
  $(call WarnVar,OUTPUT_D)
endif
ifndef OUTPUT
  OUTPUT?=${OUTDIR}/${OUTNAME}${OUTSUFFIX}
  $(call WarnVar,OUTPUT)
endif
ifndef DEBUG_SUFFIX 
  DEBUG_SUFFIX=d
  $(call WarnVar,DEBUG_SUFFIX)
endif

CFLAGS+=-Wall -W -Werror#
debug:CFLAGS+=-g#
release:CFLAGS+=-O2#

# The first target to make
.PHONY:${OUTNAME}
${OUTNAME}: release

###########################
# Generate object files
##########################
include ${BASEDIR}/src/mk/objects.mk

############################
# Targets
############################

${OUTPUT_D}: ${OBJECTS_D}
	$(BuildOutput)

${OUTPUT}: ${OBJECTS}
	$(BuildOutput)

.PHONY:debug
debug:${OUTPUT_D}
	$(NotifySuccess)

.PHONY:release
release:${OUTPUT} 
	$(NotifySuccess)

.PHONY:all
all: debug release

.PHONY:clean
clean:
	${CleanObjects}
	${RM} -r ${LIBDIR}
	${RM} -r ${APPDIR}
	$(NotifySuccess)
