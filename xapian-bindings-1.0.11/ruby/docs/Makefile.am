## Process this file with automake to produce Makefile.in

exampledatadir = $(docdir)/ruby/examples
dist_exampledata_DATA = examples/simpleexpand.rb examples/simpleindex.rb \
 examples/simplesearch.rb examples/simplematchdecider.rb

docdatadir = $(docdir)/ruby
dist_docdata_DATA = index.html
EXTRA_DIST = rdocs

install-data-local:
	$(mkinstalldirs) $(DESTDIR)$(docdatadir)
	@test -f rdocs/index.html || cd "$(srcdir)"; \
	  for dir in `find rdocs -type d -print` ; do \
	    echo " $(mkinstalldirs) '$(DESTDIR)$(docdatadir)/$$dir'"; \
	    $(mkinstalldirs) "$(DESTDIR)$(docdatadir)/$$dir"; \
	  done ; \
	  for f in `find rdocs -type f -print` ; do \
	    echo " $(INSTALL_DATA) '$$f' '$(DESTDIR)$(docdatadir)/$$f'"; \
	    $(INSTALL_DATA) "$$f" "$(DESTDIR)$(docdatadir)/$$f"; \
	  done

uninstall-local:
	$(mkinstalldirs) $(DESTDIR)$(docdatadir)
	@test -f rdocs/index.html || cd "$(srcdir)"; \
	  for f in `find rdocs -type f -print` ; do \
	    echo " rm -f '$(DESTDIR)$(docdatadir)/$$f'"; \
	    rm -f "$(DESTDIR)$(docdatadir)/$$f"; \
	  done

if DOCUMENTATION_RULES
BUILT_SOURCES = rdocs/index.html

rdocs: rdocs/index.html

rdocs/index.html: ../xapian.rb
	rm -rf rdocs
	$(RDOC) --op rdocs $(srcdir)/../xapian.rb
endif
