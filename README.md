# SnakeClassic

1. Install and configure <a href="https://wiki.libsdl.org/Installation" target="_blank">SDL2</a>&nbsp;<a
    href="https://developer.nvidia.com/cuda-downloads" target="_blank">CUDA</a>(optional)&nbsp;<a
    href="https://www.libsdl.org/projects/SDL_net/" target="_blank">SDL2_Net</a>
2. Configure variables in ``compile.sh``. See <a href="https://mahir1010.github.io/SnakeAI/" target="_blank">SnakeAI</a> for details.
3. Compile

Options:



| option | Description |
| ------- | ----------|
| - | play the game |
|   0    | probabilistic heurisitc algorithm (requires CUDA) |
|   1    | Greedy algorithm |
|   2    | Network Controller: Accepts commands over TCP |


Compilation Options:
1. To play ```./compile.sh```
![Self](https://raw.githubusercontent.com/mahir1010/SnakeClassic/screenshot/SnakeClassic.gif)
2. To select probabilistic heurisitc algorithm ```./compile.sh 0```
![prob](https://raw.githubusercontent.com/mahir1010/SnakeAI/screenshot/Probabilistic%20Heuristic.gif)
3. To select Greedy Algorithm ```./compile.sh 1```
![greedy locally optimum](https://raw.githubusercontent.com/mahir1010/SnakeAI/screenshot/locally%20optimum.gif)
