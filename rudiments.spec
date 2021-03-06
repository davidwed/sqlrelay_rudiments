Name: rudiments
Version: 1.0.7
Release: 1%{?dist}
Summary: C++ class library for developing systems and applications

# Library source code is LGLPv2.
License: LGPLv2
URL: http://rudiments.sourceforge.net
Source0: http://downloads.sourceforge.net/%{name}/%{name}-%{version}.tar.gz

BuildRequires: libedit-devel, pcre-devel, openssl-devel, libcurl-devel, krb5-devel

%description
A C++ class library for developing systems and applications.  Rudiments includes
frameworks for processes, threads, clients, servers, loggers and compilers.  It
also includes data structures for buffers, arrays, linked lists and
dictionaries, and utility classes for processing text and binary data, regular
expressions, random numbers, encryption, date and time, system information,
files, directories, file-systems, inter-process communication, dynamic
libraries, and XML.


%package devel
License: LGPLv2
Summary: Development files for rudiments
Requires: %{name}%{?_isa} = %{version}-%{release}, libedit-devel, openssl-devel, libcurl-devel, krb5-devel

%description devel
Development files for rudiments.

%package doc
# Documentation is GPLv2 except for example code in the documentation.
# Example code is FSFUL.
License: GPLv2 and FSFUL
Summary: Documentation for rudiments
BuildArch: noarch

%description doc
Documentation for rudiments.


%prep
%autosetup -p1

%build
chmod -x include/rudiments/private/permissions.h
%configure --disable-static
make %{?_smp_mflags}

%install
make install DESTDIR=%{buildroot}

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig

%files
%{_libdir}/librudiments.so.*
%doc AUTHORS ChangeLog
%exclude %{_libdir}/librudiments.la
%if 0%{?fedora}
%license COPYING
%exclude %{_datadir}/licenses/rudiments
%else
%{_datadir}/licenses/rudiments
%endif

%files devel
%{_includedir}/rudiments
%{_libdir}/librudiments.so
%{_bindir}/rudiments-config
%{_libdir}/pkgconfig/rudiments.pc
%{_mandir}/man1/rudiments-config*

%files doc
%{_docdir}/%{name}

%changelog
* Wed Sep 08 2017 David Muse <david.muse@firstworks.com> - 1.0.7-1
- Updated to version 1.0.7.

* Fri Sep 01 2017 David Muse <david.muse@firstworks.com> - 1.0.6-1
- Updated to version 1.0.6.

* Fri May 12 2017 David Muse <david.muse@firstworks.com> - 1.0.5-1
- Updated to version 1.0.5.

* Tue Feb 21 2017 David Muse <david.muse@firstworks.com> - 1.0.4-1
- Added fedora dist-tag conditional.
- Replaced readline with libedit.

* Wed Jan 25 2017 David Muse <david.muse@firstworks.com> - 1.0.4-1
- Updated to version 1.0.4.

* Fri Jan 20 2017 David Muse <david.muse@firstworks.com> - 1.0.3-1
- Updated to version 1.0.3.
- Removed call to make uninstall-license.
- Added a directive to exclude licenses installed by make install.
- Added Requires to devel subpackage.
- Escaped percent sign in changelog.

* Sun Dec 25 2016 David Muse <david.muse@firstworks.com> - 1.0.2-1
- Updated to version 1.0.2.
- Replaced buildroot macro in comment with (buildroot).

* Fri Dec 23 2016 David Muse <david.muse@firstworks.com> - 1.0.1-1
- Updated to version 1.0.1.
- Removed "parsers" from description text.
- Removed Requires from doc package.

* Tue Jul 26 2016 David Muse <dmuse@firstworks.com> - 0.56.0-1
- Added readline dependency

* Wed Dec 16 2015 David Muse <dmuse@firstworks.com> - 0.55.0-1
- Added krb5 dependencies
- Updated shared library name to reflect libtool versioning scheme.

* Tue Oct 06 2015 David Muse <dmuse@firstworks.com> - 0.54-1
- Added libcurl dependencies
- Removed mention of test/build-script licenses
- Made doc rpm noarch

* Fri Sep 25 2015 David Muse <dmuse@firstworks.com> - 0.53-4
- License updates

* Thu Sep 17 2015 Jens Lody <fedora@jenslody.de> - 0.53-3
- Added doc-package
- Removed obsolete cleaning of buildroot
- Removed docdir= from make install

* Fri Sep  11 2015 David Muse <dmuse@firstworks.com> - 0.53-2
- More Fedora Naming/Packaging/Review Guidelines compliance updates.
- configure patch for fedora 23

* Fri Aug  28 2015 David Muse <dmuse@firstworks.com> - 0.53-1
- Fedora Naming/Packaging/Review Guidelines compliance updates.

* Fri Jan  31 2003 David Muse <dmuse@firstworks.com>
- Made it so it could be distributed with rudiments.
- Added devel.

* Fri May  3 2002 Matthias Saou <matthias.saou@est.une.marmotte.net>
- Rebuilt against Red Hat Linux 7.3.
- Added the %%{?_smp_mflags} expansion.

* Mon Apr 15 2002 Matthias Saou <matthias.saou@est.une.marmotte.net> 0.24-fr1
- Update to 0.24 at last.

* Wed May 16 2001 Matthias Saou <matthias.saou@est.une.marmotte.net>
- Initial RPM release.

