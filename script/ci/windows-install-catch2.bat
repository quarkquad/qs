[Net.ServicePointManager]::SecurityProtocol = [Net.SecurityProtocolType]::Tls, [Net.SecurityProtocolType]::Tls11, [Net.SecurityProtocolType]::Tls12, [Net.SecurityProtocolType]::Ssl3
[Net.ServicePointManager]::SecurityProtocol = "Tls, Tls11, Tls12, Ssl3"

wget -q https://github.com/catchorg/Catch2/archive/refs/tags/v3.3.2.zip -O catch2.zip
unzip catch2.zip
cd Catch2-3.3.2

cmake -B cmake-build-install -H. -DBUILD_TESTING=OFF -G"Visual Studio 16 2019"
cmake --build cmake-build-install --target install
