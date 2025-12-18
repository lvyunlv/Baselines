#include <shark/protocols/init.hpp>
#include <shark/protocols/finalize.hpp>
#include <shark/protocols/input.hpp>
#include <shark/protocols/output.hpp>
#include <shark/protocols/relu.hpp>
#include <shark/protocols/mul.hpp>
#include <shark/protocols/reciprocal.hpp>
#include <shark/protocols/lrs.hpp>
#include <shark/utils/assert.hpp>
#include <shark/utils/timer.hpp>

using u64 = shark::u64;
using namespace shark::protocols;

int f = 16;
u64 n = 10000;

shark::span<u64> softmax(u64 a, u64 b, shark::span<u64> &x)
{
    always_assert(x.size() == (a * b));
    auto exps = relu::call(x);

    shark::span<u64> den(a);
    for (u64 i = 0; i < a; ++i)
    {
        u64 sum = 0;
        for (u64 j = 0; j < b; ++j)
        {
            sum += exps[i * b + j];
        }
        den[i] = sum + 1 * (party != DEALER);
    }

    auto den_inv = reciprocal::call(den, f);
    shark::span<u64> den_inv_expanded(a * b);
    for (u64 i = 0; i < a; ++i)
    {
        for (u64 j = 0; j < b; ++j)
        {
            den_inv_expanded[i * b + j] = den_inv[i];
        }
    }

    auto y = mul::call(exps, den_inv_expanded);
    return lrs::call(y, f);
}

int main(int argc, char **argv)
{
    init::from_args(argc, argv);

    shark::span<u64> X(n);

    for (u64 i = 0; i < n; i++)
        X[i] = 0;

    if (party != DEALER)
        peer->sync();

    shark::utils::start_timer("softmax");
    auto Y = softmax(1, n, X);
    shark::utils::stop_timer("softmax");
    output::call(Y);
    
    finalize::call();
    shark::utils::print_all_timers();
}
