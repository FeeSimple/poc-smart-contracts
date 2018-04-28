all:
	rm -f build/* !build/.gitkeep
	eosiocpp -o build/guest.wast src/guest.cpp
	eosiocpp -g build/guest.abi src/guest.cpp
