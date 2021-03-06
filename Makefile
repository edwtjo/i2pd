UNAME := $(shell uname -s)
SHLIB := libi2pd.so
I2PD  := i2p
TESTS := tests/tests
GREP := fgrep
DEPS := obj/make.dep

include filelist.mk

USE_AESNI  := yes
USE_STATIC := no

ifeq ($(UNAME),Darwin)
	DAEMON_SRC += DaemonLinux.cpp
	include Makefile.osx
else ifeq ($(shell echo $(UNAME) | $(GREP) -c FreeBSD),1)
	DAEMON_SRC += DaemonLinux.cpp
	include Makefile.bsd
else ifeq ($(UNAME),Linux)
	DAEMON_SRC += DaemonLinux.cpp
	include Makefile.linux
else # win32
	DAEMON_SRC += DaemonWin32.cpp
endif

all: mk_build_dir $(SHLIB) $(I2PD)
tests: mk_build_test_dir $(TESTS)

mk_build_dir:
	mkdir -p obj
	mkdir -p obj/transport
	mkdir -p obj/util
	mkdir -p obj/crypto
	mkdir -p obj/tunnel
	mkdir -p obj/i2pcontrol
mk_build_test_dir:
	mkdir -p obj/tests

api: $(SHLIB)

## NOTE: The NEEDED_CXXFLAGS are here so that CXXFLAGS can be specified at build time
## **without** overwriting the CXXFLAGS which we need in order to build.
## For example, when adding 'hardening flags' to the build
## (e.g. -fstack-protector-strong -Wformat -Werror=format-security), we do not want to remove
## -std=c++11. If you want to remove this variable please do so in a way that allows setting
## custom FLAGS to work at build-time.

deps:
	@mkdir -p obj
	@mkdir -p obj/transport
	@mkdir -p obj/tests
	@mkdir -p obj/util
	@mkdir -p obj/crypto
	@mkdir -p obj/tunnel
	@mkdir -p obj/i2pcontrol
	$(CXX) $(CXXFLAGS) $(NEEDED_CXXFLAGS) -MM *.cpp > $(DEPS)
	@sed -i -e '/\.o:/ s/^/obj\//' $(DEPS)

obj/%.o : %.cpp
	@mkdir -p obj
	@mkdir -p obj/transport
	@mkdir -p obj/tests
	@mkdir -p obj/util
	@mkdir -p obj/crypto
	@mkdir -p obj/tunnel
	@mkdir -p obj/i2pcontrol
	$(CXX) $(CXXFLAGS) $(NEEDED_CXXFLAGS) $(INCFLAGS) $(CPU_FLAGS) -c -o $@ $<

# '-' is 'ignore if missing' on first run
-include $(DEPS)

$(I2PD):  $(patsubst %.cpp,obj/%.o,$(DAEMON_SRC))
	$(CXX) -o $@ $^ $(LDLIBS) $(LDFLAGS)

$(SHLIB): $(patsubst %.cpp,obj/%.o,$(LIB_SRC))
ifneq ($(USE_STATIC),yes)
	$(CXX) $(LDFLAGS) $(LDLIBS) -shared -o $@ $^
endif

$(TESTS): $(patsubst %.cpp,obj/%.o,$(TESTS_SRC))
	$(CXX) -o $@ $^ $(LDLIBS) $(LDTESTLIBS) $(LDFLAGS)


clean:
	rm -rf obj
	$(RM) $(I2PD) $(SHLIB) $(TESTS)

LATEST_TAG=$(shell git describe --tags --abbrev=0 master)
dist:
	git archive --format=tar.gz -9 --worktree-attributes \
	    --prefix=i2pd_$(LATEST_TAG)/ $(LATEST_TAG) -o i2pd_$(LATEST_TAG).tar.gz

.PHONY: all
.PHONY: tests
.PHONY: clean
.PHONY: deps
.PHONY: dist
.PHONY: api
.PHONY: mk_build_dir
.PHONY: mk_build_test_dir
