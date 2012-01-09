@echo off
copy /Y *.h "D:\Program Files\Microsoft Visual Studio 8\VC\include\CryptoFBC\*.h"
copy /Y Release\CryptoFBC.lib "D:\Program Files\Microsoft Visual Studio 8\VC\lib\cryptofbcvc8.lib"
copy /Y Debug\CryptoFBC.lib "D:\Program Files\Microsoft Visual Studio 8\VC\lib\cryptofbcvc8D.lib"