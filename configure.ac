dnl
dnl $Id: configure.ac,v 1.3 2006/06/25 09:36:57 z_hossain Exp $
dnl
dnl A dockable window framework for Fast Light Tool Kit (FLTK).
dnl
dnl Copyright (C) 2006 by MD. Z. Hossain
dnl
dnl This library is free software; you can redistribute it and/or
dnl modify it under the terms of the GNU Library General Public
dnl License as published by the Free Software Foundation; either
dnl version 2 of the License, or (at your option) any later version.
dnl
dnl This library is distributed in the hope that it will be useful,
dnl but WITHOUT ANY WARRANTY; without even the implied warranty of
dnl MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
dnl Library General Public License for more details.
dnl
dnl You should have received a copy of the GNU Library General Public
dnl License along with this library; if not, write to the Free Software
dnl Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
dnl USA.
dnl
dnl Please report all bugs and problems to: 
dnl
dnl             z_hossain@users.sf.net
dnl

AC_PREREQ([2.59])

AC_INIT([fltkdock], [0.07], [z_hossain@users.sf.net])
AC_CONFIG_HEADERS([config.h:config-h.in])
AC_CONFIG_SRCDIR([src/DockWindow.cxx])

AC_SUBST(FLTKDOCK_VERSION)
AC_SUBST(FLTKDOCK_API_VERSION)
FLTKDOCK_VERSION=0.07
FLTKDOCK_API_VERSION=1

AC_PROG_CXX
AC_PROG_RANLIB

AC_CHECK_HEADERS([string.h])
AC_CHECK_HEADERS([math.h])

AC_C_CONST

dnl check fltk					=====
AC_PATH_PROG(FLTK_CONFIG,fltk2-config)

AC_SUBST(FLDFLAGS)
AC_SUBST(FCXXFLAGS)
AC_SUBST(FVERSION)
if test x"$FLTK_CONFIG" != x; then
   FLDFLAGS=`fltk2-config --ldflags`
   FCXXFLAGS=`fltk2-config --cxxflags`
   FVERSION=`fltk2-config --api-version`
else
   AC_MSG_ERROR([FLTK API version 2.0 is required to compile])
fi

dnl check version				=====
if test "$FVERSION" != "2.0"; then
   AC_MSG_ERROR([FLTK API version 2.0 is required to compile])
fi

dnl check ar					=====
AC_PATH_PROG(AR,ar)
if test x"$AR" = x; then
	AC_MSG_ERROR([library archiver is required to build the library])
fi

dnl check options				=====
AC_ARG_ENABLE(debug,    [  --enable-debug          turn on debugging (default=no)])
AC_SUBST(DEBUGFLAG)
if test x$enable_debug = xyes; then
    DEBUGFLAG="-g"
else
    DEBUGFLAG="-O"
fi

AC_SUBST(DSOCOMMAND)
AC_SUBST(DSOSUFFIX)
AC_SUBST(DSOSYMLINK)

AC_ARG_ENABLE(shared, [  --enable-shared          compile shared libraries (default=no)])

if test x$enable_shared = xyes; then
	DSOSUFFIX=".so.$FLTKDOCK_API_VERSION"
    DSOSYMLINK=".so"

	case $(uname) in
	Darwin*)
	    DSOSYMLINK=".dylib"
	    DSOSUFFIX=".$FLTKDOCK_API_VERSION.dylib"
	    DSOCOMMAND="ld $DSOFLAGS -dylib /usr/lib/dylib1.o -lc"
	    ;;
	SunOS* | UNIX_S*)
	    DSOCOMMAND="\$(CXX) -h \$@ \$(LDLIBS) -G $DEBUGFLAG -o"
	    ;;
	Linux* | *BSD*)
		DSOCOMMAND="\$(CXX) \$(LDLIBS) -shared $DEBUGFLAG -o"
		;;
	CYGWIN*)
		AC_MSG_WARN(Shared libraries are not supported under CygWin.)
		;;
	*)
		AC_MSG_WARN(Modify Makefile according to your machine.  Trying -shared option with compiler.)
		DSOCOMMAND="\$(CXX) -Wl,-soname,\$@ \$(LDLIBS) -shared $DEBUGFLAG -o"
		;;
	esac
else
    DSOCOMMAND="echo"
    DSOSUFFIX=""
    DSOSYMLINK=""
fi

if test x$prefix = xNONE; then
    prefix=/usr/local
fi

if test x$exec_prefix = xNONE; then
    exec_prefix="\${prefix}"
fi

AC_OUTPUT(Makefile src/Makefile test/Makefile)
