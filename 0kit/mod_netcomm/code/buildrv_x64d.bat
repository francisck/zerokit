ml64.exe /Fo "..\obj\x64\debug\private.obj" /c amd64\private.asm
ml64.exe /Fo "..\obj\x64\debug\mod_netcommApi.obj" /c amd64\mod_netcommApi.asm
cl.exe @cl_x64d.rsp main.c
link.exe @link_x64d.rsp