#pragma once

#include <string>
#include "vapp/core/network.hpp"

namespace Vapp {

struct AiCommand {
    std::string command;
    std::string args;
};

class AiRequest {
   public:
    AiRequest();
    ~AiRequest();

    void init(std::shared_ptr<Network> network);

    AiCommand requestCommand(const std::string& promptMessage);

   private:
    std::shared_ptr<Network> m_network;
};

}  // namespace Vapp