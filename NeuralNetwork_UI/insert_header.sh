#!/bin/sh
# insert_header.sh
# Insert top comments to .h and .cpp file.

# print data in format mm-dd-year
date=`date '+%m-%d-20%y'`

# process all .h, cpp files
for file in *.h *.cpp *.c
do 
	echo $file

# define header lines (file name, file info, copyright, etc.)
header_lines=\
"\/\*\*	\n \
	$file \n \
	Neural Network Designer project \n \
	since 2017 - 2017 (initiated by Shi, Guoyong) \n \
\n \
	Mixed-Signal Design Automation Lab ($date)	\n \
\*\*\/\n\n"

	# insert lines to file head
	sed -i "1i $header_lines" $file
done


