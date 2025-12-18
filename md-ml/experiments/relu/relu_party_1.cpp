// By Yansong Zhang

#include "relu_config.h"

#include "share/Spdz2kShare.h"
#include "protocols/Circuit.h"
#include "utils/print_vector.h"

using namespace std;
using namespace md_ml;
using namespace md_ml::experiments::relu;

int main(int argc, char **argv) {
    using ShrType = Spdz2kShare64;
    using ClearType = ShrType::ClearType;

    std::string dst_ip = "";
    if (argc < 2)
    {
        dst_ip = "127.0.0.1";
    } else {
        dst_ip = argv[1];
    }
    

    PartyWithFakeOffline<ShrType> party(1, 2, 5050, dst_ip, kJobName);
    Circuit<ShrType> circuit(party);

    auto a = circuit.input(0, 1, dim);
    auto b = circuit.relu(a);
    auto c = circuit.output(b);
    circuit.addEndpoint(c);

    circuit.readOfflineFromFile();
    circuit.runOnlineWithBenckmark();

    circuit.printStats();

    return 0;
}
