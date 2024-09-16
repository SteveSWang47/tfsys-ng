#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <fstream>

int main()
{
    printf("Content-type: text/html\n\n");
    std::string id(getenv("REQUEST_URI")), curlin;
    id = id.substr(id.size()-7, 7).data();
    std::ifstream listin("/usr/share/tfsys/setlist");
    while(listin)
    {
        getline(listin, curlin);
        printf("<a href=\"/cgi-bin/solve.cgi?%s%s\"> %s </a><br />\n", curlin.substr(0, 5).data(), id.data(), curlin.data());
    }
    return 0;
}