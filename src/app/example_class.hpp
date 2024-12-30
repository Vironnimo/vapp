#pragma once

#include "vapp/vapp.hpp"

class ExampleClass {
    public:
     ExampleClass();
     ~ExampleClass();
     int m_testValue = 100;
    public:
     ExampleClass(std::shared_ptr<Vapp::Vapp> vapp);
    private:
     std::shared_ptr<Vapp::Vapp> m_vapp;
};