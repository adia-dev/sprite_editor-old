//
// Created by abdoulayedia on 16.12.2022
//
#pragma once

#include "components/Components.h"

namespace se {
	// Window
	class Window {
	  public:
		Window();
		~Window();

		bool isOpen() const { return _window != nullptr && _window->isOpen(); }

		void HandleEvents();
		void Update();
		void Render();
		void ShutDown();

		const ImVec2& GetMousePos() const;
		const ImVec2& GetStartLeftMouseButtonPressedPos() const;
		bool          GetIsLeftMouseButtonPressed() const;
		sf::Shader&   GetShader() { return _shader; }

		void ToggleShowGrid() { _showGrid = !_showGrid; }
		bool GetShowGrid() const { return _showGrid; }
		void SetShowGrid(bool state) { _showGrid = state; }

		sf::Font& GetFont() { return _font; }

	  private:
		uint16_t _width  = 2560;
		uint16_t _height = 1600;

		// SFML variables
		sf::RenderWindow* _window = nullptr;
		sf::RenderTexture _renderTexture;
		sf::Clock         _clock;
		sf::Shader        _shader;

		// ImGui variables
		ImFont*  _imFont = nullptr;
		ImGuiIO* _imIO   = nullptr;
		sf::Font _font;
		sf::Text _text;

		ImVec2 _mousePos;
		ImVec2 _startLeftMouseButtonPressedPos;
		bool   _isLeftMouseButtonPressed = false;
		bool   _showGrid                 = true;

		// Init methods
		int Init();

	  public:
		static void      setFancyImguiStyle();
		static sf::Color ImVec4toSFColor(ImVec4 color);
	};
} // namespace se
