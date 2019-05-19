%define debug_package %{nil}

Name: ompchat
Version: 1.0
Release: 1
Summary: Basic chat program written as a test
License: MIT
Source0: %{name}-%{version}.tar.gz

%description
Very simple client-server chat.

%prep
%setup

%build
%{make_build}

%install
%{make_build}

%clean
make clean

%files
%{_bindir}/ompchat

%changelog