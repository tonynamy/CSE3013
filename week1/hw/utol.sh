echo 'Working directory' #/sogang/under/cse20131251/shell 
read dirname #directory 이름을 입력을 저장

if ! [ -d "$dirname" ]  
then
	dirname=`pwd`
fi

if ! [ -x "$dirname" ]
then 
	echo "Cannot Access to Directory"
	exit 1
fi

for dir in *
do

newname=`echo $dir | tr "[a-z] [A-Z]" "[A-Z] [a-z]"` #변수 newname은 dir의  대소문자를tr을 이용해서 바꾼것
mv $dir $newname #$dir을 newname으로 바꾼다. 
done 
