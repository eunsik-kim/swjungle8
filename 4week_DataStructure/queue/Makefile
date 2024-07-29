.PHONY: help build test

build:
build: ## Build executables
	$(MAKE) -C src

test:
test: ## Test rbtree implementation
	$(MAKE) -C test cleantest

clean:
clean: ## Clear build environment
	$(MAKE) -C src clean
	$(MAKE) -C test clean
