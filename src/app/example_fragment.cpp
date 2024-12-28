#include "example_fragment.hpp"

void TestFragment::draw() {
    ImGui::BeginChild("example");
    ImGui::Text("geiles zeug hier....");
    ImGui::EndChild();
}
