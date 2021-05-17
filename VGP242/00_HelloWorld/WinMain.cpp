#include <Core/Inc/Core.h>

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	LOG("Hello %s, 4 + 5 is %d", "World", 4 + 5);
	ASSERT(7 == 42, "7 does not equal to 42");
	return 0;
}