# This is _NOT_ the library release version, it's an API version.
# Please read Chapter 6 "Library interface versions" of the libtool documentation before making any modification
LIBVERSION=0:0:0

INCLUDES = $(all_includes) -I$(top_srcdir)/include
AM_CFLAGS = ${CFLAGS} -fPIC ${SYMBOL_VISIBILITY}

lib_LTLIBRARIES = libfx2mirisdr.la

libfx2mirisdr_la_SOURCES = libfx2mirisdr.c tuner_msi001.c
libfx2mirisdr_la_LDFLAGS = -version-info $(LIBVERSION)

bin_PROGRAMS         = fx2miri_sdr

fx2miri_sdr_SOURCES     = fx2miri_sdr.c
fx2miri_sdr_LDADD       = libfx2mirisdr.la
