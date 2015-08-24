Name: rudiments
Version: 0.53
Release: 1%{?dist}
Summary: C++ utility and base class library

License: LGPLv2
URL: http://rudiments.sourceforge.net
Source0: http://downloads.sourceforge.net/projects/%{name}/%{name}-%{version}.tar.gz

BuildRequires: pcre-devel,openssl-devel

%description
C++ utility and base class library.


%package devel
Summary: Libraries and header files for developing with rudiments
Requires: %{name}%{?_isa} = %{version}-%{release}

%description devel
Libraries and header files for developing with rudiments.


%prep
%setup -q

%build
chmod -x include/rudiments/private/permissions.h
%configure
make %{?_smp_mflags}

%install
rm -rf %{buildroot}
make DESTDIR=%{buildroot} docdir=%{buildroot}%{_docdir}/%{name} install

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig

%clean
rm -rf %{buildroot}

%files
%defattr(-, root, root)
%{_libdir}/librudiments-*.so.*
%{_datadir}/licenses/%{name}
%doc AUTHORS ChangeLog

%files devel
%defattr(-, root, root)
%{_includedir}/rudiments
%{_libdir}/*.so
%{_libdir}/*.a
%{_libdir}/*.la
%{_bindir}/rudiments-config
%{_libdir}/pkgconfig/rudiments.pc
%{_mandir}/man1/rudiments-config*
%{_docdir}/%{name}

%changelog
* Fri Jan  31 2003 David Muse <dmuse@firstworks.com> 0.53-1
- Fedora compliance updates.

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

