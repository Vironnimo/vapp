#pragma once

#include <memory>

#include "vapp/vapp.hpp"
#include "example_class.hpp"

class App {
   public:
    App();
    void run();
    void testStuff();

   private:
    std::unique_ptr<Vapp::Vapp> m_vapp;
    ExampleClass* m_exampleClass = nullptr;

   private:
    void init();

};