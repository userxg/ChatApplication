#include "Server.h"

int main()
{
    
    Server server(2525);
    try {
        server.Run();

    }
    catch (std::exception& e)
    {
        std::cout << e.what() << "\n";
    }
   
    system("pause");
    std::cin.get();
    return 0;
}