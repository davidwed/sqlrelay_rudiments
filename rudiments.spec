Summary: C++ class library for daemons, clients and servers.
Name: rudiments
Version: 0.26.3
Release: 1
License: LGPL
Group: Development/Libraries
Source0: %{name}-%{version}.tar.gz
BuildRoot: %{_tmppath}/%{name}-root

%description
Rudiments is an Open Source C++ class library providing base classes
for things such as daemons, clients and servers, and wrapper classes
for the standard C functions for things like such as regular
expressions, semaphores and signal handling.


%package devel
Summary: Libraries and header files for developing with rudiments.
Group: Development/Libraries

%description devel
Libraries and header files for developing with rudiments.


%package doc
Summary: Documentation for rudiments.
Group: Development/Libraries

%description doc
Documentation for rudiments.


%prep
%setup -q

%build
%configure
make %{?_smp_mflags}

%install
rm -rf %{buildroot}
#%makeinstall bindir=%{buildroot}%{_bindir} libdir=%{buildroot}%{_libdir} incdir=%{buildroot}%{_includedir} docdir=%{buildroot}%{_docdir}/%{name}-%{version}
%makeinstall DESTDIR=%{buildroot} docdir=%{buildroot}%{_docdir}/%{name}-%{version}

%post
/sbin/ldconfig

%postun
/sbin/ldconfig

%clean
rm -rf %{buildroot}

%files
%defattr(-, root, root)
%{_libdir}/librudiments-*.so.*

%files devel
%{_includedir}/rudiments
%{_libdir}/*.so
%{_libdir}/*.a
%{_libdir}/*.la
%{_bindir}/rudiments-config

%files doc
%{_docdir}/%{name}-%{version}

%changelog
* Fri Jan  31 2003 David Muse <dmuse@firstworks.com>
- Made it so it could be distributed with rudiments.
- Added devel.

* Fri May  3 2002 Matthias Saou <matthias.saou@est.une.marmotte.net>
- Rebuilt against Red Hat Linux 7.3.
- Added the %{?_smp_mflags} expansion.

* Mon Apr 15 2002 Matthias Saou <matthias.saou@est.une.marmotte.net> 0.24-fr1
- Update to 0.24 at last.

* Wed May 16 2001 Matthias Saou <matthias.saou@est.une.marmotte.net>
- Initial RPM release.

