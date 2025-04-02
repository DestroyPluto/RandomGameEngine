
pushd "./build"
#build the main project
cmake -G "NMake Makefiles" --fresh -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..
NMake
popd

pushd "./Bin"
.\Mercury.exe
popd