SRC_URL = http://cas.ee.ic.ac.uk/people/dt10/teaching/2013/hpce/cw2
PWD = $(shell pwd)

packages/sox-14.4.1.tar.gz :
	curl $(SRC_URL)/sox-14.4.1.tar.gz -o packages/sox-14.4.1.tar.gz

build/sox-14.4.1/configure : packages/sox-14.4.1.tar.gz
	cd build && tar -xzf ../packages/sox-14.4.1.tar.gz

build/sox-14.4.1/Makefile : build/sox-14.4.1/configure
	cd build/sox-14.4.1 && ./configure --prefix="$(PWD)/local"

build/sox-14.4.1/src/sox : build/sox-14.4.1/Makefile
	cd build/sox-14.4.1 && $(MAKE)

local/bin/sox : build/sox-14.4.1/src/sox
	cd build/sox-14.4.1 && $(MAKE) install


clean :
	rm -rf packages/*
	rm -rf build/*
	rm -rf local/*
