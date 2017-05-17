REM COPY HEADERS FOR CGL
REM CGL
rd /S /Q C:\local\64\cygnet\r\
xcopy /S /Y .\*.hpp C:\local\cygnet\64\r\include\cg\
xcopy /S /Y ..\x64\ReleaseLib\Cygnet3.lib C:\local\cygnet\64\r\lib\