#pragma once
#include <ui/common/StandaloneWindow.h>

class CameraManagerInspector : public StandaloneWindow
{
public:
    CameraManagerInspector(csl::fnd::IAllocator* allocator);

    //virtual void PreRender() override;
    virtual void RenderContents() override;
};

