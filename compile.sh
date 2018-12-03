SnakeAIPath="../SnakeAI"
search_depth=3
gcc -c Snake.c -O3
if [  $# -gt 0 ] 
  then
  if [ $1 -eq 0 ]
  then
    nvcc -c $SnakeAIPath/*.cu $SnakeAIPath/probabilistic_heuristic.c $SnakeAIPath/utility.c -O3 -I ./includes/ -I ./  -I $CUDA_HOME/include -arch=sm_61 -rdc=true -D PROB_HEURISTIC -D AIMODE -D SEARCH_DEPTH=$search_depth 
    gcc -c main.c -O3 -I $SnakeAIPath/ -I ./includes/ -I $CUDA_HOME/include/ -D AIMODE -D PROB_HEURISTIC 
    nvcc *.o -o Snake -lSDL2 -lm -arch=sm_61  -lcudadevrt -O3
    rm *.o
    exit
  elif [ $1 -eq 1 ]
    then
    gcc -c $SnakeAIPath/greedy_local_optimum.c -I ./includes/ -D AIMODE -O3
    gcc -c main.c -I $SnakeAIPath/ -I ./includes/ -D AIMODE -O3
  fi
  else
    gcc -c main.c  -O3
fi
gcc *.o -o Snake -lSDL2 -lm -O3
rm *.o