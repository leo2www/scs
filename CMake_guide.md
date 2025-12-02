cmake -DCMAKE_INSTALL_PREFIX:PATH="E:\project\Solvers\scs\build\install" -BBUILD -DBUILD_SHARED_LIBS=OFF ../

cmake --build BUILD --config Release 

cmake --install BUILD --config Release 

---
使用
cmake -S .. -B BUILD

cmake --build BUILD --config Release

.\BUILD\Release\test_scs.exe