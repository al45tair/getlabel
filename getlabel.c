#include <Carbon/Carbon.h>
#include <stdio.h>
#include <stdlib.h>

const char *colours[] = {
  "none",
  "grey",
  "green",
  "purple",
  "blue",
  "yellow",
  "red",
  "orange"
};

int
main (int argc, char **argv)
{
  int n;

  if (argc < 2) {
    fprintf (stderr,
"usage: getlabel <file1> [<file2> ...]\n"
"\n"
"For each file listed on the command line, return the colour of its\n"
"label.  The colours that may be returned are as follows:\n"
"\n");
    for (n = 0; n < 8; ++n)
      fprintf (stderr, "  %s\n", colours[n]);

    exit (0);
  }

  for (n = 1; n < argc; ++n) {
    OSStatus ret;
    OSErr err;
    FSRef ref;
    FSCatalogInfo info;
    UInt16 flags;
    int colour;

    ret = FSPathMakeRef ((UInt8 *)argv[n], &ref, NULL);

    if (ret != noErr) {
      fprintf (stderr, "getlabel: %s: error %d trying to make FSRef.\n",
               argv[n], (int)ret);
      exit (1);
    }

    err = FSGetCatalogInfo (&ref, kFSCatInfoNodeFlags | kFSCatInfoFinderInfo,
                            &info, NULL, NULL, NULL);

    if (err != noErr) {
      fprintf (stderr, 
               "getlabel: %s: error %d trying to get catalog information.\n",
               argv[n], (int)err);
      exit (1);
    }

    if (info.nodeFlags & kFSNodeIsDirectoryMask) {
      FolderInfo *pinfo = (FolderInfo *)&info.finderInfo;
      
      flags = pinfo->finderFlags;
    } else {
      FileInfo *pinfo = (FileInfo *)&info.finderInfo;

      flags = pinfo->finderFlags;
    }

    colour = (flags & kColor) >> 1;

    puts (colours[colour]);
  }

  return 0;
}
