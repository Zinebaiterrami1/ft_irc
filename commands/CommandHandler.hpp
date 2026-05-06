# pragma once
#include "Commandeparse.hpp"
class  CommandHandler
{
    // public:
    //     void execute(const Commandeparse &cmd);
    private:
        void HandledJOIN(const Commandeparse &cmd);
        void HandledKICK(const Commandeparse &cmd);
        void HandledINVITE(const Commandeparse &cmd);
        void HandledTOPIC(const Commandeparse &cmd);
        void HandledMODE(const Commandeparse &cmd);
        void HandledPRIVMSG(const Commandeparse &cmd);
};

