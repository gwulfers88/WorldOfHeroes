@echo off

set WildCard=*.h *.cpp *.c *.inl
set /p FindString="What do you want to find: "

set hour=%TIME:~0,2%
set min=%TIME:~3,2%
set sec=%TIME:~6,2%

echo ******************
echo ******************

echo task started at %hour%:%min%:%sec%
echo %FindString%'s FOUND:
findstr -s -n -i -l %FindString% %WildCard%

echo ******************
echo ******************
