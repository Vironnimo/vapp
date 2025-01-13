#pragma once

#include <functional>
#include <string>
#include <unordered_map>

namespace Vapp {

class EventSystem {
   public:
    EventSystem();
    ~EventSystem();

    void subscribe(const std::string& eventName, std::function<void(void*)> callback);
    void emit(const std::string& eventName, void* data = nullptr);

   private:
    std::unordered_map<std::string, std::vector<std::function<void(void*)>>> m_observers;
};

}  // namespace Vapp