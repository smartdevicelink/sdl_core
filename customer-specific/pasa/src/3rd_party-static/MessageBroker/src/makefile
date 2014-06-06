
all: library binary

library:
	$(MAKE) -C lib_messagebroker $@
	$(MAKE) -C server $@
	$(MAKE) -C client $@

binary:

clean:
	$(MAKE) -C lib_messagebroker clean
	$(MAKE) -C server clean
	$(MAKE) -C client clean
	