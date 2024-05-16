
#include "Client.h"



int main() {
    Client client;
    try {
        client.Run();
    }
    catch (std::exception& e)
    {
        std::cout << e.what();
    }

    std::cin.get();
    system("pause");
    return 0;
}