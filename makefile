include .env
export

all:
	make rebuild

rebuild:
	make clean
	make build

clean:
	rm -f fsmanager/* !fsmanager/.gitkeep
	rm -f dapphub/* !dapphub/.gitkeep

build:
	eosiocpp -o fsmanager/fsmanager.wast src/fsmanager.cpp
	eosiocpp -g fsmanager/fsmanager.abi src/fsmanager.cpp
	eosiocpp -o dapphub/dapphub.wast src/dapphub.cpp
	eosiocpp -g dapphub/dapphub.abi src/dapphub.cpp

setupacc:
	cleos wallet unlock --password $``(DEFAULT_PWD)
	cleos create account eosio user $(EOSIO_PUB_KEY) $(EOSIO_PUB_KEY)
	cleos create account $(EOSIO_ACCOUNT_NAME) $(FS_CONTRACT_ACCOUNT_NAME) $(EOSIO_PUB_KEY) $(EOSIO_PUB_KEY)

# Requires account $(FS_CONTRACT_ACCOUNT_NAME)
reinstall:
	make rebuild
	make deploy

# Requires account $(FS_CONTRACT_ACCOUNT_NAME)
deploy:
	cleos set contract $(FS_CONTRACT_ACCOUNT_NAME) fsmanager -p $(FS_CONTRACT_ACCOUNT_NAME)
