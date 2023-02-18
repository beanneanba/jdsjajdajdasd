#include "memory.h"

#include <thread>

namespace offsets
{
	// client.dll
	constexpr auto localplayer = 0xDEA964;
	constexpr auto entitylist = 0x4DFFEF4;
	constexpr auto glowObjectManager = 0x535A9C8;

	// entity
	constexpr auto teamnum = 0xF4;
	constexpr auto glowIndex = 0x10488;
}

struct Color
{
	constexpr Color(float r, float g, float b, float a = 1.f) noexcept :
		r(r), g(g), b(b), a(a) { }

	float r, g, b, a;
};

int main()
{
	auto mem = Memory{ "csgo.exe" };

	const auto client = mem.GetModuleAddress("client.dll");

	const auto color = Color{ 1.f, 1.f, 0.f };

	while (true)
	{
		const auto localplayer = mem.Read<std::uintptr_t>(client + offsets::localplayer);
		const auto glowObjectManager = mem.Read<std::uintptr_t>(client + offsets::glowObjectManager);
		
		for (auto i = 0; i < 64; ++i)
		{
			const auto entity = mem.Read<std::uintptr_t>(client + offsets::entitylist + i * 0x10);

			if (mem.Read<std::uintptr_t>(entity + offsets::teamnum) == mem.Read<std::uintptr_t>(localplayer + offsets::teamnum))
				continue;

			const auto glowIndex = mem.Read<std::int32_t>(entity + offsets::glowIndex);

			mem.Write<Color>(glowObjectManager + (glowIndex * 0x38) + 0x8, color); // color
			
			mem.Write<bool>(glowObjectManager + (glowIndex * 0x38) + 0x27, true);
			mem.Write<bool>(glowObjectManager + (glowIndex * 0x38) + 0x28, true);
		}
	
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	return 0;

}