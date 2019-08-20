#include "doctest.h"

int main(int argc, char* argv[]) {
	doctest::Context ctx;
	ctx.setOption("abort-after", 5);
	ctx.setOption("no-breaks", true);
	ctx.setOption("order-by", "file");
	ctx.applyCommandLine(argc, argv);
	return ctx.run();
}