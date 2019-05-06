Name:  fujitsu-library
Version:  1.0.0
Release:   1%{?dist} // it generates rpm as fujitsu-library-1.0.0-1.fc22.noarch.rpm  (Name-Version-Relaese)
Summary:    Creating rpm packages for Fujitsu Library

License:    Morpho
AutoReqProv:    no // To rectify the usage of third party lib on target machine
requires: libusbx systemd-devel
Source0:    fujitsu-library.tar.gz

BuildArch:    noarch // No architecture dependency
%define _binaries_in_noarch_packages_terminate_build   0

%description
Rpm package contains library files of Fujitsu Library module.
%package devel
requires: fujitsu-library
summary: Developement package for Fujitsu Library.
%description devel
Rpm package contains library and header files of Fujitsu Library.
%prep
%setup -q -c

%install
mkdir -p -m 777 %{buildroot}/opt/morpho/include/printer
mkdir -p -m 777 %{buildroot}/opt/morpho/etc/ld.so.conf.d
mkdir -p -m 777 %{buildroot}/opt/morpho/lib/printer

cp -r /home/mitesh/WorkSpace/Source_Code/web-access-2.0/FujitsuPrinter/include/*.h %{buildroot}/opt/morpho/include/printer
cp /home/mitesh/WorkSpace/Source_Code/web-access-2.0/FujitsuPrinter/build/fujitsu.conf %{buildroot}/opt/morpho/etc/ld.so.conf.d
cp -r /home/mitesh/WorkSpace/Source_Code/web-access-2.0/FujitsuPrinter/build/lib/* %{buildroot}/opt/morpho/lib/printer
%files
/opt/morpho/etc/ld.so.conf.d
/opt/morpho/lib/printer
%files devel
/opt/morpho/include/printer

%post
%postun
rm -rf /opt/morpho/lib/printer
if [ 1 -gt 0 ]; then rm /opt/morpho/etc/ld.so.conf.d/fujitsu.conf; fi;
%postun devel
rm -rf /opt/morpho/include/printer
%changelog
* Tue Nov 14 2017 The Fujitsu Library RPM Package 1.0.0
- Initial version of the package
