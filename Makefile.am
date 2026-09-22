
AM_CFLAGS = $(URING_CFLAGS)
smoothd_LDADD = $(URING_LIBS)

bin_PROGRAMS = smoothd smooth
smoothd_SOURCES = smoothd.c
smooth_SOURCES = smooth.c

EXTRA_DIST = smoothd.spec
dist_man_MANS = smooth.1 smoothd.8

CLEANFILES = smooth.1 smoothd.8

smooth.1: smooth$(EXEEXT)
	$(builddir)/smooth$(EXEEXT) --help | txt2man -d 1 -s 1 -t "${PACKAGE_NAME}" -r "${PACKAGE_NAME}-${PACKAGE_VERSION}" > $@

smoothd.8: smoothd$(EXEEXT)
	$(builddir)/smoothd$(EXEEXT) --help | txt2man -d 1 -s 8 -t "${PACKAGE_NAME}" -r "${PACKAGE_NAME}-${PACKAGE_VERSION}" > $@

