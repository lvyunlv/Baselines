#include <shark/protocols/init.hpp>
#include <shark/protocols/finalize.hpp>
#include <shark/protocols/input.hpp>
#include <shark/protocols/output.hpp>
#include <shark/protocols/sigmoid.hpp>
#include <shark/utils/assert.hpp>
#include <shark/utils/timer.hpp>

using u64 = shark::u64;
using namespace shark::protocols;

int f = 16;
u64 n = 10000;  // The number of testing instance

int main(int argc, char **argv)
{
    init::from_args(argc, argv);

    shark::span<u64> X(n);

    for (u64 i = 0; i < n; i++)
        X[i] = 0;

    if (party != DEALER)
        peer->sync();

    shark::utils::start_timer("sigmoid");
    auto Y = sigmoid::call(f, X);
    shark::utils::stop_timer("sigmoid");
    output::call(Y);
    
    finalize::call();
    shark::utils::print_all_timers();
}
