all:
	mkdir build
	(cd build; cmake -DCMAKE_BUILD_TYPE=Debug ..)

clean:
	-rm -rf build
