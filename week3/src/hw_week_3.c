#include <stdio.h>
#include <modbus-tcp.h>

int main(void) {
	modbus_t *ctx; //pointer

	ctx=modbus_new_tcp("127.0.01",1502);
	printf("Created Local Modbus");
	return 0;
}
