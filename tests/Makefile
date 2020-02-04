include-folder := C:/Users/colli/git/C++ Helpful Libraries/src
compiler := g++
cl-compiler-flags := /std:c++17 -I'$(include-folder)' /EHsc
gcc-compiler-flags := -std=c++17 -I'$(include-folder)'
clang-compiler-flags := -std=c++17 -I'$(include-folder)'

ifeq ($(compiler), cl)
compiler-flags := $(cl-compiler-flags)
endif

ifeq ($(compiler), g++)
compiler-flags := $(gcc-compiler-flags)
endif

ifeq ($(compiler), clang++)
compiler-flags := $(clang-compiler-flags)
endif

mathTest:	mathTest.cpp
	@$(compiler) $(compiler-flags) $< -o $@
	@mathTest.exe

stringTest:	stringTest.cpp
	@$(compiler) $(compiler-flags) $< -o $@
	@stringTest.exe

algorithmTest:	algorithmTest.cpp
	@$(compiler) $(compiler-flags) $< -o $@
	@algorithmTest.exe

graphTest:	graphTest.cpp
	@$(compiler) $(compiler-flags) $< -o $@
	@graphTest.exe

iostreamTest:	iostreamTest.cpp
	@$(compiler) $(compiler-flags) $< -o $@
	@iostreamTest.exe


tests:	mathTest stringTest algorithmTest

clean:
	-rm *.exe
	-rm *.obj