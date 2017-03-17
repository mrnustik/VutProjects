ftrest(1)
=========


NAME
----

ftrest - file transfer using REST 

SYNOPSIS
--------

`ftrestd` [`-p` *PORT*] [`-r` *ROOT*]

`ftrest` *\<COMMAND\>* *<REMOTE_PATH\>* [*<LOCAL_PATH>*] 

DESCRIPTION
-----------

File transfer service consists of two main components the server (`ftrestd`) and the client (`ftrest`). The server needs to be running on a computer, you want to store your users files on. The client part is used to read/create/delete users files and folder.


OPTIONS
-------

`-p` *PORT*
  Set the port on which server will be listening. Default is 6677.

`-r` *ROOT FOLDER*
  Sets the directory where server will store the users data. Default is current directory.

*\<COMMAND\>*
  Specifies which command will be used. List of supported commands:
  * `mkd` - creates directory specified in *<REMOTE_PATH\>*
  * `rmd` - removes directory specified in *<REMOTE_PATH\>*
  * `lst` - prints the content of a directory specified in *<REMOTE_PATH\>*
  * `put` - uploads file specified in *<LOCAL_PATH>* to a path specified in *<REMOTE_PATH\>*
  * `get` - downloads file specified in *<REMOTE_PATH\>* to a current directory or to *<LOCAL_PATH>* if specified
  * `del` - deletes file specified in *<REMOTE_PATH>*

*<REMOTE_PATH>* 
  Specifies the remote path of file/folder. Its format is http://SERVER:PORT/USER/PATH, where *SERVER* is an address of computer running `ftrestd`, *PORT* specifies the port that is used by `ftrestd`, *USER* is username that specified the directory within the  *ROOT FOLDER* and *PATH* specifies remote path within the user directory to a file/folder.

*<LOCAL_PATH>*
  Specifies path to a place where the file should be uploaded from or downloaded to.

DIAGNOSTICS
-----------

The following diagnostics may be issued on stderr:

**Not a directory.**
 when *<REMOTE_PATH>* points to a file, but the *<COMMAND\>* used is `lst` or `rmd`

**Directory not found** when *<REMOTE_PATH>* points to a non existing folder and the *<COMMAND\>* used is `lst` or `rmd`

**Directory not empty** when *<COMMAND\>* used is `rmd`and folder specified in *<REMOTE_PATH>* is not empty

**Already exists** 
  when *<REMOTE_PATH>* points to an existing file/folder and the *<COMMAND\>* used is `mkd` or `put`

**Not a file**
  when *<REMOTE_PATH>* points to a folder, but the *<COMMAND\>* used is `del` or `get`

**User Account Not Found**
 if the *USER* specified in *<REMOTE_PATH>* does not exist
 
**Unknown error** 
  for all other errors

BUGS
----

Server is able to handle only one connection at a time.

EXAMPLES
--------

Starting a server on a port 12345 with a root folder /home/
$ ftrestd -p 12354 -r /home/

Creating a new folder on a server running on port 12345 with user tonda:
$ ftrest mkd http://localhost:12345/tonda/foo/bar
 
Uploading a file named doc.pdf to a server that is running on port 12345 with user tonda:
$ ftrest put http://localhost:12345/tonda/foo/bar/doc.pdf ~/doc.pdf
  
Download a file named doc.pdf from a server that is running on port 12345 with user tonda:
$ ftrest get http://localhost:12345/tonda/foo/bar/doc.pdf
 
Removal of a file named doc.pdf from a server that is running on port 12345 with user tonda:
$ ftrest del http://localhost:12345/tonda/foo/bar/doc.pdf
  
Removal of a folder named bar from a server that is running on port 12345 with user tonda:
$ ftrest rmd http://localhost:12345/tonda/foo/bar

AUTHOR
------

Michal Mrnuštík <xmrnus01@stud.fit.vutbr.cz>