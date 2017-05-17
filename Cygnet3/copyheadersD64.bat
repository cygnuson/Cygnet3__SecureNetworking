REM COPY HEADERS FOR CGL
REM CGL
rd /S /Q C:\local\64\cygnet\d\
xcopy /S /Y .\*.hpp C:\local\cygnet\64\d\include\cg\
xcopy /S /Y ..\x64\DebugLib\Cygnet3.lib C:\local\cygnet\64\d\lib\
xcopy /S /Y x64\DebugLib\Cygnet3.pdb C:\local\cygnet\64\d\lib\