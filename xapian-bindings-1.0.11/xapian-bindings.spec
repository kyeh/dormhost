# RedHat-style .spec file for Xapian
# xapian-bindings.spec.  Generated from xapian-bindings.spec.in by configure.
#
# If you don't want to try building packages for all the supported bindings,
# you can disable particular bindings by passing these options to rpmbuild:
#
#	--without csharp	Disable C# bindings
#	--without php		Disable PHP bindings
#	--without python	Disable Python bindings
#	--without tcl8		Disable Tcl8 bindings

%if 0%{?!_without_python:1}
%define python_ver %(%{__python} -c "import sys; print sys.version[:3]")
%endif

%if 0%{?!_without_php:1}
%define php_extension_dir %(php-config --extension-dir)
%endif

Summary: Bindings for the Xapian Probabilistic Information Retrieval Library
Name: xapian-bindings
Version: 1.0.11
Release: 1
License: GPL
Vendor: xapian.org
Group: Development/Libraries
URL: http://xapian.org/
Requires: xapian-core-libs = %{version}
%if 0%{?!_without_python:1}
BuildRequires: python-devel >= 2.2
%endif
%if 0%{?!_without_php:1}
BuildRequires: php-devel >= 4.3
%endif
%if 0%{?!_without_tcl8:1}
BuildRequires: tcl-devel >= 8.1
%endif
%if 0%{?!_without_csharp:1}
BuildRequires: mono-devel >= 1.1 
%endif
BuildRequires: autoconf automake libtool
Source0: http://oligarchy.co.uk/xapian/%{version}/%{name}-%{version}.tar.gz
Buildroot: %{_tmppath}/%{name}-%{version}-%{release}-root

%description
Xapian is an Open Source Probabilistic Information Retrieval Library. It
offers a highly adaptable toolkit that allows developers to easily add advanced
indexing and search facilities to applications. This package provides the
files needed for scripts which use Xapian.

%if 0%{?!_without_python:1}
%package python
Group: Development/Libraries
Summary: Files needed for developing Python scripts which use Xapian.
Requires: %{name} = %{version}, python >= %{python_ver}

%description python
Xapian is an Open Source Probabilistic Information Retrieval framework. It
offers a highly adaptable toolkit that allows developers to easily add advanced
indexing and search facilities to applications. This package provides the
files needed for developing Python scripts which use Xapian.
%endif

%if 0%{?!_without_php:1}
%package php
Group: Development/Libraries
Summary: Files needed for developing PHP scripts which use Xapian.
Requires: %{name} = %{version}, php

%description php
Xapian is an Open Source Probabilistic Information Retrieval framework. It
offers a highly adaptable toolkit that allows developers to easily add advanced
indexing and search facilities to applications. This package provides the
files needed for developing PHP scripts which use Xapian.
%endif

%if 0%{?!_without_tcl8:1}
%package tcl8
Group: Development/Libraries
Summary: Files needed for developing TCL scripts which use Xapian.
Requires: %{name} = %{version}, tcl

%description tcl8
Xapian is an Open Source Probabilistic Information Retrieval framework. It
offers a highly adaptable toolkit that allows developers to easily add advanced
indexing and search facilities to applications. This package provides the
files needed for developing TCL scripts which use Xapian.
%endif

%if 0%{?!_without_csharp:1}
%package csharp
Group: Development/Libraries
Summary: Files needed for developing C# applications which use Xapian.
Requires: %{name} = %{version}, mono-core

%description csharp
Xapian is an Open Source Probabilistic Information Retrieval framework. It
offers a highly adaptable toolkit that allows developers to easily add advanced
indexing and search facilities to applications. This package provides the
files needed for developing C# applications which use Xapian.
%endif

%prep
%setup -q -n %{name}-%{version}

%build
# FC6 (at least) has a patched libtool which knows not to set rpath for
# /usr/lib64, which upstream libtool fails to do currently.  We can drop
# this "autoreconf --force" and the "BuildRequires:" for the autotools
# once upstream libtool is fixed.  Note: this overwrites INSTALL, but
# that doesn't matter here as we don't package it.
autoreconf --force
# Only build the bindings we want.
%configure %{?!_without_python:--with-python} %{?!_without_php:--with-php} %{?!_without_tcl8:--with-tcl} %{?!_without_csharp:--with-csharp}
make

%install
[ "%{buildroot}" != "/" ] && rm -rf %{buildroot}
mkdir -p %{buildroot}
# Installation of some files does not obey prefix, so use make install with
# DESTDIR instead of makeinstall.
make install DESTDIR=%{buildroot}
# Move the docs to the right places.
%{?!_without_python: mv %{buildroot}%{_datadir}/doc/%{name}/python %{buildroot}%{_datadir}/doc/%{name}-python-%{version}}
%{?!_without_php: mv %{buildroot}%{_datadir}/doc/%{name}/php %{buildroot}%{_datadir}/doc/%{name}-php-%{version}}
%{?!_without_php: [ -d "%{buildroot}%{_datadir}/php5" ] && mv %{buildroot}%{_datadir}/php5 %{buildroot}%{_datadir}/php}
%{?!_without_php: [ -d "%{buildroot}%{_datadir}/php4" ] && mv %{buildroot}%{_datadir}/php4 %{buildroot}%{_datadir}/php}
%{?!_without_tcl8: mv %{buildroot}%{_datadir}/doc/%{name}/tcl8 %{buildroot}%{_datadir}/doc/%{name}-tcl8-%{version}}
%{?!_without_csharp: mv %{buildroot}%{_datadir}/doc/%{name}/csharp %{buildroot}%{_datadir}/doc/%{name}-csharp-%{version}}

%clean
[ "%{buildroot}" != "/" ] && rm -rf %{buildroot}

%files
%defattr(-, root, root)
# There are no common files at the moment
%doc AUTHORS ChangeLog COPYING NEWS README

%if 0%{?!_without_python:1}
%files python
%defattr(-, root, root)
%{_libdir}/python%{python_ver}/site-packages/_xapian.so
%{_libdir}/python%{python_ver}/site-packages/xapian.py
%{_libdir}/python%{python_ver}/site-packages/xapian.pyc
%ghost %{_libdir}/python%{python_ver}/site-packages/xapian.pyo
%doc %{_datadir}/doc/%{name}-python-%{version}
%endif

%if 0%{?!_without_php:1}
%files php
%defattr(-, root, root)
%{php_extension_dir}/xapian.so
%{_datadir}/php/xapian.php
%doc %{_datadir}/doc/%{name}-php-%{version}
%endif

%if 0%{?!_without_tcl8:1}
%files tcl8
%defattr(-, root, root)
%{_datadir}/xapian%{version}
%doc %{_datadir}/doc/%{name}-tcl8-%{version}
%endif

%if 0%{?!_without_csharp:1}
%files csharp
%defattr(-, root, root)
%{_libdir}/XapianSharp.la
%{_libdir}/XapianSharp.so
%{_libdir}/mono/XapianSharp
%{_libdir}/mono/gac/XapianSharp
%doc %{_datadir}/doc/%{name}-csharp-%{version}
%endif
