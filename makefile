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

unlock:
	cleos wallet unlock --password $(WALLET_PWD)

setupacc:	
	cleos create account $(EOSIO_ACCOUNT_NAME) $(USER_ACCOUNT_NAME) $(EOSIO_PUB) $(EOSIO_PUB)
	cleos create account $(EOSIO_ACCOUNT_NAME) $(FSMGR_ACCOUNT_NAME) $(EOSIO_PUB) $(EOSIO_PUB)

# Requires account $(FSMGR_ACCOUNT_NAME)
localReinstall:
	make rebuild
	make localDeploy

# Requires account $(FSMGR_ACCOUNT_NAME)
localDeploy:
	cleos set contract $(FSMGR_ACCOUNT_NAME) fsmanager -p $(FSMGR_ACCOUNT_NAME)

# Requires account $(USERAAAAAAAA_ACCOUNT_NAME)
testnetDeploy:
	cleos --wallet-url http://localhost:8888 --url http://138.197.194.220:8877 set contract fsmgrcode333 ~/Documents/feesimple/feesimple_contracts/fsmanager ~/Documents/feesimple/feesimple_contracts/fsmanager/fsmanager.wast ~/Documents/feesimple/feesimple_contracts/fsmanager/fsmanager.abi

# Push data
# cleos --wallet-url http://localhost:8888 --url http://138.197.194.220:8877 push action fsmgrcode333 addproperty '{"author":"mtsalenc111","name":"Abbey Road Studios","address_1":"3 Abbey Road, St John`s Wood","address_2":"2nd Floor","city":"City of Westminster","region":"London","postal_code":"123456","unit_count":1}' --permission mtsalenc111

# Query
# cleos --wallet-url http://localhost:8888 --url http://138.197.194.220:8877 get table fsmgrcode333 fsmgrcode333 property

