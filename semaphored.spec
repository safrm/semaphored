Name:       semaphored
Summary:    Graphical tool for presenting waighted uml-like charts or kanban dashboards
Version:    0.0.1
Release:    1
Group:      System/Libraries
License:    LGPL v2.1
URL:        http://semaphored.safrm.net/
Vendor:     Miroslav Safr <miroslav.safr@gmail.com>
Source0:    %{name}-%{version}.tar.bz2
Autoreq: on
Autoreqprov: on
BuildRequires:  pkgconfig(QtCore)
BuildRequires:  pkgconfig(QtGui)
BuildRequires:  pkgconfig(QtNetwork)
BuildRequires:  pkgconfig(QtSvg)
BuildRequires:  pkgconfig(QtXml)
BuildRequires:  qt-devel  
BuildRequires:  gcc-c++

%description
Graphical tool for presenting waighted uml-like charts or kanban dashboards


%prep
%setup -c -n ./%{name}-%{version}

%build
%if 0%{?fedora} || 0%{?rhel_version} || 0%{?centos_version}
 /usr/bin/qmake-qt4 install_prefix=/usr
%else
 qmake install_prefix=/usr
%endif
make %{?jobs:-j%jobs}

%install
rm -rf %{buildroot}
export INSTALL_ROOT=%{buildroot}
make install


%files
%defattr(-,root,root,-)
%{_bindir}/semaphored
%{_datadir}/applications/semaphored.desktop
%{_datadir}/icons/hicolor/scalable/apps/semaphored.svg
%dir %{_datadir}/doc/semaphored
%{_datadir}/doc/semaphored/README
%{_datadir}/doc/semaphored/TODO
%{_datadir}/doc/semaphored/LICENSE.LGPL


