Name:       easybrain
Summary:    Graphical tool for presenting waighted uml-like charts or kanban dashboards
Version:    0.0.1
Release:    1
Group:      System/Libraries
License:    LGPL v2.1
URL:        http://safrm.net/projects/semaphored
Vendor:     Miroslav Safr <miroslav.safr@gmail.com>
Source0:    %{name}-%{version}.tar.bz2
Autoreq: on
Autoreqprov: on

%description
Graphical tool for presenting waighted uml-like charts or kanban dashboards


%prep
%setup -c -n ./%{name}-%{version}
# >> setup
# << setup

%build
# >> build pre
qmake install_prefix=/usr
# << build pre
make %{?jobs:-j%jobs}

# >> build post
# << build post

%install
rm -rf %{buildroot}
# >> install pre
export INSTALL_ROOT=%{buildroot}
# << install pre 
make install

# >> install post
# << install post






%files
%defattr(-,root,root,-)
# >> files
%{_bindir}/semaphored
%{_datadir}/applications/semaphored.desktop
%{_datadir}/icons/hicolor/scalable/apps/semaphored.svg
%{_datadir}/doc/semaphored/README
%{_datadir}/doc/seamphored/TODO 
%{_datadir}/doc/seamphored/LICENSE.LGPL
# << files


