#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <stdlib.h>
#include <dirent.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>

#include "register_res.h"

#define ARRAY_LEN(array) \
    (sizeof(array)/sizeof(array[0]))

static BOOL register_each_item (char *dname, int flag)
{
    struct dirent *e;
    DIR *dir;
    char buff [120];
	char *p;

	SetResPath ("./res/");
    dir = opendir (dname);
    if (!dir)
        return -1;
    
    while ((e = readdir (dir)) != NULL){
        if (!strstr (e->d_name, ".gif") && !strstr (e->d_name, ".png") && \
			!strstr (e->d_name, ".bmp") && !strstr (e->d_name, ".jpg")) {
            continue;
        }

        strcpy (buff, dname);
        strcat (buff, e->d_name);
		if((p= strchr(buff, '/')) != NULL) {
			p = p+1;
		} else {
			p = buff;
		}
		if(flag == 1) {
			if (RegisterResFromFile (HDC_SCREEN, p) == FALSE)
			{
				fprintf (stderr, "can't register %s\n", p);
				//printf ("can't register %s\n", p);
				return FALSE;
			}
		} else {
			UnregisterRes(p);
		}

    }
    closedir (dir);
    return TRUE;
}

void register_pic(char *dname)
{
	//printf("register_pic %s\n", dname);
	register_each_item(dname, 1);
}

void un_register_pic(char *dname)
{
	register_each_item(dname, 0);
}
