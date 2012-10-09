#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "CMessage.hpp"

int main(int argc, char* argv[])
{
   srand(time(NULL));

   CMessage::write();

   return 0;
}
