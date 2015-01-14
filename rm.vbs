' sanity check
if WScript.Arguments.Count<1 then
	WScript.quit
end if

' create file system object
set fso=CreateObject("Scripting.FileSystemObject")

' get the file to remove
file=WScript.Arguments.Item(WScript.Arguments.Count-1)

' collapse backslashes and convert slashes to backslashes
file=replace(file,"\\","\",1,-1,0)
file=replace(file,"/","\",1,-1,0)

' remove the file
call fso.DeleteFile(file)
