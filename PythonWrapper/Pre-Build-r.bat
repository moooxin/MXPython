
rem --------------------------------


set root_src_dir=%~dp0
set root_dst_dir=%~dp0

echo %root_src_dir%\
echo %root_dst_dir%\

cd %root_src_dir%\

set python_root_dir=%root_src_dir%\..\Python3.7.1\

rem debug 
set dest_name=Release
set dest_root_dir=%root_src_dir%\%dest_name%

mkdir %dest_root_dir%
mkdir %dest_root_dir%\ph
mkdir %dest_root_dir%\ph\lib
rem copy python bin ---------------------------------------------------------------

xcopy %python_root_dir%\bin\*.* %dest_root_dir% /e /h /y 

xcopy %python_root_dir%\home\lib\*.* %dest_root_dir%\ph\lib\ /e /h /y 

pause


