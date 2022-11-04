echo Build started %DATE% %TIME%

set PATH=C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\Common7\IDE;%PATH%

set msbuild=devenv
set sln=crproj.sln
set buildlog=project.log

rem type nul > %buildlog%
echo Build started %DATE% %TIME% > %buildlog%

:buildmain
set buildprofile="Release"
%msbuild% %sln% /Clean %buildprofile%
%msbuild% %sln% /Build %buildprofile% /Out %buildlog% || goto builderr

:over
echo Build finished successfully %DATE% %TIME%
echo Build finished successfully %DATE% %TIME% >> %buildlog%
pause
exit(0)

:builderr
echo Build %buildprofile% failed %DATE% %TIME%
echo Build %buildprofile% failed %DATE% %TIME% >> %buildlog%
pause
exit(1)

