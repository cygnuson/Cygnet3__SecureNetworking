REM COPY HEADERS FOR CGL
REM CGL
rd /S /Q C:\local\32\cygnet\r\
xcopy /S /Y .\*.hpp C:\local\cygnet\32\r\include\cg\
xcopy /S /Y ..\ReleaseLib\Cygnet3.lib C:\local\cygnet\32\r\lib\