Summary: A program by Rob Burrowes to generate lockfiles per process
Name: lockfile
License: MIT
Version: 1.0.0
Release: 1
Source: lockfile-1.0.0.tar.gz
BuildArch: noarch
BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}

%description
 
%prep
%setup -q

%build
make

%install
make install

%post 

%clean
rm -rf $RPM_BUILD_ROOT
 
%files
/usr/local
%defattr(-,root,root)
/usr/local/bin/lockfile
/usr/local/lib/liblockfile.a
/usr/local/include/lockfile_p.h
