#include "doctest.h"

int main(int argc, char* argv[]) {
	int res = 0;
	doctest::Context ctx;
	ctx.setOption("abort-after", 5);
	ctx.setOption("no-breaks", true);
	ctx.setOption("order-by", "file");
	ctx.applyCommandLine(argc, argv);

	res = ctx.run();
	if(ctx.shouldExit())
		return res;
	
	return res;
}