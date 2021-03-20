Name: colorgrab
Version: 0.3
Release: 1%{?dist}
Summary: A cross-platform color picker
License: MIT
URL: https://github.com/nielssp/colorgrab
Source0: https://github.com/nielssp/colorgrab/archive/refs/tags/v%{version}.tar.gz#/%{name}-%{version}.tar.gz

BuildRequires: compat-wxGTK3-gtk2-devel

%description
A cross-platform color picker.

%prep
%autosetup -p1 -S git

%build
%cmake
%cmake_build

%install
%cmake_install

%files
/usr/bin/colorgrab
/usr/share/applications/colorgrab.desktop
/usr/share/icons/hicolor/128x128/apps/colorgrab.png
/usr/share/icons/hicolor/16x16/apps/colorgrab.png
/usr/share/icons/hicolor/256x256/apps/colorgrab.png
/usr/share/icons/hicolor/32x32/apps/colorgrab.png
/usr/share/icons/hicolor/48x48/apps/colorgrab.png
/usr/share/icons/hicolor/64x64/apps/colorgrab.png
/usr/share/icons/hicolor/scalable/apps/colorgrab.svg
/usr/share/doc/colorgrab/LICENSE
/usr/share/doc/colorgrab/README.md

%changelog
* Fri Mar 19 2021 John Sullivan <jsgh@kanargh.org.uk> - 0.3-1
- Create Fedora RPM
