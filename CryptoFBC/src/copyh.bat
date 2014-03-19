@echo off
copy /Y *.h "D:\Program Files (x86)\Microsoft Visual Studio 9.0\VC\include\CryptoFBC\*.h"
copy /Y Release\CryptoFBC.lib "D:\Program Files (x86)\Microsoft Visual Studio 9.0\VC\lib\cryptofbcvc8.lib"
copy /Y Debug\CryptoFBC.lib "D:\Program Files (x86)\Microsoft Visual Studio 9.0\VC\lib\cryptofbcvc8D.lib"