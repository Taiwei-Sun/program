#!/bin/bash

cd /home/soft/server
/usr/bin/nohup /usr/bin/java -Xmx1024M -Xms1024M -jar ./minecraftforge-universal-1.6.4-9.11.1.947.jar &
exit 0
