#include "Application.h"

class SandboxApp : public VulkanApi::Application
{
public:

};

int main()
{
    auto* app = new SandboxApp();
    app->Run();
    delete app;

    return 0;
}
