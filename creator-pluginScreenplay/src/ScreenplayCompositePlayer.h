#ifndef __SCREENPLAYCOMPOSITEPLAYER_H__
#define __SCREENPLAYCOMPOSITEPLAYER_H__

#include "Composite.h"

class QRadioButton;
class QWidget;

namespace Screenplay
{
	class PlayerUI;

	class PlayerView : public Composite::View
	{
	public:
		QWidget* GetRoot();
		void AttachCanvas(QWidget* _canvas);
	protected:
		void setupUI() override;
	private:
		PlayerUI* ui_;
	};

	class PlayerComponent : public Composite::MVCComponent
	{
	public:
		PlayerComponent(PlayerView* _view);
	protected:
		void initialize(Composite::Core* _core) override;
	private:
	};
}

#endif
