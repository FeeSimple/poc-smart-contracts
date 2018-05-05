include .env
export

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

setupacc:
	cleos wallet unlock --password $(DEFAULT_PWD)
	cleos create account eosio user $(EOSIO_PUB_KEY) $(EOSIO_PUB_KEY)
	cleos create account $(EOSIO_ACCOUNT_NAME) $(INV_CONTRACT_ACCOUNT_NAME) $(EOSIO_PUB_KEY) $(EOSIO_PUB_KEY)

# Requires account $(INV_CONTRACT_ACCOUNT_NAME)
reinstall:
	make rebuild
	make deploy

# Requires account $(INV_CONTRACT_ACCOUNT_NAME)
deploy:
	cleos set contract $(INV_CONTRACT_ACCOUNT_NAME) inventory -p $(INV_CONTRACT_ACCOUNT_NAME)
