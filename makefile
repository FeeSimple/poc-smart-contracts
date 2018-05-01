all:
	make rebuild

rebuild:
	make clean
	make build

clean:
	rm -f property/* !property/.gitkeep

build:
	eosiocpp -o property/property.wast src/property.cpp
	eosiocpp -g property/property.abi src/property.cpp

# requires unlocked account prop.code
reinstall:
	make rebuild
	make deploy

# requires unlocked account prop.code
deploy:
	cleos set contract prop.code property -p prop.code
