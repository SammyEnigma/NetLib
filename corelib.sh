DIR=CoreLib
if [ -d "$DIR" ]; 
	then
		echo "$DIR exists, getting latest ..."
		git pull
	else
		echo "$DIR does not exists, cloning now ..."
		git clone https://github.com/SRIIIXI/CoreLib.git
fi