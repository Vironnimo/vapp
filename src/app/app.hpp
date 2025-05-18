#pragma once

#include <memory>

#include "example_class.hpp"
#include "vapp/vapp.hpp"

class App {
   public:
    int m_appValue = 1337;

    App();
    ~App();
    void loadResources();

    void run();
    void testStuff();

   private:
    std::shared_ptr<Vapp::Vapp> m_vapp;
    std::unique_ptr<ExampleClass> m_exampleClass;

    void init();
};