AC_INIT([threads],[0.1])        #filename threads

AC_CONFIG_AUX_DIR([config])
AC_CONFIG_MACRO_DIR([m4])	#not required but makes things neat
AC_CONFIG_HEADERS([config.h])	#not required but makes things neat

AM_INIT_AUTOMAKE
AM_SILENT_RULES([yes])		#will print only major lines such as CC and CCG

LT_INIT				#initialise lib tool

AC_PROG_CC

AC_SEARCH_LIBS([pthread_create],pthread, [],       #[] action if found is to do nothing
	AC_MSG_ERROR([Pthread Library Not Found]))  #Warn that no pthread library is found

PKG_CHECK_MODULES([MODBUS],[libmodbus])	

AC_CONFIG_FILES([
	Makefile
	src/Makefile
])				

AC_OUTPUT 
