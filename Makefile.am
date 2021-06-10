## Process this file with automake to produce Makefile.in

SUBDIRS = src

dist_doc_DATA = \
	README \
	COPYING \
	AUTHORS \
	ChangeLog \
	INSTALL \
	NEWS

## Remove doc directory on uninstall

uninstall-local:
	-rm -r $(docdir)
