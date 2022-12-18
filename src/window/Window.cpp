//
// Created by abdoulayedia on 16.12.2022
//
#include "Window.h"

namespace se {
	Window::Window() {
		Init();
	}

	Window::~Window() {
		if (_window) {
			delete _window;
		}
	}

	int Window::Init() {
		_window = new sf::RenderWindow(sf::VideoMode(_width, _height),
		                               "ImGui SFML",
		                               sf::Style::Default);
		if (_window == nullptr) {
			std::cerr << "Could not create the window instance.\n";
			return -1;
		}

		if (!ImGui::SFML::Init(*_window)) {
			std::cerr << "Could not init the ImGui SFML window.\n";
			return -1;
		}

		_imIO = &ImGui::GetIO();
		_imIO->ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		_imIO->FontGlobalScale = 1.5f;

		_imFont = _imIO->Fonts->AddFontFromFileTTF(
		    "/Users/abdoulayedia/Projects/Dev/C++/sprite_editor/assets/fonts/"
		    "Poppins/Poppins-Regular.ttf",
		    20);

		setFancyImguiStyle();

		if (!ImGui::SFML::UpdateFontTexture()) {
			std::cerr << "Could not update the font texture.\n";
			return -1;
		}

		return 1;
	}

	void Window::HandleEvents() {
		if (_window == nullptr) return;

		sf::Event event;
		while (_window->pollEvent(event)) {
			ImGui::SFML::ProcessEvent(*_window, event);

			switch (event.type) {
				case sf::Event::Closed:
					_window->close();
					break;
				case sf::Event::KeyPressed:
					if (event.key.code == sf::Keyboard::Escape) {
						_window->close();
					}
					break;
				default:
					break;
			}
		}
	}

	void Window::Update() {
		if (_window == nullptr) return;

		ImGui::SFML::Update(*_window, _clock.restart());
	}

	void Window::Render() {
		static ImVec4      circleColor {1.0f, 0.0f, 1.0f, 0.5f};
		static float       circleRadius = 100.f;
		static std::size_t circlePoints = 30;
		// position, rotation, scale
		static sf::Vector2f position {250, 250};
		static float        rotation = 0.f;
		static sf::Vector2f scale {1, 1};

		sf::CircleShape shape {circleRadius, circlePoints};
		shape.setFillColor(ImVec4toSFColor(circleColor));
		shape.setPosition(position);
		shape.setRotation(rotation);
		shape.setScale(scale);

		static ImVec2     viewportSize {500, 500};
		sf::RenderTexture rt {};
		rt.create(viewportSize.x, viewportSize.y);

		rt.clear(sf::Color(11, 11, 11));

		// draw a grid of lines spaced 50 pixels apart
		sf::VertexArray lines(sf::Lines, 2 * (viewportSize.x + viewportSize.y));
		for (int i = 0; i < viewportSize.x; i += 50) {
			lines[2 * i].color        = sf::Color(50, 50, 50, 100);
			lines[2 * i + 1].color    = sf::Color(50, 50, 50, 100);
			lines[2 * i].position     = sf::Vector2f(i, 0);
			lines[2 * i + 1].position = sf::Vector2f(i, viewportSize.y);
		}

		for (int i = 0; i < viewportSize.y; i += 50) {
			lines[2 * i + 2 * viewportSize.x].color =
			    sf::Color(50, 50, 50, 100);
			lines[2 * i + 2 * viewportSize.x + 1].color =
			    sf::Color(50, 50, 50, 100);
			lines[2 * i + 2 * viewportSize.x].position = sf::Vector2f(0, i);
			lines[2 * i + 2 * viewportSize.x + 1].position =
			    sf::Vector2f(viewportSize.x, i);
		}

		rt.draw(lines);
		rt.draw(shape);

		ImGui::PushFont(_imFont);
		ImGui::DockSpaceOverViewport();

		ImGui::ShowDemoWindow();

		Components::MenuBar(_width);

		static std::vector<std::string> logs;

		if (logs.size() == 0) {
			for (int i = 0; i < 100; i++) {
				logs.push_back("Item " + std::to_string(i));
			}
		}

		Components::Console(logs);

		if (ImGui::Begin("Circle manipulator")) {
			ImGuiColorEditFlags flags = ImGuiColorEditFlags_AlphaBar;
			ImGui::ColorPicker4("Circle Color",
			                    (float*)&circleColor,
			                    flags,
			                    nullptr);

			ImGui::DragFloat("Circle Radius", &circleRadius);
			ImGui::DragInt("Circle Points", (int*)&circlePoints, 1.0f, 3, 500);
			// Position rotation and scale
			ImGui::DragFloat2("Position",
			                  (float*)&position,
			                  1.0f,
			                  0,
			                  viewportSize.x);
			ImGui::DragFloat("Rotation", &rotation, 1.0f, 0, 360);
			ImGui::DragFloat2("Scale", (float*)&scale, 1.0f, 0, 500);
		}
		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));
		if (ImGui::Begin("Viewport")) {
			viewportSize = ImGui::GetWindowSize();
			ImGui::Image(rt);
		}
		ImGui::End();
		ImGui::PopStyleVar();
		ImGui::PopFont();

		_window->clear();
		_window->draw(shape);
		ImGui::SFML::Render(*_window);
		_window->display();
	}

	// Static methods
	void Window::setFancyImguiStyle() {
		ImVec4* colors                    = ImGui::GetStyle().Colors;
		colors[ImGuiCol_Text]             = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		colors[ImGuiCol_TextDisabled]     = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
		colors[ImGuiCol_WindowBg]         = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
		colors[ImGuiCol_ChildBg]          = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_PopupBg]          = ImVec4(0.19f, 0.19f, 0.19f, 0.92f);
		colors[ImGuiCol_Border]           = ImVec4(0.19f, 0.19f, 0.19f, 0.29f);
		colors[ImGuiCol_BorderShadow]     = ImVec4(0.00f, 0.00f, 0.00f, 0.24f);
		colors[ImGuiCol_FrameBg]          = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
		colors[ImGuiCol_FrameBgHovered]   = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
		colors[ImGuiCol_FrameBgActive]    = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
		colors[ImGuiCol_TitleBg]          = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
		colors[ImGuiCol_TitleBgActive]    = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
		colors[ImGuiCol_MenuBarBg]        = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
		colors[ImGuiCol_ScrollbarBg]      = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
		colors[ImGuiCol_ScrollbarGrab]    = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
		colors[ImGuiCol_ScrollbarGrabHovered] =
		    ImVec4(0.40f, 0.40f, 0.40f, 0.54f);
		colors[ImGuiCol_ScrollbarGrabActive] =
		    ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
		colors[ImGuiCol_CheckMark]         = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
		colors[ImGuiCol_SliderGrab]        = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
		colors[ImGuiCol_SliderGrabActive]  = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
		colors[ImGuiCol_Button]            = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
		colors[ImGuiCol_ButtonHovered]     = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
		colors[ImGuiCol_ButtonActive]      = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
		colors[ImGuiCol_Header]            = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
		colors[ImGuiCol_HeaderHovered]     = ImVec4(0.00f, 0.00f, 0.00f, 0.36f);
		colors[ImGuiCol_HeaderActive]      = ImVec4(0.20f, 0.22f, 0.23f, 0.33f);
		colors[ImGuiCol_Separator]         = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
		colors[ImGuiCol_SeparatorHovered]  = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
		colors[ImGuiCol_SeparatorActive]   = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
		colors[ImGuiCol_ResizeGrip]        = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
		colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
		colors[ImGuiCol_ResizeGripActive]  = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
		colors[ImGuiCol_Tab]               = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
		colors[ImGuiCol_TabHovered]        = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
		colors[ImGuiCol_TabActive]         = ImVec4(0.20f, 0.20f, 0.20f, 0.36f);
		colors[ImGuiCol_TabUnfocused]      = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
		colors[ImGuiCol_TabUnfocusedActive] =
		    ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
		colors[ImGuiCol_DockingPreview]   = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
		colors[ImGuiCol_DockingEmptyBg]   = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
		colors[ImGuiCol_PlotLines]        = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
		colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
		colors[ImGuiCol_PlotHistogram]    = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
		colors[ImGuiCol_PlotHistogramHovered] =
		    ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
		colors[ImGuiCol_TableHeaderBg]     = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
		colors[ImGuiCol_TableBorderStrong] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
		colors[ImGuiCol_TableBorderLight]  = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
		colors[ImGuiCol_TableRowBg]        = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_TableRowBgAlt]     = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
		colors[ImGuiCol_TextSelectedBg]    = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
		colors[ImGuiCol_DragDropTarget]    = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
		colors[ImGuiCol_NavHighlight]      = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
		colors[ImGuiCol_NavWindowingHighlight] =
		    ImVec4(1.00f, 0.00f, 0.00f, 0.70f);
		colors[ImGuiCol_NavWindowingDimBg] = ImVec4(1.00f, 0.00f, 0.00f, 0.20f);
		colors[ImGuiCol_ModalWindowDimBg]  = ImVec4(1.00f, 0.00f, 0.00f, 0.35f);

		ImGuiStyle& style       = ImGui::GetStyle();
		style.WindowPadding     = ImVec2(8.00f, 8.00f);
		style.FramePadding      = ImVec2(5.00f, 2.00f);
		style.CellPadding       = ImVec2(6.00f, 6.00f);
		style.ItemSpacing       = ImVec2(6.00f, 6.00f);
		style.ItemInnerSpacing  = ImVec2(6.00f, 6.00f);
		style.TouchExtraPadding = ImVec2(0.00f, 0.00f);
		style.IndentSpacing     = 25;
		style.ScrollbarSize     = 15;
		style.GrabMinSize       = 10;
		style.WindowBorderSize  = 1;
		style.ChildBorderSize   = 1;
		style.PopupBorderSize   = 1;
		style.FrameBorderSize   = 1;
		style.TabBorderSize     = 1;
		style.WindowRounding    = 7;
		style.ChildRounding     = 4;
		style.FrameRounding     = 3;
		style.PopupRounding     = 4;
		style.ScrollbarRounding = 9;
		style.GrabRounding      = 3;
		style.LogSliderDeadzone = 4;
		style.TabRounding       = 4;
	}

	sf::Color Window::ImVec4toSFColor(ImVec4 color) {
		return sf::Color(color.x * 255,
		                 color.y * 255,
		                 color.z * 255,
		                 color.w * 255);
	}
} // namespace se