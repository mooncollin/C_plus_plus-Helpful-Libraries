include-folder=C:\Users\colli\git\C++ Helpful Libraries\src
compiler=g++
compiler-flags= -Wall -pedantic -std=c++17 -I'$(include-folder)'

mathTest:	mathTest.cpp
	@$(compiler) $(compiler-flags) $< -o $@
	@mathTest.exe
	@rm mathTest.exe

stringTest:	stringTest.cpp
	@$(compiler) $(compiler-flags) $< -o $@
	@stringTest.exe
	@rm stringTest.exe

algorithmTest:	algorithmTest.cpp
	@$(compiler) $(compiler-flags) $< -o $@
	@algorithmTest.exe
	@rm algorithmTest.exe

tests:	mathTest stringTest algorithmTest