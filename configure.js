fso=new ActiveXObject("Scripting.FileSystemObject")
fso.CopyFile("config.windows.mk config.mk")
fso.CopyFile("include\\rudiments\\private\\config.windows.h include\\rudiments\\private\\config.h")
