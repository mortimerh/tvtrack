#!/bin/bash
now=$(date +"%Y-%m-%d")
filename="../data/showlist_$now.xml"
curl -o $filename http://services.tvrage.com/feeds/show_list.php --compress