#pragma once

#include "Core/Core.h"
#include "Graphics/Canvas.h"
#include "GUI/ControlEvent.h"
#include "GUI/ControlEventHandler.h"
#include "GUI/ControlSkin.h"
#include "GUI/ControlFont.h"

namespace FDX
{
	class CControlContainer;

	enum class EAlignVertical
	{
		DontCare,
		Top,
		Bottom,
		Center
	};

	enum class EAlignHorizontal
	{
		DontCare,
		Left,
		Right,
		Center
	};

	enum EControlFillMode
	{
		DontCare = 0,
		Horizontal = 1,
		Vertical = 2
	};

	class CControl;
	using CControlPtr = SmartPtr<CControl>;

	class FDX_EXPORT CControl : public CControlEventHandler
	{
	public:

		friend class CControlEventHandler;

		CControl(const String &pstrName);
		virtual ~CControl();

		const String& GetName() const
		{
			return mstrName;
		}

		int32_t GetX() const
		{
			return miX;
		}

		int32_t GetY() const
		{
			return miY;
		}

		uint32_t GetWidth() const
		{
			return muiWidth;
		}

		uint32_t GetHeight() const
		{
			return muiHeight;
		}

		int32_t GetScreenX() const
		{
			return miScreenX;
		}

		int32_t GetScreenY() const
		{
			return miScreenY;
		}

		int32_t GetDepth() const
		{
			return miDepth;
		}

		float GetParentFractionX() const
		{
			return mfParentFractionX;
		}

		float GetParentFractionY() const
		{
			return mfParentFractionY;
		}

		EAlignVertical GetAlignV() const
		{
			return mAlignV;
		}

		EAlignHorizontal GetAlignH() const
		{
			return mAlignH;
		}

		uint32_t GetFillMode() const
		{
			return muiFillMode;
		}

		CControlSkin* GetSkin() const
		{
			return mSkin.get();
		}

		template <typename T>
		T* GetSkinAs() const
		{
			return static_cast<T*>(mSkin.get());
		}

		const Array<CControl*>& GetChildControls() const
		{
			return mChildControls;
		}

		CControl* GetChildControl(uint32_t puiIdx)
		{
			if (puiIdx < mChildControls.size())
				return mChildControls[puiIdx];

			return nullptr;
		}

		template <typename T>
		T* GetChildControlAs(uint32_t puiIdx)
		{
			if (puiIdx < mChildControls.size())
				return static_cast<T*>(mChildControls[puiIdx]);

			return nullptr;
		}

		virtual EResult Create(CControlContainer *pContainer);

		virtual bool IsEnabled() const;
		virtual bool IsVisble() const;
		virtual bool IsPointOver(int32_t puiX, int32_t puiY) const;

		virtual void SetName(const String &pstrName);
		virtual void SetX(int32_t puiX);
		virtual void SetY(int32_t puiY);
		virtual void SetPosition(int32_t puiX, int32_t puiY);
		virtual void SetSize(uint32_t puiWidth, uint32_t puiHeight);
		virtual void SetDepth(int32_t piDepth);
		virtual void SetWidth(uint32_t puiWidth);
		virtual void SetHeight(uint32_t puiHeight);
		virtual void SetParentFractionX(float pfFractionX);
		virtual void SetParentFractionY(float pfFractionY);

		virtual void SetAlignV(EAlignVertical pAlignV);
		virtual void SetAlignH(EAlignHorizontal pAlignH);
		virtual void SetFillMode(uint32_t pFillMode);

		virtual void SetEnabled(bool pbEnabled);
		virtual void SetVisible(bool pbVisible);
		virtual void SetSkin(CControlSkinPtr pSkin);

		virtual void AddChild(CControl *pControl);
		virtual void RemoveChild(CControl *pControl);

		virtual void Update(float pfDelta);
		virtual void Draw(CCanvas *pCanvas);	

		virtual void UpdateScreenPosition(uint32_t puiParentX, 
			uint32_t puiParentY);

		virtual void LayoutChildControls();
		virtual void BringToFront();
		virtual void SendToBack();
		virtual void SortChildren();
						
	protected:

		virtual void PreDraw(CCanvas *pCanvas);
		virtual void PostDraw(CCanvas *pCanvas);

	protected:

		String mstrName;
		int32_t miX;
		int32_t miY;
		int32_t miScreenX;
		int32_t miScreenY;
		int32_t miDepth;
		uint32_t muiWidth;
		uint32_t muiHeight;

		float mfParentFractionX;
		float mfParentFractionY;
		EAlignVertical mAlignV;
		EAlignHorizontal mAlignH;
		uint32_t muiFillMode;
		
		CControl *mParent;
		CControlSkinPtr mSkin;
		Array<CControl*> mChildControls;

		bool mbIsEnabled;
		bool mbIsVisible;
		bool mbIsPositionChanged;
	};
}