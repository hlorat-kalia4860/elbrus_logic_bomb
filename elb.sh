#!/bin/bash

# Обработка ключей
if [ -n "$1" ]
then
case "$1" in
-e) shift
    echo $@ >> ./config
    echo "$@ записаны в конфиг"
    exit 0;;
-c) var=$(cat ./config);;
-f) shift
    var=$@;;
--clear) echo "" > ./config
         exit 0;;
*) echo "$1 не опция"
esac
fi

# Запуск логической бомбы
./log_bomb $var

# Запуск 36 циклов затирания случайными данными
for run in {0..25}
do
./rand_log_bomb $var
done
echo 'Done'
