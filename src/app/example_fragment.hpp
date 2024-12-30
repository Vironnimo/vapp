#pragma once

#include "vapp/gui/i_fragment.hpp"

class TestFragment : public Vapp::IFragment {
   public:
    TestFragment();
    ~TestFragment();
    void draw() override;

   private:
};