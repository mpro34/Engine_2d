# Engine_2d

This project is a 2D Game engine using SDL. The game data is configured and loaded with a Lua script.

The main entry point to the code base is the Game.cpp file which takes care of the managers, game loop, etc.

### How it works
The fundamental base class is the Entity class, which everything on the game screen starts off as. The player, enemies and even the tiles all start off as a base Entity class and then various Components are added to each Entity based on what it needs.
For example, the player Entity has a CollisionComponent so that we can track if anything hits the player. Alternatively, a tile Entity which holds information about a certain tile in the map may or may not have a Collision component so that the player can fly over it.


The following items are todo items that will help improve the overall experience of the engine:
1. Add projectiles to player with space bar to shoot
2. Optimize loops and other performance tuning.
3. Add more levels with lua scripting and load next levels once finish is reached.
4. Add map editor


#### Class Diagram for 2D Engine
![Alt text](https://github.com/mpro34/Engine_2d/blob/master/extra/class_diagram.png)

#### The screenshot below shows the starting location for the player, in this case is an animated chopper image. The objective is to get to the "H" helipad while avoiding enemy projectiles (yellow balls) and any other obstacles.
![Alt text](https://github.com/mpro34/Engine_2d/blob/master/extra/start_level_new.png)

#### The screenshot below shows the player on its way to the goal and the various types of enemies. There is a camera that follows the main player transform as well.
![Alt text](https://github.com/mpro34/Engine_2d/blob/master/extra/playing_new.png)
