#!/bin/bash

./server&

sleep 1

./client "This test has a really long word that is over 80 characters to test functionality 123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890 and thats the end of that"

./client "This test has over 500 characters and should truncate down to 500 characters but otherwise function normally. snake dog female tiki torch fans alligators papacy evangelicals theocracy Lorem ipsum dolor sit amet, diceret evertitur dissentias cu eam, dolor alienum vulputate ex per. In est nibh veri quidam. Ea sea agam rebum graecis, cu duo utamur disputando, petentium neglegentur comprehensam ne duo. Usu deleniti partiendo an. Per omnis putant i, vim eu tempor platonem vituperatoribus. ending now!############################### ##################### ##############"

for number in {0..1000}
do
./client >>file1.txt "having                         long                                  spaces                    in                  the                   text."
done

killall -9 server
