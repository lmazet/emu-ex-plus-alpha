#pragma once

/*  This file is part of EmuFramework.

	Imagine is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Imagine is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with EmuFramework.  If not, see <http://www.gnu.org/licenses/> */

#include <imagine/gui/TableView.hh>
#include <imagine/gui/AlertView.hh>

class InputManagerView;
struct InputDeviceConfig;
struct KeyCategory;

class ButtonConfigSetView : public View
{
private:
	typedef DelegateFunc<void (Input::Event e)> SetDelegate;

	IG::WindowRect viewFrame{};
	#ifdef CONFIG_INPUT_POINTING_DEVICES
	IG::WindowRect unbindB{}, cancelB{};
	#endif
	std::array<char, 128> str{};
	std::array<char, 24> actionStr{};
	Gfx::Text text{};
	#ifdef CONFIG_INPUT_POINTING_DEVICES
	Gfx::Text unbind{}, cancel{};
	#endif
	SetDelegate onSetD{};
	const Input::Device &dev;
	const Input::Device *savedDev{};
	InputManagerView &rootIMView;

	void initPointerUI();
	bool pointerUIIsInit();

public:
	ButtonConfigSetView(ViewAttachParams attach, InputManagerView &rootIMView,
		Input::Device &dev, const char *actionName, SetDelegate onSet);
	~ButtonConfigSetView();
	IG::WindowRect &viewRect() final { return viewFrame; }
	void place() final;
	bool inputEvent(Input::Event e) final;
	void draw(Gfx::RendererCommands &cmds) final;
	void onAddedToController(Input::Event e) final;
};

class ButtonConfigView : public TableView
{
private:
	struct BtnConfigMenuItem : public DualTextMenuItem
	{
		using DualTextMenuItem::DualTextMenuItem;
		void draw(Gfx::RendererCommands &cmds, Gfx::GC xPos, Gfx::GC yPos, Gfx::GC xSize, Gfx::GC ySize, _2DOrigin align, const Gfx::ProjectionPlane &projP) const final;
	};

	InputManagerView &rootIMView;
	TextMenuItem reset{};
	using KeyNameStr = std::array<char, 20>;
	struct BtnConfigEntry
	{
		BtnConfigMenuItem item{nullptr, ""};
		KeyNameStr str{};

		BtnConfigEntry()
		{
			item.t2.setString(str.data());
		}
	} *btn{};
	const KeyCategory *cat{};
	InputDeviceConfig *devConf{};
	Input::Time leftKeyPushTime{};
	std::array<char, 96> conflictStr{};

	void onSet(Input::Key mapKey, int keyToSet);
	static KeyNameStr makeKeyNameStr(Input::Key key, const char *name);

public:
	ButtonConfigView(ViewAttachParams attach, InputManagerView &rootIMView, const KeyCategory *cat, InputDeviceConfig &devConf);
	~ButtonConfigView() final;
	bool inputEvent(Input::Event e) final;
};
