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
	cleos --wallet-url http://127.0.0.1:6666 --url http://138.197.194.220:8877  wallet unlock --password $(WALLET_PWD)

importKey:
	cleos --wallet-url http://127.0.0.1:6666 --url http://138.197.194.220:8877 wallet import --private-key 5KdUmNF1T6aNhzohopYNFL5p2JQq9op6TwXXtB6pvkwWSkyB53w

setupacc:
	cleos --wallet-url http://127.0.0.1:6666 --url http://138.197.194.220:8877 system newaccount useraaaaaaaa fsmgrcode111 EOS5Q7x2hZjgrx9qmXn2huzNwXcVB2bCmvmuzRHjP6iPHEgEjKQSk --stake-net "1 XFS" --stake-cpu "1 XFS" --buy-ram "3 XFS"

buyRam:
	cleos --wallet-url http://127.0.0.1:6666 --url http://138.197.194.220:8877 system buyram useraaaaaaab fsmgrcode111 "7 XFS"

deploy:
	cleos --wallet-url http://127.0.0.1:6666 --url http://138.197.194.220:8877 set contract fsmgrcode111 ~/feesimple_contracts/fsmanager ~/feesimple_contracts/fsmanager/fsmanager.wast ~/feesimple_contracts/fsmanager/fsmanager.abi

pushData:
	cleos --wallet-url http://127.0.0.1:6666 --url http://138.197.194.220:8877 push action fsmgrcode111 addproperty '{"author":"fsmgrcode111","name":"Abbey Road Studios","address_1":"3 Abbey Road, St John`s Wood","address_2":"2nd Floor","city":"City of Westminster","region":"London","postal_code":"123456","unit_count":1}' --permission fsmgrcode111

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

