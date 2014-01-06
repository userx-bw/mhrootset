#!/bin/bash

# written just for mhsetroot 
# changes the set time intervals when the wallpaper and color changes right from the command line
# defualt is 3 minutes 
# just type this in at the command line if you put it in your home directory
#
# ./<script name> <time you want the WP to change in minues> >/dev/null 2>/dev/null
#
#written by Michael Heras Nov. 30, 2013
#
# 


stTime=1
Ntime=$1


function rst(){

if [[ Ntime -le stTime ]]; then
	resetT=$((3*60)) #default 3 minutes 
	else
	resetT=$((Ntime*60))
return $resetT #pass the var to space land

fi

}

rst

#kill the prior pids leaving the new one
function killoldPid(){
# add the script name <script name> remove the < > brackets also, when adding the what ever you named this script
if [ $(pgrep -u $USER <script name> | wc -l) -ge 2 ] && [ $(pgrep -u $USER sleep | wc -l ) -ge 1 ]; then
	echo $(pgrep -u $USER <script name> | wc -l) "--" $(pgrep -u $USER sleep | wc -l)
	pkill -u $USER -o sleep
	pkill -u $USER -o <script name> # <--you need to add what you call this script here <script-name>
else
         # add script name here too 
echo "<script name> pid $(pgrep -u $USER <script name> ) sleep pid $(pgrep -u $USER sleep)"
fi

}

color() { 
echo \#$(head -n 1 /dev/urandom | od -t x1 -N 9 | awk '{print $2 $3 $4}') 
}

color2() { 
echo \#$(head -n 1 /dev/urandom | od -t x1 -N 15 | awk '{print $2 $3 $4}') 
}


tint() { 
echo \#$(head -n 1 /dev/urandom | od -t x1 -N 3 | awk '{print $2 $3 $4}') 
}

addSetNum1()
{
 RANGE=90
 echo
 number=$RANDOM
let "number %= $RANGE"
  
 }
 
 addSetNum2()
{
 RANGE=90
 echo
 number=$RANDOM
let "number %= $RANGE"
  
 }
 
 setGradient()
 {
	 
	 RANGE=360
	 number=$RANDOM
	 let "number %= $RANGE"
 }
 setContrast()
 {
	 RANGE=3
	 number=$RANDOM
	 let "number %= $RANGE"
 }
 

flipS() {
    local options=("" "-fliph" "-flipd" "-flipv")
    local index=$(( RANDOM % ${#options[@]} ))
    setS=${options[$index]}
   }

flipS

#put it in a loop so that it keeps on keeping on

function wps(){


if [[ $resetT -gt 10 ]]; then

nw="$resetT"

echo "$renew --"

#set your wallpaper directory here
WALLPAPERS=/path/to/your/wallpaper
{
    while true; do   
		
		find "$WALLPAPERS" -type f \( -name '*.jpg' -o -name '*.png' \) -print0 | shuf -n 1 -z | xargs -0 \
		mhsetroot -addd $(color) "$(addSetNum1)" -addd $(color2) "$(addSetNum2)" -gradient "$(setGradient)" \
		-tint $(tint) -brightness "$(setContrast)" ${setS} -tilev 200x200  
						#changes options ie -tile -fill -dia -center -flipimg then add the prams needed after
			
		echo "sleep time changed to $(($nw/60)) minutes"
			
		sleep "$nw"
		killoldPid
		
	done &
}
fi
}

#call function to make it work
wps

