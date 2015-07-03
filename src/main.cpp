/// \file main.cpp

#include <iostream>                  // for std::cout

#include "version.h"
#include "id.h"

/// \brief  Main function
/// \param  argc An integer argument count of the command line arguments
/// \param  argv An argument vector of the command line arguments
/// \return an integer 0 upon exit success  
///
int main(int argc, char** argv)
{

  
   std::cout << "Hello Boost!, " << VERSION << std::endl;
   std::cout << GIT_DETAILS << std::endl;

    return 0;
}

