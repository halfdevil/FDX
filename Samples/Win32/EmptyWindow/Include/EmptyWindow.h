#pragma once

#include "Game/Win32GameFramework.h"

using namespace FDX;

class CEmptyWindow : public CWin32GameFramework
{
public:

	EResult OnUpdate() override;
	EResult OnDraw() override;
};