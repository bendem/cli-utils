PREFIX ?= /usr
CXX ?= g++
CXXFLAGS := -I src -std=c++14 $(CXXFLAGS)
comp = $(CXX) $(CXXFLAGS)

dep_catch_url = https://raw.githubusercontent.com/philsquared/Catch/35f510545d55a831372d3113747bf1314ff4f2ef/single_include/catch.hpp
dep_catch_name := $(shell basename $(dep_catch_url))

executables = log color prepend
executables_out := $(executables:%=build/%)

src_files := $(wildcard src/*.cpp)
obj_files := $(src_files:src/%.cpp=build/%.o)
obj_files := $(filter-out $(executables_out:%=%.o),$(obj_files))
test_src_files := $(filter-out tests/tests.cpp,$(wildcard tests/*.cpp))
test_obj_files := $(test_src_files:%.cpp=build/%.o)


.PHONY: all

all: build/ $(executables_out)
	@echo done

# dirs
build/ include_tests/:
	mkdir -p $@


# build
$(executables_out): build/%: src/%.cpp $(obj_files)
	$(comp) -o $@ $^

build/%.o: src/%.cpp src/%.hpp
	$(comp) -c -o $@ $<

build/%.o: src/%.cpp
	$(comp) -c -o $@ $<

# tests
test: build/tests include_tests/$(dep_catch_name) build/run_tests run_tests

include_tests/$(dep_catch_name): include_tests/
	curl -s -o $@ $(dep_catch_url)

build/run_tests: tests/tests.cpp $(test_obj_files) $(obj_files)
	$(comp) -I include_tests -o $@ $^

build/tests:
	mkdir -p $@

build/tests/%.o: tests/%.cpp
	$(comp) -I include_tests -c -o $@ $<

run_tests: build/run_tests
	./build/run_tests


# install
install:
	$(foreach binary,$(executables),cp -u build/$(binary) $(PREFIX)/bin/$(binary);)


# misc
clean:
	rm -r build

debug:
	@echo $(executables_out)
	@echo $(obj_files)
