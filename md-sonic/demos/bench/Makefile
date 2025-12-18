SHELL := /bin/bash

PARTY_COUNTS := 2 4 8 16 32 64

JAR_FILE := fresco-demo-bench.jar
BASE_PORT := 8080
LOG_FILE := log.txt

move_%PC:
	$(eval N := $(subst move_,,$(subst PC,,$@)))
	for ((i=1; i<=$(N); i++)); do \
		mkdir -p server$$i; \
		cp target/$(JAR_FILE) server$$i/; \
	done

runRelu_%PC:
	$(eval N := $(subst runRelu_,,$(subst PC,,$@)))
	$(eval PARTY_LIST := $(shell for ((i=1; i<=$(N); i++)); do echo -n "-p $$i:localhost:$$(( $(BASE_PORT) + i )) "; done))
	for ((i=1; i<=$(N); i++)); do \
		cd server$$i && \
		java -jar $(JAR_FILE) \
			-e SEQUENTIAL_BATCHED \
			-i $$i \
			$(PARTY_LIST) \
			-s sonic64 \
			-D sonic.useMaskedEvaluation=true \
			-sonic -l -relu -h1 1 -w1 10000 \
			> $(LOG_FILE) 2>&1 & \
	done


all: $(foreach n,$(PARTY_COUNTS),move_$(n)PC) $(foreach n,$(PARTY_COUNTS),runRelu_$(n)PC)

clean:
	rm -rf server*/


