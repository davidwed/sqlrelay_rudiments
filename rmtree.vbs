' sanity check
if WScript.Arguments.Count<1 then
	WScript.quit
end if

' create file system object
set fso=CreateObject("Scripting.FileSystemObject")

' get the folder to remove
folder=WScript.Arguments.Item(WScript.Arguments.Count-1)

' collapse backslashes and convert slashes to backslashes
folder=replace(folder,"\\","\",1,-1,0)
folder=replace(folder,"/","\",1,-1,0)

' remove the folder
if fso.FolderExists(folder) then
	call fso.DeleteFolder(folder,true)
end if
