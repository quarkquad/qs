mkdir build\windows
pushd build\windows
JUCE_DIR="${JUCE_DIR:-../cmake/lib/cmake/JUCE-7.0.3/}"
cmake -DCMAKE_VERBOSE_MAKEFILE:BOOL=ON -DCMAKE_BUILD_TYPE=Debug -DJUCE_DIR="$JUCE_DIR" -DQS_TESTS=ON ../..
cmake --build . --target qs_test
ctest
popd

