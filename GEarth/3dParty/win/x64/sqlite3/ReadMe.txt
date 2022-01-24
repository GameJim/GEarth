SQLite3 版本号3240000

bin下载地址：https://www.sqlite.org/2018/sqlite-dll-win64-x64-3240000.zip

include下载地址：https://www.sqlite.org/2018/sqlite-amalgamation-3240000.zip

lib：
下载sqlite-dll-win64-x64-3240000.zip后解压，里面没有lib文件，里面有个sqlite3.def文件。
打开Developer Command Prompt for VS2015，运行lib /DEF:sqlite3.def /MACHINE:x64 /OUT:sqlite3.lib，生成lib文件。