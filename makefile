# Variables set on .env

include .env

all:
	rm -f fsmanager/* !fsmanager/.gitkeep
	rm -f dapphub/* !dapphub/.gitkeep
	eosiocpp -o fsmanager/fsmanager.wast src/fsmanager.cpp
	eosiocpp -g fsmanager/fsmanager.abi src/fsmanager.cpp
	eosiocpp -o dapphub/dapphub.wast src/dapphub.cpp
	eosiocpp -g dapphub/dapphub.abi src/dapphub.cpp

clean:
	rm -f fsmanager/* !fsmanager/.gitkeep
	rm -f dapphub/* !dapphub/.gitkeep

build:
	eosiocpp -o fsmanager/fsmanager.wast src/fsmanager.cpp
	eosiocpp -g fsmanager/fsmanager.abi src/fsmanager.cpp
	eosiocpp -o dapphub/dapphub.wast src/dapphub.cpp
	eosiocpp -g dapphub/dapphub.abi src/dapphub.cpp

unlockStage:
	cleos --wallet-url http://127.0.0.1:6666 --url http://138.197.194.220:8877  wallet unlock -n dev --password $(WALLET_PWD)

importKeysStage:
	cleos --wallet-url http://127.0.0.1:6666 --url http://138.197.194.220:8877 wallet import $(USRAAA_PRIV) -n dev
	cleos --wallet-url http://127.0.0.1:6666 --url http://138.197.194.220:8877 wallet import $(DEVACC_PRIV) -n dev

setupaccStage:
	cleos --wallet-url http://127.0.0.1:6666 --url http://138.197.194.220:8877 system newaccount --stake-net "1 XFS" --stake-cpu "1 XFS" --buy-ram "13 XFS" $(USRA_ACCOUNT_NAME) $(FSMGR_ACCOUNT_NAME) $(USRA_PUB) $(FSMGR_PUB)

buyRamMatheusStage:
	cleos --wallet-url http://127.0.0.1:6666 --url http://138.197.194.220:8877 system buyram $(USRA_ACCOUNT_NAME) $(USRA_ACCOUNT_NAME) "1 XFS"

buyRamFsmgrStage:
	cleos --wallet-url http://127.0.0.1:6666 --url http://138.197.194.220:8877 system buyram $(USRA_ACCOUNT_NAME) $(FSMGR_ACCOUNT_NAME) "1 XFS"

sellRamStage:
	cleos --wallet-url http://127.0.0.1:6666 --url http://138.197.194.220:8877 system sellram $(USRA_ACCOUNT_NAME) 100000

deployStage:
	cleos --wallet-url http://127.0.0.1:6666 --url http://138.197.194.220:8877 set contract $(FSMGR_ACCOUNT_NAME) ~/poc-smart-contracts/fsmanager ~/poc-smart-contracts/fsmanager/fsmanager.wast ~/poc-smart-contracts/fsmanager/fsmanager.abi

pushFakeDataStage:
	cleos --wallet-url http://127.0.0.1:6666 --url http://138.197.194.220:8877 push action $(FSMGR_ACCOUNT_NAME) addproperty '{"author":"$(USR_ACCOUNT_NAME)","name":"Abbey Road Studios","address_1":"3 Abbey Road, St John`s Wood","address_2":"2nd Floor","city":"City of Westminster","region":"London","postal_code":"123456","unit_count":1}' --permission $(USR_ACCOUNT_NAME)@active
	cleos --wallet-url http://127.0.0.1:6666 --url http://138.197.194.220:8877 push action $(FSMGR_ACCOUNT_NAME) addproperty '{"author":"$(USR_ACCOUNT_NAME)","name":"Lincoln Building","address_1":"Herriman St, 545","address_2":"5th Block","city":"Salt Lake City","region":"Park City","postal_code":"58086","unit_count":7}' --permission $(USR_ACCOUNT_NAME)@active
	cleos --wallet-url http://127.0.0.1:6666 --url http://138.197.194.220:8877 push action $(FSMGR_ACCOUNT_NAME) addproperty '{"author":"$(USRB_ACCOUNT_NAME)","name":"John Ferry","address_1":"Sandpoint Avenue, 3332","address_2":"2nd floor","city":"Idaho City","region":"Boise County","postal_code":"58543","unit_count":4}' --permission $(USRB_ACCOUNT_NAME)@active

getDataStage:
	cleos --wallet-url http://127.0.0.1:6666 --url http://138.197.194.220:8877 get table $(FSMGR_ACCOUNT_NAME) $(USR_ACCOUNT_NAME) property

getAccountMtsStage:
	cleos --wallet-url http://127.0.0.1:6666 --url http://138.197.194.220:8877 get account $(USRA_ACCOUNT_NAME)

getAccountFsmgrStage:
	cleos --wallet-url http://127.0.0.1:6666 --url http://138.197.194.220:8877 get account $(FSMGR_ACCOUNT_NAME)