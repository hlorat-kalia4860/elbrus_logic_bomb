#!/bin/bash

# Запуск логической бомбы
./log_bomb $@

# Запуск 36 циклов затирания случайными данными
for run in {0..25}
do
./rand_log_bomb $@
done
echo 'Done'
