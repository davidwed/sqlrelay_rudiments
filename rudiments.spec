Name: rudiments
Version: 0.53
Release: 2%{?dist}
Summary: C++ class library for developing systems and applications

License: LGPLv2
URL: http://rudiments.sourceforge.net
Source0: http://downloads.sourceforge.net/%{name}/%{name}-%{version}.tar.gz

BuildRequires: pcre-devel,openssl-devel

%description
A C++ class library for developing systems and applications.  Rudiments includes
frameworks for processes, threads, clients, servers, parsers, loggers and
compilers.  It also includes data structures for buffers, arrays, linked lists
and dictionaries, and utility classes for processing text and binary data,
regular expressions, random numbers, encryption, date and time, system
information,  files, directories, file-systems, inter-process communication,
dynamic libraries, and XML.


%package devel
Summary: Development files for rudiments
Requires: %{name}%{?_isa} = %{version}-%{release}

%description devel
Development files for rudiments.


%prep
%autosetup

%build
chmod -x include/rudiments/private/permissions.h
%configure --disable-static
make %{?_smp_mflags}

%install
rm -rf %{buildroot}
make DESTDIR=%{buildroot} docdir=%{buildroot}%{_docdir}/%{name} install

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig

%files
%{_libdir}/librudiments-%{version}.so.1
%{_libdir}/librudiments-%{version}.so.1.0.0
%doc AUTHORS ChangeLog
%license COPYING
%exclude %{_libdir}/librudiments.la

%files devel
%{_includedir}/rudiments
%{_libdir}/librudiments.so
%{_bindir}/rudiments-config
%{_libdir}/pkgconfig/rudiments.pc
%{_mandir}/man1/rudiments-config*
%{_docdir}/%{name}
%exclude %{_libdir}/librudiments.la

%changelog
* Fri Sep  11 2015 David Muse <dmuse@firstworks.com> 0.53-2
- More Fedora Naming/Packaging/Review Guidelines compliance updates.

* Fri Aug  28 2015 David Muse <dmuse@firstworks.com> 0.53-1
- Fedora Naming/Packaging/Review Guidelines compliance updates.

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

