"C:\Program Files\CMake\bin\cmake.EXE" --no-warn-unused-cli -DCMAKE_BUILD_TYPE:STRING=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -DCMAKE_C_COMPILER:FILEPATH=C:\msys64\ucrt64\bin\gcc.exe -DCMAKE_CXX_COMPILER:FILEPATH=C:\msys64\ucrt64\bin\g++.exe -SE:/freelance/grimlock -Be:/freelance/grimlock/build -G "MinGW Makefiles"
"C:\Program Files\CMake\bin\cmake.EXE" --build e:/freelance/grimlock/build --config Debug --target all -j 14 --
cd .\build\
.\Grimlock.exe