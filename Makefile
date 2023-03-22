debug:
	if [ -d debug ]; then \
		rm -r debug; \
	fi
	mkdir debug
	cmake -DCMAKE_BUILD_TYPE=Debug -S . -B debug

setup:
	if [ -d build ]; then \
		rm -r build; \
	fi
	mkdir build
	cmake -S . -B build
