all:
	make rebuild

rebuild:
	make clean
	make build

clean:
	rm -f inventory/* !inventory/.gitkeep

build:
	eosiocpp -o inventory/inventory.wast src/inventory.cpp
	eosiocpp -g inventory/inventory.abi src/inventory.cpp

# requires unlocked account inv.code
reinstall:
	make rebuild
	make deploy

# requires unlocked account inv.code
deploy:
	cleos set contract inv.code inventory -p inv.code
