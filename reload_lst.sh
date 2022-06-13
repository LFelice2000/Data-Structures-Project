#!/usr/bin/expect -f
# 04
# add test no check if index is loaded
# properly when program is started
set timeout -1
set programName "library"
set filename "test"

spawn rm -f $filename.db $filename.ind $filename.lst

# create index and database
exec "./add_delete_test_02.sh"

#restart program
spawn valgrind --leak-check=full ./$programName best_fit test

# print index
send "printLst\n"
expect "Entry #0"
expect "    offset: #46"
expect "    size: #36"
expect "Entry #1"
expect "    offset: #136"
expect "    size: #36"
expect "Entry #2"
expect "    offset: #90"
expect "    size: #38"

expect "exit"
send "exit\n"
expect "all done"
puts "1) List creating OK, ;-)"
puts "2) Script end"
