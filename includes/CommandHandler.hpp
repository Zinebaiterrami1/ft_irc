# pragma once
#include "Commandeparse.hpp"
class  CommandHandler
{
    // public:
    //     void execute(const Commandeparse &cmd);
    public:
        void HandledJOIN(const Commandeparse &cmd);
        void HandledKICK(const Commandeparse &cmd);
        void HandledINVITE(const Commandeparse &cmd);
        void HandledTOPIC(const Commandeparse &cmd);
        void HandledMODE(const Commandeparse &cmd);
        void HandledPRIVMSG(const Commandeparse &cmd);
        void HandledPASS(const Commandeparse &cmd);
        void HandledUSER(const Commandeparse &cmd);
        void HandledNICK(const Commandeparse &cmd);
        void HandledPART(const Commandeparse &cmd);

};

