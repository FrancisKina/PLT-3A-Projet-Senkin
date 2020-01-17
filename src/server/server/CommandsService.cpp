#include "CommandsService.h"
#include "ServiceException.h"

#include <iostream>
using namespace server;
using namespace std;

CommandsService::CommandsService() : AbstractService("/command"), engine(engine){
    commands["size"] = 0;
    commands["commands"][0] = "";
    commandsHistoric["size"] = 0;
    commandsHistoric["commands"][0] = "";
}

HttpStatus CommandsService::get(Json::Value& out, int id) const
{
    // if (id == 1)
    // {
    //     out["canGet"] = (commands.size() > 0) ? 1 : 0;
    // }
    // else
    // {
        // only new commands
        out = commands;

        // for each get , reinitialize the dynamic list of commands
        //commands["size"] = 0;
        //commands["commands"] = Json::Value(Json::arrayValue);
    // }

    return HttpStatus::OK;
}

HttpStatus CommandsService::post(const Json::Value& in, int id)
{
    commands["commands"][commands["size"].asUInt()] = in;
    commands["size"] = commands["size"].asUInt() + 1;

    // add also to the historic
    commandsHistoric["commands"][commandsHistoric["size"].asUInt()] = in;
    commandsHistoric["size"] = commandsHistoric["size"].asUInt() + 1;

    return HttpStatus::NO_CONTENT;
}

Json::Value CommandsService::getCommandsHistoric(){
    return commandsHistoric;
}
