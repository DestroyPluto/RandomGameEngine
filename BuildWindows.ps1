
#build the main project
cmake -G "NMake Makefiles" --fresh .
NMake

pushd "./Bin"
.\Mercury.exe
popd