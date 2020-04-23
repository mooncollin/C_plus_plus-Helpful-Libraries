include-folder := ../src/
compiler := g++
cl-compiler-flags := /std:c++2a -I'$(include-folder)' /EHsc
gcc-compiler-flags := -std=c++2a -I'$(include-folder)'
clang-compiler-flags := -std=c++2a -I'$(include-folder)'

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

httpTest:	httpTest.cpp
	@$(compiler) $(compiler-flags) $< -o $@ -lWs2_32
	@httpTest.exe

socketTest:	socketTest.cpp
	@$(compiler) $(compiler-flags) $< -o $@ -lWs2_32
	@socketTest.exe

rangesTest:	rangesTest.cpp
	@$(compiler) $(compiler-flags) $< -o $@.exe
	@rangesTest.exe


tests:	mathTest stringTest algorithmTest

clean:
	-rm *.exe
	-rm *.obj