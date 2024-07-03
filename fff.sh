for i in "
./sysdeps/e2k/strchr.c
./sysdeps/e2k/strchrnul.c
./sysdeps/e2k/strncat.c
./sysdeps/e2k/strncmp.c
./sysdeps/e2k/stpcpy.c
./sysdeps/e2k/strncase.c
./sysdeps/e2k/memccpy.c
./sysdeps/e2k/strcmp.c
./sysdeps/e2k/memcpy.c
./sysdeps/e2k/strspn.c
./sysdeps/e2k/strcasecmp.c
./sysdeps/e2k/strcpy.c
./sysdeps/e2k/strcat.c
./sysdeps/e2k/stpncpy.c
./sysdeps/e2k/strlen.c
./sysdeps/e2k/rawmemchr.c
./sysdeps/e2k/strnlen.c
./sysdeps/e2k/memcmp.c
./sysdeps/e2k/strcspn.c
./sysdeps/e2k/strpbrk.c
./sysdeps/e2k/memrchr.c
./sysdeps/e2k/strncpy.c
./sysdeps/e2k/strrchr.c
./sysdeps/e2k/memchr.c
" ; do 
sed -i -e '/@(#) $Id:/d' ${i}
done
