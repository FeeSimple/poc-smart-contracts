all:
	rm -f build/* !build/.gitkeep
	eosiocpp -o build/property.wast src/property.cpp
	eosiocpp -g build/property.abi src/property.cpp
