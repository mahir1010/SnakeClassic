# SnakeClassic

1. Install and configure <a href="https://wiki.libsdl.org/Installation" target="_blank">SDL2</a>&amp;<a
    href="https://developer.nvidia.com/cuda-downloads" target="_blank">CUDA</a>
2. Define following variables in ``compile.sh``
   * SnakeAIPath: Path to the root of [SnakeAI](https://mahir1010.github.io/SnakeAI/)
   * CUDA_PATH: Path to the root of CUDA installation
   * search_depth: Creates kernel with width of (search_depth*2+1)
        
Example: &nbsp; search_depth=3

| -3 | -3 | -3 | -3 | -3 | -3 | -3 |
|----|----|----|----|----|----|----|
| -3 | -2 | -2 | -2 | -2 | -2 | -3 |
| -3 | -2 | -1 | -1 | -1 | -2 | -3 |
| -3 | -2 | -1 | 0  | -1 | -2 | -3 |
| -3 | -2 | -1 | -1 | -1 | -2 | -3 |
| -3 | -2 | -2 | -2 | -2 | -2 | -3 |
| -3 | -3 | -3 | -3 | -3 | -3 | -3 |

3. Compile


Compilation Options:
1. To play ```./compile.sh```
![Self](https://raw.githubusercontent.com/mahir1010/SnakeClassic/screenshot/SnakeClassic.gif)
2. To select probabilistic heurisitc algorithm ```./compile.sh 0```
![prob](https://raw.githubusercontent.com/mahir1010/SnakeAI/screenshot/Probabilistic%20Heuristic.gif)
3. To select Greedy Algorithm ```./compile.sh 1```
![greedy locally optimum](https://raw.githubusercontent.com/mahir1010/SnakeAI/screenshot/locally%20optimum.gif)
