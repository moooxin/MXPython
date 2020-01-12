
rem --------------------------------


set root_src_dir=%~dp0
set root_dst_dir=%~dp0

echo %root_src_dir%\
echo %root_dst_dir%\

cd %root_src_dir%\

rem copy python2.7.13 bin ---------------------------------------------------------------

set dest_name=Debug27

set python_root_dir=%root_src_dir%\..\Python2.7.13\
set dest_root_dir=%root_src_dir%\%dest_name%
set pydir=ph27

mkdir %dest_root_dir%
mkdir %dest_root_dir%\%pydir%
mkdir %dest_root_dir%\%pydir%\lib

xcopy %python_root_dir%\bin\*.* %dest_root_dir% /e /h /y 
xcopy %python_root_dir%\home\lib\*.* %dest_root_dir%\%pydir%\lib\ /e /h /y 

rem copy python3.7.1 bin ---------------------------------------------------------------

set dest_name=Debug

set python_root_dir=%root_src_dir%\..\Python3.7.1\
set dest_root_dir=%root_src_dir%\%dest_name%
set pydir=ph37

mkdir %dest_root_dir%
mkdir %dest_root_dir%\%pydir%
mkdir %dest_root_dir%\%pydir%\lib

xcopy %python_root_dir%\bin\*.* %dest_root_dir% /e /h /y 
xcopy %python_root_dir%\home\lib\*.* %dest_root_dir%\%pydir%\lib\ /e /h /y 

pause


