#Creates folder build will compiled files and runs main file "libshell"
cc libshell.c -o libshell
cc reader_shell.c -o reader
cc catalog_shell.c -o catalog
cc archive_shell.c -o archive
./build/libshell