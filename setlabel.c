#include <Carbon/Carbon.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
  OSStatus ret;
  OSErr err;
  FSRef ref;
  FSCatalogInfo info;
  int colour, n;

  if (argc != 3) {
    fprintf (stderr,
"usage: setlabel <file1> <colour>\n"
"\n"
"Set the label for the specified file to the specified colour.  The colours\n"
"that may be used are as follows:\n"
"\n");
    for (n = 0; n < 8; ++n)
      fprintf (stderr, "  %s\n", colours[n]);

    exit (0);
  }

  colour = -1;
  for (n = 0; n < 8; ++n) {
    if (strcasecmp (argv[2], colours[n]) == 0) {
      colour = n;
      break;
    }
  }

  if (colour == -1) {
    fprintf (stderr, "setlabel: bad colour \'%s\'\n", argv[2]);

    exit (1);
  }

  ret = FSPathMakeRef ((UInt8 *)argv[1], &ref, NULL);

  if (ret != noErr) {
    fprintf (stderr, "setlabel: %s: error %d trying to make FSRef.\n",
             argv[1], (int)ret);
    exit (1);
  }

  err = FSGetCatalogInfo (&ref, kFSCatInfoNodeFlags | kFSCatInfoFinderInfo,
                          &info, NULL, NULL, NULL);

  if (err != noErr) {
    fprintf (stderr,
             "setlabel: %s: error %d trying to get catalog information.\n",
             argv[1], (int)err);
    exit (1);
  }

  if (info.nodeFlags & kFSNodeIsDirectoryMask) {
    FolderInfo *pinfo = (FolderInfo *)&info.finderInfo;
      
    pinfo->finderFlags = (pinfo->finderFlags & ~kColor) | (colour << 1);
  } else {
    FileInfo *pinfo = (FileInfo *)&info.finderInfo;
    
    pinfo->finderFlags = (pinfo->finderFlags & ~kColor) | (colour << 1);
  }

  err = FSSetCatalogInfo (&ref, kFSCatInfoFinderInfo, &info);
  
  if (err != noErr) {
    fprintf (stderr, "setlabel: %s: error %d trying to set catalog "
             "information.\n",
             argv[1], (int)err);
    exit (1);
  }

  return 0;
}
