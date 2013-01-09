@echo off
setlocal EnableDelayedExpansion
set liste=
set pathLancement=%~0
set pathLancement=!pathLancement:%~nx0=!
echo %pathLancement%
FOR /R "./InterfaceGraphique/bin/ca/polymtl/inf2990" %%f IN (*.class) DO (
    echo %%~nxf | findstr /L "$"
    IF NOT !errorlevel! == 0 (set var=%%f) 
    IF NOT !errorlevel! == 0 (set var=!var:.class=!)
    IF NOT !errorlevel! == 0 (set var=!var:%pathLancement%InterfaceGraphique\bin\=!)
    IF NOT !errorlevel! == 0 (set var=!var:\=.!)
    IF NOT !errorlevel! == 0 (set liste= !liste! !var!)
    IF NOT !errorlevel! == 0 (echo !var!)
)

javah -jni -o FacadeJNI.h -classpath "./InterfaceGraphique/bin" !liste!
javap -s -p -classpath "./InterfaceGraphique/bin" !liste! > Signatures.txt
endlocal

PAUSE