#!/usr/bin/expect -f
# 02
# add test. Does not check 
# the index or the reuse of deleted registers
# first test
set timeout -1
set filename "test"
set programName "library"

# delete all files starting with $filename
spawn rm -f $filename.db $filename.ind $filename.lst $filename.info

# call program
spawn ./$programName next_fit $filename
expect "Type command and argument/s."
expect "exit"

# add first book
send  "add 12345|978-2-12345680-3|El Quijote|Catedra\r"
expect "Record with BookID=12345 has been added to the database"
expect "exit"

# add second book
send  "add 12346|978-2-12345086-3|La busca|Catedra\r"
expect "Record with BookID=12346 has been added to the database"
expect "exit"

# add third book
send  "add 12347|978-2-12345680-4|el quijote|catedra\r"
expect "Record with BookID=12347 has been added to the database"
expect "exit"

# add fourth book
send  "add 12348|978-2-12345086-3|Harry Potter: La Piedra Filosofal|catedra\r"
expect "Record with BookID=12348 has been added to the database"
expect "exit"

# add fifth book
send  "add 12349|978-2-12356786-3|Moon Knigt|Marvel\r"
expect "Record with BookID=12349 has been added to the database"
expect "exit"

# add sixth book
send  "add 12350|978-2-1235326-3|Ms. Marvel|Marvel\r"
expect "Record with BookID=12350 has been added to the database"
expect "exit"

# add seventh book
send  "add 12351|978-2-1235326-3|The Avengers|Marvel\r"
expect "Record with BookID=12351 has been added to the database"
expect "exit"

# add seventh book
send  "add 12352|978-2-1235326-3|Spiderman|Marvel\r"
expect "Record with BookID=12352 has been added to the database"
expect "exit"

# add eighth book
send  "add 12353|978-2-1235326-3|Spiderman: Miles Morales|Marvel\r"
expect "Record with BookID=12353 has been added to the database"
expect "exit"

# add seventh book
send  "add 12354|978-2-1235326-3|X-Men|Marvel\r"
expect "Record with BookID=12354 has been added to the database"
expect "exit"

# add seventh book
send  "add 12355|978-2-1235326-3|Iron Man|Marvel\r"
expect "Record with BookID=12355 has been added to the database"
expect "exit"

# add nineth book
send  "add 12356|978-2-1235326-3|The Avengers 2|Marvel\r"
expect "Record with BookID=12356 has been added to the database"
expect "exit"

# add nineth book
send  "add 12357|978-2-1235326-3|Ant Man|Marvel\r"
expect "Record with BookID=12357 has been added to the database"
expect "exit"

# add tenth book
send  "add 12358|978-2-1235326-3|Kang: The Conqueror|Marvel\r"
expect "Record with BookID=12358 has been added to the database"
expect "exit"

# add tenth book
send  "add 12359|978-2-1235326-3|Wanda Vision|Marvel\r"
expect "Record with BookID=12359 has been added to the database"
expect "exit"

# add eleventh book
send  "add 12360|978-2-1235326-3|Captain Marvel|Marvel\r"
expect "Record with BookID=12360 has been added to the database"
expect "exit"

# add twelveth book
send  "add 12361|978-2-1235326-3|Captain America|Marvel\r"
expect "Record with BookID=12361 has been added to the database"
expect "exit"

# add thirteenth book
send  "add 12362|978-2-1235326-3|Black Panther|Marvel\r"
expect "Record with BookID=12362 has been added to the database"
expect "exit"

# add fourteenth book
send  "add 12363|978-2-1235326-3|Loki|Marvel\r"
expect "Record with BookID=12363 has been added to the database"
expect "exit"

# add fifteenth book
send  "add 12364|978-2-1235326-3|Dr Strange|Marvel\r"
expect "Record with BookID=12364 has been added to the database"
expect "exit"

# add sixteenth book
send  "add 12365|978-2-1235326-3|Dr Strange 2|Marvel\r"
expect "Record with BookID=12365 has been added to the database"
expect "exit"

# add seventeenth book
send  "add 12366|978-2-1235326-3|Black Panther 2|Marvel\r"
expect "Record with BookID=12366 has been added to the database"
expect "exit"

# add eighteenth book
send  "add 12367|978-2-1235326-3|Kraven|Marvel\r"
expect "Record with BookID=12367 has been added to the database"
expect "exit"

# add nineteenth book
send  "add 12368|978-2-1235326-3|Darth Vader|Panini\r"
expect "Record with BookID=12368 has been added to the database"
expect "exit"

# add twentieth book
send  "add 12369|978-2-1235326-3|Darth Vader 2|Panini\r"
expect "Record with BookID=12369 has been added to the database"
expect "exit"

#----------------------------------------------------------------

# add third book
send  "del 12347\r"
expect "Record with BookID=12347 has been deleted"
expect "exit"

# add thirteenth book
send  "del 12362\r"
expect "Record with BookID=12362 has been deleted"
expect "exit"

# add eighth book
send  "del 12353\r"
expect "Record with BookID=12353 has been deleted"
expect "exit"

# add fourteenth book
send  "del 12363\r"
expect "Record with BookID=12363 has been deleted"
expect "exit"

# add seventh book
send  "del 12352\r"
expect "Record with BookID=12352 has been deleted"
expect "exit"

send    "printLst\r"
expect  "Entry #0"
expect      "offset: #90"
expect      "size: #38"
expect  "Entry #1"
expect      "offset: #806"
expect      "size: #40"
expect  "Entry #2"
expect      "offset: #382"
expect      "size: #51"
expect  "Entry #3"
expect      "offset: #853"
expect      "size: #31"
expect  "Entry #4"
expect      "offset: #339"
expect      "size: #36"

send  "add 13253|978-2-1235326-3|Spiderman: Miles Morales|Marvel\r"
expect "Record with BookID=13253 has been added to the database"
expect "exit"

send "printLst\r"
expect  "Entry #0"
expect      "offset: #90"
expect      "size: #38"
expect  "Entry #1"
expect      "offset: #806"
expect      "size: #40"
expect  "Entry #2"
expect      "offset: #853"
expect      "size: #31"
expect  "Entry #3"
expect      "offset: #339"
expect      "size: #36"

send "add 13452|978-2-12345086-3|Harry Potter: La Piedra Filosofal|catedra\r"
expect "Record with BookID=13452 has been added to the database"
expect "exit"

send "printLst\r"
expect  "Entry #0"
expect      "offset: #90"
expect      "size: #38"
expect  "Entry #1"
expect      "offset: #806"
expect      "size: #40"
expect  "Entry #2"
expect      "offset: #853"
expect      "size: #31"
expect  "Entry #3"
expect      "offset: #339"
expect      "size: #36"

expect "exit"
send "exit\n"
expect "all done"

# call program
spawn ./$programName next_fit $filename
expect "Type command and argument/s."
expect "exit"

send "printLst\r"
expect  "Entry #0"
expect      "offset: #90"
expect      "size: #38"
expect  "Entry #1"
expect      "offset: #806"
expect      "size: #40"
expect  "Entry #2"
expect      "offset: #853"
expect      "size: #31"
expect  "Entry #3"
expect      "offset: #339"
expect      "size: #36"

send "add 13347|978-2-12345680-4|el quijote|catedra\r"
expect "Record with BookID=13347 has been added to the database"
expect "exit"

send "printLst\r"
expect  "Entry #0"
expect      "offset: #806"
expect      "size: #40"
expect  "Entry #1"
expect      "offset: #853"
expect      "size: #31"
expect  "Entry #2"
expect      "offset: #339"
expect      "size: #36"

expect "exit"
send "exit\r"
expect "all done"

spawn ./$programName next_fit $filename

send "printLst\r"
expect  "Entry #0"
expect      "offset: #806"
expect      "size: #40"
expect  "Entry #1"
expect      "offset: #853"
expect      "size: #31"
expect  "Entry #2"
expect      "offset: #339"
expect      "size: #36"

send   "add 13469|978-2-1235326-3|Darth Vader 2|Panini\r"
expect "Record with BookID=13469 has been added to the database"
expect "exit"

send "printLst\r"
expect  "Entry #0"
expect      "offset: #853"
expect      "size: #31"
expect  "Entry #1"
expect      "offset: #339"
expect      "size: #36"
expect "exit"

send "exit\r"
expect "all done"