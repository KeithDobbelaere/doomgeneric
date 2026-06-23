#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>

int mkdir(const char* path, mode_t mode)
{
	(void)path;
	(void)mode;

	// PicoCalc target has no filesystem wired yet.
	// Pretend directory creation succeeded so Doom can continue.
	return 0;
}