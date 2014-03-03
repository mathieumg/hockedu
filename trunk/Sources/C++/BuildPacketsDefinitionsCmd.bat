@echo off
pushd.
cd Reseau
python %1 || echo Python interpreter missing on this computer
set errorlevel=0
popd

