#pragma once

#include "Game/OSXGameFramework.h"

using namespace FDX;

class COSXSample : public COSXGameFramework
{
public:

    EResult OnUpdate() override;
    EResult OnDraw() override;
};