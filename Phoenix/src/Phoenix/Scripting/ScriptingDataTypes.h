#pragma once

namespace phx {
	enum ScriptingDataType {
		Null,
		Number,
		Bool,
		String,
		Function,
		Thread,
		Table,		   // List
		LightUserData, // Pointer
		UserData,	   // Custom data type
	};
}