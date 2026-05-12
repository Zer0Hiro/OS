echo "START"
mkdir $1
cd $1
touch $USER
man ls | head -n 5 > $USER
man ls | tail -n 5 >> $USER
chmod u-w $USER
touch home.txt
echo $USER > home.txt
cd ..
echo "END OF SCRIPT"





