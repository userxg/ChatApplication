#include "Server.h"

int main()
{
    
    Server server(2525);
    server.Run();
   
    system("pause");
    std::cin.get();
    return 0;
}