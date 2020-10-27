sed -e "27 s/m/j/g" -e "28 s/j/m/g" lab1_first.c > lab1_second.c
gcc lab1_second.c -o lab1_second

sed -e "28 s/j/k/g" -e "29 s/k/j/g" lab1_first.c > lab1_third.c
gcc lab1_third.c -o lab1_third

sed -e "28 s/m/k/g" -e "29 s/k/m/g" lab1_second.c > lab1_fourth.c
gcc lab1_fourth.c -o lab1_fourth

sed -e "27 s/m/k/g" -e "28 s/k/m/g" lab1_third.c > lab1_fifth.c
gcc lab1_fifth.c -o lab1_fifth

sed -e "28 s/m/j/g" -e "29 s/j/m/g" lab1_fifth.c > lab1_six.c
gcc lab1_six.c -o lab1_six