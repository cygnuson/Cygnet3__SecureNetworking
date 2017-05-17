REM COPY HEADERS FOR CGL
REM CGL
rd /S /Q C:\local\32\cygnet\d\
xcopy /S /Y .\*.hpp C:\local\cygnet\32\d\include\cg\
xcopy /S /Y ..\DebugLib\Cygnet3.lib C:\local\cygnet\32\d\lib\
xcopy /S /Y DebugLib\Cygnet3.pdb C:\local\cygnet\32\d\lib\