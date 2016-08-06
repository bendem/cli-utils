CXX ?= g++
CXX_FLAGS := -pedantic $(CXX_FLAGS)
comp = $(CXX) $(CXXFLAGS)

PREFIX ?= /usr

executables = log color
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
build/:
	mkdir -p $@


# build
$(executables_out): build/%: src/%.cpp $(obj_files)
	$(comp) -o $@ $^

build/%.o: src/%.cpp src/%.hpp
	$(comp) -c -o $@ $<

build/%.o: src/%.cpp
	$(comp) -c -o $@ $<

# tests
test: build/tests build/run_tests run_tests

build/run_tests: tests/tests.cpp $(test_obj_files) $(obj_files)
	$(comp) -o $@ $^

build/tests:
	mkdir -p $@

build/tests/%.o: tests/%.cpp
	$(comp) -c -o $@ $<

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
