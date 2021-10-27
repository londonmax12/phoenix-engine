#pragma once

namespace phx
{
	typedef enum class KeyCode : uint16_t
	{
		// From glfw3.h
		Space = 32,
		Apostrophe = 39, /* ' */
		Comma = 44, /* , */
		Minus = 45, /* - */
		Period = 46, /* . */
		Slash = 47, /* / */

		D0 = 48, /* 0 */
		D1 = 49, /* 1 */
		D2 = 50, /* 2 */
		D3 = 51, /* 3 */
		D4 = 52, /* 4 */
		D5 = 53, /* 5 */
		D6 = 54, /* 6 */
		D7 = 55, /* 7 */
		D8 = 56, /* 8 */
		D9 = 57, /* 9 */

		Semicolon = 59, /* ; */
		Equal = 61, /* = */

		A = 65,
		B = 66,
		C = 67,
		D = 68,
		E = 69,
		F = 70,
		G = 71,
		H = 72,
		I = 73,
		J = 74,
		K = 75,
		L = 76,
		M = 77,
		N = 78,
		O = 79,
		P = 80,
		Q = 81,
		R = 82,
		S = 83,
		T = 84,
		U = 85,
		V = 86,
		W = 87,
		X = 88,
		Y = 89,
		Z = 90,

		LeftBracket = 91,  /* [ */
		Backslash = 92,  /* \ */
		RightBracket = 93,  /* ] */
		GraveAccent = 96,  /* ` */

		World1 = 161, /* non-US #1 */
		World2 = 162, /* non-US #2 */

		/* Function keys */
		Escape = 256,
		Enter = 257,
		Tab = 258,
		Backspace = 259,
		Insert = 260,
		Delete = 261,
		Right = 262,
		Left = 263,
		Down = 264,
		Up = 265,
		PageUp = 266,
		PageDown = 267,
		Home = 268,
		End = 269,
		CapsLock = 280,
		ScrollLock = 281,
		NumLock = 282,
		PrintScreen = 283,
		Pause = 284,
		F1 = 290,
		F2 = 291,
		F3 = 292,
		F4 = 293,
		F5 = 294,
		F6 = 295,
		F7 = 296,
		F8 = 297,
		F9 = 298,
		F10 = 299,
		F11 = 300,
		F12 = 301,
		F13 = 302,
		F14 = 303,
		F15 = 304,
		F16 = 305,
		F17 = 306,
		F18 = 307,
		F19 = 308,
		F20 = 309,
		F21 = 310,
		F22 = 311,
		F23 = 312,
		F24 = 313,
		F25 = 314,

		/* Keypad */
		KP0 = 320,
		KP1 = 321,
		KP2 = 322,
		KP3 = 323,
		KP4 = 324,
		KP5 = 325,
		KP6 = 326,
		KP7 = 327,
		KP8 = 328,
		KP9 = 329,
		KPDecimal = 330,
		KPDivide = 331,
		KPMultiply = 332,
		KPSubtract = 333,
		KPAdd = 334,
		KPEnter = 335,
		KPEqual = 336,

		LeftShift = 340,
		LeftControl = 341,
		LeftAlt = 342,
		LeftSuper = 343,
		RightShift = 344,
		RightControl = 345,
		RightAlt = 346,
		RightSuper = 347,
		Menu = 348
	} Key;

	inline std::ostream& operator<<(std::ostream& os, KeyCode keyCode)
	{
		os << static_cast<int32_t>(keyCode);
		return os;
	}
}

// From glfw3.h
#define PHX_KEY_SPACE           ::phx::Key::Space
#define PHX_KEY_APOSTROPHE      ::phx::Key::Apostrophe    /* ' */
#define PHX_KEY_COMMA           ::phx::Key::Comma         /* , */
#define PHX_KEY_MINUS           ::phx::Key::Minus         /* - */
#define PHX_KEY_PERIOD          ::phx::Key::Period        /* . */
#define PHX_KEY_SLASH           ::phx::Key::Slash         /* / */
#define PHX_KEY_0               ::phx::Key::D0
#define PHX_KEY_1               ::phx::Key::D1
#define PHX_KEY_2               ::phx::Key::D2
#define PHX_KEY_3               ::phx::Key::D3
#define PHX_KEY_4               ::phx::Key::D4
#define PHX_KEY_5               ::phx::Key::D5
#define PHX_KEY_6               ::phx::Key::D6
#define PHX_KEY_7               ::phx::Key::D7
#define PHX_KEY_8               ::phx::Key::D8
#define PHX_KEY_9               ::phx::Key::D9
#define PHX_KEY_SEMICOLON       ::phx::Key::Semicolon     /* ; */
#define PHX_KEY_EQUAL           ::phx::Key::Equal         /* = */
#define PHX_KEY_A               ::phx::Key::A
#define PHX_KEY_B               ::phx::Key::B
#define PHX_KEY_C               ::phx::Key::C
#define PHX_KEY_D               ::phx::Key::D
#define PHX_KEY_E               ::phx::Key::E
#define PHX_KEY_F               ::phx::Key::F
#define PHX_KEY_G               ::phx::Key::G
#define PHX_KEY_H               ::phx::Key::H
#define PHX_KEY_I               ::phx::Key::I
#define PHX_KEY_J               ::phx::Key::J
#define PHX_KEY_K               ::phx::Key::K
#define PHX_KEY_L               ::phx::Key::L
#define PHX_KEY_M               ::phx::Key::M
#define PHX_KEY_N               ::phx::Key::N
#define PHX_KEY_O               ::phx::Key::O
#define PHX_KEY_P               ::phx::Key::P
#define PHX_KEY_Q               ::phx::Key::Q
#define PHX_KEY_R               ::phx::Key::R
#define PHX_KEY_S               ::phx::Key::S
#define PHX_KEY_T               ::phx::Key::T
#define PHX_KEY_U               ::phx::Key::U
#define PHX_KEY_V               ::phx::Key::V
#define PHX_KEY_W               ::phx::Key::W
#define PHX_KEY_X               ::phx::Key::X
#define PHX_KEY_Y               ::phx::Key::Y
#define PHX_KEY_Z               ::phx::Key::Z
#define PHX_KEY_LEFT_BRACKET    ::phx::Key::LeftBracket   /* [ */
#define PHX_KEY_BACKSLASH       ::phx::Key::Backslash     /* \ */
#define PHX_KEY_RIGHT_BRACKET   ::phx::Key::RightBracket  /* ] */
#define PHX_KEY_GRAVE_ACCENT    ::phx::Key::GraveAccent   /* ` */
#define PHX_KEY_WORLD_1         ::phx::Key::World1        /* non-US #1 */
#define PHX_KEY_WORLD_2         ::phx::Key::World2        /* non-US #2 */

/* Function keys */
#define PHX_KEY_ESCAPE          ::phx::Key::Escape
#define PHX_KEY_ENTER           ::phx::Key::Enter
#define PHX_KEY_TAB             ::phx::Key::Tab
#define PHX_KEY_BACKSPACE       ::phx::Key::Backspace
#define PHX_KEY_INSERT          ::phx::Key::Insert
#define PHX_KEY_DELETE          ::phx::Key::Delete
#define PHX_KEY_RIGHT           ::phx::Key::Right
#define PHX_KEY_LEFT            ::phx::Key::Left
#define PHX_KEY_DOWN            ::phx::Key::Down
#define PHX_KEY_UP              ::phx::Key::Up
#define PHX_KEY_PAGE_UP         ::phx::Key::PageUp
#define PHX_KEY_PAGE_DOWN       ::phx::Key::PageDown
#define PHX_KEY_HOME            ::phx::Key::Home
#define PHX_KEY_END             ::phx::Key::End
#define PHX_KEY_CAPS_LOCK       ::phx::Key::CapsLock
#define PHX_KEY_SCROLL_LOCK     ::phx::Key::ScrollLock
#define PHX_KEY_NUM_LOCK        ::phx::Key::NumLock
#define PHX_KEY_PRINT_SCREEN    ::phx::Key::PrintScreen
#define PHX_KEY_PAUSE           ::phx::Key::Pause
#define PHX_KEY_F1              ::phx::Key::F1
#define PHX_KEY_F2              ::phx::Key::F2
#define PHX_KEY_F3              ::phx::Key::F3
#define PHX_KEY_F4              ::phx::Key::F4
#define PHX_KEY_F5              ::phx::Key::F5
#define PHX_KEY_F6              ::phx::Key::F6
#define PHX_KEY_F7              ::phx::Key::F7
#define PHX_KEY_F8              ::phx::Key::F8
#define PHX_KEY_F9              ::phx::Key::F9
#define PHX_KEY_F10             ::phx::Key::F10
#define PHX_KEY_F11             ::phx::Key::F11
#define PHX_KEY_F12             ::phx::Key::F12
#define PHX_KEY_F13             ::phx::Key::F13
#define PHX_KEY_F14             ::phx::Key::F14
#define PHX_KEY_F15             ::phx::Key::F15
#define PHX_KEY_F16             ::phx::Key::F16
#define PHX_KEY_F17             ::phx::Key::F17
#define PHX_KEY_F18             ::phx::Key::F18
#define PHX_KEY_F19             ::phx::Key::F19
#define PHX_KEY_F20             ::phx::Key::F20
#define PHX_KEY_F21             ::phx::Key::F21
#define PHX_KEY_F22             ::phx::Key::F22
#define PHX_KEY_F23             ::phx::Key::F23
#define PHX_KEY_F24             ::phx::Key::F24
#define PHX_KEY_F25             ::phx::Key::F25

/* Keypad */
#define PHX_KEY_KP_0            ::phx::Key::KP0
#define PHX_KEY_KP_1            ::phx::Key::KP1
#define PHX_KEY_KP_2            ::phx::Key::KP2
#define PHX_KEY_KP_3            ::phx::Key::KP3
#define PHX_KEY_KP_4            ::phx::Key::KP4
#define PHX_KEY_KP_5            ::phx::Key::KP5
#define PHX_KEY_KP_6            ::phx::Key::KP6
#define PHX_KEY_KP_7            ::phx::Key::KP7
#define PHX_KEY_KP_8            ::phx::Key::KP8
#define PHX_KEY_KP_9            ::phx::Key::KP9
#define PHX_KEY_KP_DECIMAL      ::phx::Key::KPDecimal
#define PHX_KEY_KP_DIVIDE       ::phx::Key::KPDivide
#define PHX_KEY_KP_MULTIPLY     ::phx::Key::KPMultiply
#define PHX_KEY_KP_SUBTRACT     ::phx::Key::KPSubtract
#define PHX_KEY_KP_ADD          ::phx::Key::KPAdd
#define PHX_KEY_KP_ENTER        ::phx::Key::KPEnter
#define PHX_KEY_KP_EQUAL        ::phx::Key::KPEqual

#define PHX_KEY_LEFT_SHIFT      ::phx::Key::LeftShift
#define PHX_KEY_LEFT_CONTROL    ::phx::Key::LeftControl
#define PHX_KEY_LEFT_ALT        ::phx::Key::LeftAlt
#define PHX_KEY_LEFT_SUPER      ::phx::Key::LeftSuper
#define PHX_KEY_RIGHT_SHIFT     ::phx::Key::RightShift
#define PHX_KEY_RIGHT_CONTROL   ::phx::Key::RightControl
#define PHX_KEY_RIGHT_ALT       ::phx::Key::RightAlt
#define PHX_KEY_RIGHT_SUPER     ::phx::Key::RightSuper
#define PHX_KEY_MENU            ::phx::Key::Menu