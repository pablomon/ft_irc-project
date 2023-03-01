#include "bot.hpp"
#include "empeBot.hpp"

int setPort(int narg, char **xarg)
{
    int port = 6667;
    for (int i = 0; i < narg; i++)
    {
        if(xarg[i][0] == '+')
        {
            int aux = 0;
            for (int j = 1; j < (int)strlen(xarg[i]); j++)
            {
                aux = aux * 10 + xarg[i][j] - '0';
            }
            port = aux;
        }
    }
    return (port);
}

std::string setPassword(int narg, char **xarg)
{
    std::string pass;

    pass = "";
    for (int i = 0; i < narg; i++)
    {
        if (xarg[i][0] == '-')
        {
            pass = &xarg[i][1];
            break;
        }
    }
    return (pass);
}

std::string setServerAddr(int narg, char **xarg)
{
    std::string serverAddr = "127.0.0.1";
    for (int i = 0; i < narg; i++)
    {
        if(xarg[i][0] >= '0' && xarg[i][0] <= '9')
            serverAddr = xarg[i];
    }
    return (serverAddr);
}

int main(int narg, char **xarg)
{
    int port = setPort(narg, xarg);
    std::string serverAddr = setServerAddr(narg, xarg);
    std::string pass = setPassword(narg, xarg);

    empeBot cBot(port, serverAddr, pass);
    if (cBot.connectBot() == 0)
    {
        std::cerr << "Bot can not initialize" << std::endl;
        return (1);
    }
    cBot.execute();

    return (0);
}