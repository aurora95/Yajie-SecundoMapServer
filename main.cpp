#include <iostream>
#include <ev++.h>
#include <stdio.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include "AppMapServer.h"

int main()
{
    AppMapServer server(6669);
    server.run();
}
