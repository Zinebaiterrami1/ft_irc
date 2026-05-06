# pragma once
#include<iostream>
// #include<string>
#include <vector>
class  Commandeparse
{
    public :
        std::string name;
        std::vector<std::string> args;
};


void execute(const Commandeparse &cmd);



