NAME
----

ftrest - file transfer using REST 

SYNOPSIS
--------

`ftrestd` [`-p` *PORT*] [`-r` *ROOT*]

`ftrest` *COMMAND* *URL* [*LOCAL_PATH*] 

DESCRIPTION
-----------




OPTIONS
-------

`-b`
  Do not write "busy" to stdout while processing.

`-c` *config-file*
  Use the alternate system wide *config-file* instead of */etc/foo.conf*. This
  overrides any `FOOCONF` environment variable.

`-a`
  In addition to the baz segments, also parse the blurfl headers.

`-r`
  Recursive mode. Operates as fast as lightning at the expense of a megabyte
  of virtual memory.


DIAGNOSTICS
-----------

The following diagnostics may be issued on stderr:

**Bad magic number.**
  The input file does not look like an archive file.

**Old style baz segments.**
  `foo` can only handle new style baz segments. COBOL object libraries are not
  supported in this version.

BUGS
----

The command name should have been chosen more carefully to reflect its
purpose.

AUTHOR
------

Jens Schweikhardt <howto@schweikhardt.net>