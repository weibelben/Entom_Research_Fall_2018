
#include "../HeaderFiles/CMakeSLAM.h"
#include "../HeaderFiles/CrashAndBurn.h"

#include <stdlib.h>
#include <iostream>


CrashAndBurn::CrashAndBurn(string warning)
{
    if (!TEST_RUN)
    {
        cout << warning;
        exit(0);
    }
}
