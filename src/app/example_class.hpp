#pragma once

#include "vapp/core/actions.hpp"

class ExampleClass {
    public:
     int m_testValue = 100;
    public:
     ExampleClass(std::shared_ptr<Vapp::Actions> actions);
    private:
     std::shared_ptr<Vapp::Actions> m_actions;
};