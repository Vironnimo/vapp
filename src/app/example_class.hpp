#pragma once

#include "vapp/vapp.hpp"

class ExampleClass {
    public:
     int m_testValue = 100;

    public:
     ExampleClass(std::shared_ptr<Vapp::Vapp> vapp);
     ~ExampleClass();

     void databaseExamples();

    private:
     std::shared_ptr<Vapp::Vapp> m_vapp;
};