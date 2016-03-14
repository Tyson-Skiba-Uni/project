AC_INIT([hw_week_3],[0.1])

AC_CONFIG_AUX_DIR([config])
AC_CONFIG_MACRO_DIR([m4])	#not required but makes things neat
AC_CONFIG_HEADERS([config.h])	#not required but makes things neat

AM_INIT_AUTOMAKE
AM_SILENT_RULES([yes])		#will print only major lines such as CC and CCG

LT_INIT				#initialise lib tool

AC_PROG_CC

PKG_CHECK_MODULES([MODBUS],[libmodbus])	

AC_CONFIG_FILES([
	Makefile
	src/Makefile
])				

AC_OUTPUT 
