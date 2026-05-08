// #include "../includes/CommandHandler.hpp"
// #include<cctype>
// #include "../includes/config.hpp"
// #include "../includes/Server.hpp"
// #include "../includes/Commandeparse.hpp"


// void Client::HandledJOIN(const Commandeparse &cmd)
// {
//     try{

//         if(!Authenticated)
//             throw;
//         if(cmd.args.empty()){
//             creat_raply(":" + ser->get_hostname + " 461 " + "JOIN :Not enough parameters" + " :\r\n");
//             throw;
//         }
//         if(!valideArgs(cmd.args))//if no # //existe 
//         {
//             throw;
//         }
//         if(cmd.args.size() == 1 )
//         {
//             if(cmd.args[0] != "0")
//                 joinChannel(cmd.args[0], username, ser);//if first client creat channel
//             else 
//                 leaveAll(cmd.args[0], username, ser);
//         }
//         else 
//         {//MODE #secret +k hello42   //key == MODE #secret +k hello42
//             std::string *channels = split_Channels(cmd.args);
//             std::string *keys = split_Keys(cmd.args);
//             joinMultiChannl(channels, keys, username, ser);
//         }
//     }
//     catch(...){
//         return;
//     }
// }

// // void CommandHandler::HandledKICK(const Commandeparse &cmd)
// // {
    
// // }

// // void CommandHandler::HandledINVITE(const Commandeparse &cmd)
// // {
    
// // }

// // void CommandHandler::HandledTOPIC(const Commandeparse &cmd)
// // {
    
// // }

// // void CommandHandler::HandledMODE(const Commandeparse &cmd)
// // {
    
// // }

// // void CommandHandler::HandledPASS(const Commandeparse &cmd)
// // {
// //     // if(client.registerd)
// //     // {
// //     //     std::cout
// //     // }er(*NULL)
// //     config con;
// //     if(cmd.args.empty())
// //     {
// //         std::cout << "PASS: no password given\n";
// //         return ;
// //     }
// //     if(cmd.args.size() > 1)
// //     {
// //         std::cout << "error number args is > 1" << std::endl;
// //         return ;
// //     }
// //     std::string pass = cmd.args[0];
// //     if(pass != con.password)
// //     {
// //         std::cout << "password incorrect " << std::endl;
// //     }
// //     else
// //     {
// //         std::cout << "password correct" << std::endl;
// //     }
// // }
// // void CommandHandler::HandledPART(const Commandeparse &cmd)
// // {

    
// // }

// // void CommandHandler::HandledUSER(const Commandeparse &cmd)
// // {
// //     //check wach deja m regestered b password 
// //     // check wach authentificate 


// // }

