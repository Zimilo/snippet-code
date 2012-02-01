/**
 * zimilo@code-trick.com
 */

#ifndef __ZIACK_VERSION_H__
#define __ZIACK_VERSION_H__

#define ZIACK_MAJOR_VERSION "0"
#define ZIACK_MINOR_VERSION "1"

char *
ziack_version_major()
{
  return ZIACK_MAJOR_VERSION;
}

char *
ziack_version_minor()
{
  return ZIACK_MINOR_VERSION;
}

#endif // !__ZIACK_VERSION_H__
