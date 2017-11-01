Readme.txt

	By Guoyong Shi, 2017/7/11

To compile, go to ./Main.

	$ make -f Make_Line_Parser

	This makes the parser generation from "CmdLineScanner.lex" and
	"CmdLineParser.yy".

	$ make

	This makes the executable "QStudio.exe".

In the "Main" folder, there is a Qt project file "Main.pro". You can see
the source code structure for this Qt project. The header and CPP source
files auto-generated for parser are created by running the first make above.

Do NOT modify the pro file unless you completely understand how to use
Qt project file.

In case you modify the pro file, you may regenerate the Makefile by

	$ qmake

Do NOT run "$ qmake -project" because this will regenerate "Main.pro".
Hence make sure to back up the file first.


