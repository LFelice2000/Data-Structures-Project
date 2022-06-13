#!/usr/bin/expect -f
# 05

# b) check that list of deleted books is created
##     and file with list is saved
# b.1 firstfit
set timeout -1
set programName "library"
set filename "test"

# delete all files starting with test
# file delete [glob test*]
spawn rm -f $filename.db $filename.ind $filename.lst

# call program
#spawn valgrind ./$programName best_fit test
spawn ./$programName best_fit test
expect "Type command and argument/s."
expect "exit"

# ADD BOOKS
# add first book
send  "add 12346|978-2-12345680-3|El Quijote|Catedra\r"
expect "Record with BookID=12346 has been added to the database"
expect "exit"
send "printInd\n"
expect "exit"

# add second book
send  "add 12345|978-2-12345086-3|La busca|Catedra\r"
expect "Record with BookID=12345 has been added to the database"
expect "exit"
send "printInd\n"
expect "exit"

# add third book
send  "add 12347|978-2-12345680-4|el quijote II|catedra\r"
expect "Record with BookID=12347 has been added to the database"
expect "exit"

# add fourth book
send  "add 12348|978-2-12345086-3|la encuentra|catedra\r"
expect "Record with BookID=12348 has been added to the database"
expect "exit"

# check index
# print index
puts "------------------------"
send "printInd\n"
expect "Entry #0"
expect "    key: #12345"
expect "    offset: #46"
expect "    size: #36"
expect "Entry #1"
expect "    key: #12346"
expect "    offset: #0"
expect "    size: #38"
expect "Entry #2"
expect "    key: #12347"
expect "    offset: #90"
expect "    size: #41"
expect "Entry #3"
expect "    key: #12348"
expect "    offset: #139"
expect "    size: #40"
expect "exit"

#delete book 12347
send "del 12347\r"
expect "Record with BookID=12347 has been deleted"
expect "exit"

# add book same size as 12347
send  "add 12349|978-2-12345680-4|la quijota II|catedra\r"
expect "Record with BookID=12349 has been added to the database"
expect "exit"

#check index
puts "------------------------"
send "printInd\n"
expect "Entry #0"
expect "    key: #12345"
expect "    offset: #46"
expect "    size: #36"
expect "Entry #1"
expect "    key: #12346"
expect "    offset: #0"
expect "    size: #38"
expect "Entry #2"
expect "    key: #12348"
expect "    offset: #139"
expect "    size: #40"
expect "Entry #3"
expect "    key: #12349"
expect "    offset: #90"
expect "    size: #41"
expect "exit"

send "exit\n"
expect "all done"
