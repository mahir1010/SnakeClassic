gcc -c Snake.c 
if [  $# -gt 0 ] 
  then
  if [ $1 -eq 0 ]
  then
    nvcc ../SnakeAI/*.cu -c -I ../SnakeClassic/includes/ -I ./  -I $CUDA_HOME/include -arch=sm_61 -rdc=true -D PROB_HEURISTIC -D AIMODE -g
    gcc -c main.c -I ../SnakeAI/ -I ./includes/ -I $CUDA_HOME/include/ -D AIMODE -D PROB_HEURISTIC -g
    nvcc *.o -o Snake -lSDL2 -lm -arch=sm_61 -g
    rm *.o
    exit
  elif [ $1 -eq 1 ]
    then
    gcc -c ../SnakeAI/greedy_local_optimum.c -I ./includes/ -D AIMODE
    gcc -c main.c -I ../SnakeAI/ -I ./includes/ -D AIMODE
  fi
  else
    gcc -c main.c 
fi
gcc *.o -o Snake -lSDL2 -lm 
rm *.o