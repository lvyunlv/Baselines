#include <shark/protocols/init.hpp>
#include <shark/protocols/finalize.hpp>
#include <shark/protocols/input.hpp>
#include <shark/protocols/output.hpp>
#include <shark/protocols/ars.hpp>
#include <shark/protocols/mul.hpp>
#include <shark/utils/assert.hpp>
#include <shark/utils/timer.hpp>

using u64 = shark::u64;
using namespace shark::protocols;

int f = 16;
u64 n = 10000;

shark::span<u64> gelu(shark::span<u64> &x)
{
    // return relu::call(x);

    // quad approximation from mpcformer
    //   x^2 / 8 + x/4 + 1/2
    // = 2^f * (x^2 / (2^2f * 8) + x/(2^f * 4) + 1/2)
    // = (x^2 + 2^{f+1}x + 2^{2f+2}) / 2^{f+3}
    auto x2 = mul::call(x, x);

    shark::span<u64> y(x.size());
    for (u64 i = 0; i < x.size(); ++i)
    {
        y[i] = x2[i] + x[i] * (1ull << (f + 1));
        if (party != DEALER)
        {
            y[i] += (1ull << (2*f + 2));
        }
    }
    return ars::call(y, f + 3);
}

int main(int argc, char **argv)
{
    init::from_args(argc, argv);

    shark::span<u64> X(n);

    for (u64 i = 0; i < n; i++)
        X[i] = 0;

    if (party != DEALER)
        peer->sync();

    shark::utils::start_timer("gelu");
    auto Y = gelu(X);
    shark::utils::stop_timer("gelu");
    output::call(Y);
    
    finalize::call();
    shark::utils::print_all_timers();
}
