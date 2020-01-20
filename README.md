# Engine_2d

This project is a 2D Game engine using SDL. The game data is configured and loaded with a Lua script.

The main entry point to the code base is the Game.cpp file which takes care of the managers, game loop, etc.

The following items are todo items that will help improve the overall experience of the engine:
1. Optimize loops and other performance tuning.
2. Organize the managers and various classes, instead of everything in src/
3. Add more levels with lua scripting and load next levels once finish is reached.
4. Add projectiles to player with space bar to shoot
5. Add map editor
6. Add screenshots to readme
7. Add Class Diagram to readme


#### Class Diagram for 2D Engine
![alt text]()

#### The screenshot below shows the starting location for the player, in this case is an animated chopper image. The objective is to get to the "H" helipad while avoiding enemy projectiles (yellow balls) and any other obstacles.
![alt text](https://github.com/2d_GameEngine/extra/start_level.png)

#### The screenshot below shows the player on its way to the goal and the various types of enemies. There is a camera that follows the main player transform as well!
![alt text](https://github.com/2d_GameEngine/extra/playing.png))