echo "working directory:"
read directory

if [ -z "$directory" ] # 입력이 없을 경우
then
    cd .  # 현재 디렉토리로 이동
else                   # 입력이 있을 경우
    cd $directory      # 해당 디렉토리로 이동을 시도
    if [ $? -ne 0 ]    # 이동이 실패했을 경우
    then
        echo "Error Message : 해당 디렉토리가 존재하지 않거나 접근 권한이 없음"
        exit 1
    fi
fi

for i in *
do
  if [ -f $i ]  # $i가 파일일 경우
  then
    string=`echo $i|tr '[A-Z][a-z]' '[a-z][A-Z]'`
#    echo $string
    mv $i $string
  fi
done

exit 0
