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
define BuildObject
	@if test ! -d ${OBJDIR}; then mkdir ${OBJDIR}; fi
	$(CC) ${CFLAGS} -I${INCDIR} -c ${<} -o $@
endef

define CleanObjects
	${RM} -r ${OBJDIR}
endef

###########################
# Precondition
###########################
ifndef BASEDIR
$(error BASEDIR is not defined!!)
endif
ifndef DEBUG_SUFFIX
$(error DEBUG_SUFFIX is not defined!!)
endif

###############################
# Variables
###############################
INCDIR?=${BASEDIR}/include
SOURCES:=$(shell ls *.c)
INCLUDES:=${INCDIR}/sort.h
OBJDIR:=obj# dir to put all the intermediate object files
OBJECTS:=$(addprefix ${OBJDIR}/,$(subst .c,.o,${SOURCES}))# generate the object names and prepend the object dir to each of them.
DEUBG_SUFFIX:=d
OBJECTS_D:=$(OBJECTS:.o=${DEUBG_SUFFIX}.o)# names of debug objects, whose basename ends with a "d"


############################
# Targets
############################
${OBJECTS}:${OBJDIR}/%.o:%.c ${INCLUDES}
	$(BuildObject)

${OBJECTS_D}:${OBJDIR}/%${DEUBG_SUFFIX}.o:%.c ${INCLUDES}
	$(BuildObject)


