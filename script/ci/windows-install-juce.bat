[Net.ServicePointManager]::SecurityProtocol = [Net.SecurityProtocolType]::Tls, [Net.SecurityProtocolType]::Tls11, [Net.SecurityProtocolType]::Tls12, [Net.SecurityProtocolType]::Ssl3
[Net.ServicePointManager]::SecurityProtocol = "Tls, Tls11, Tls12, Ssl3"

wget https://github.com/juce-framework/JUCE/releases/download/7.0.3/juce-7.0.3-windows.zip -O juce.zip
unzip juce.zip
cd JUCE
cmake -B cmake-build-install -G"Visual Studio 16 2019"
cmake --build cmake-build-install --target install
